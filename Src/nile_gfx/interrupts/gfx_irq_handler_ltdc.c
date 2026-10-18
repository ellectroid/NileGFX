#include "stm32f746xx.h"
#include "nile_gfx/gfx.h"
#include "nile_gfx/gfx_screen.h"

void LCD_TFT_IRQHandler(void) {
//	nile_kernel *kk = (nile_kernel*) NILE_KERNEL_MEMORY_START;
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_MEMSPACE_BASE_ADDR;

	//Only one source - register reload interrupt

	if ((LTDC->ISR & LTDC_ISR_RRIF) && (LTDC->IER & LTDC_IER_RRIE)) {
		LTDC->ICR = LTDC_ICR_CRRIF; //clear register reload interrupt flag
		//New shadow register buffer value has just been loaded by vertical blanking
		//We're now at the vertical blanking

		uint32_t past_display_buffer = (GFX->CR1 & NILE_GFX_CR1_FBDISPLAY_MASK) >> NILE_GFX_CR1_FBDISPLAY_POS;
		uint32_t future_display_buffer = (GFX->CR1 & NILE_GFX_CR1_FBDRAW_MASK) >> NILE_GFX_CR1_FBDRAW_POS;

		//load into shadow register, will not apply until the drawing sequence finished + next blanking
		LTDC_Layer2->CFBAR = (uint32_t) GFX->FB[past_display_buffer];

		//The buffer that was just used for display is now available for drawing
		GFX->CR1 = (GFX->CR1 & ~NILE_GFX_CR1_FBDRAW_MASK) | past_display_buffer << NILE_GFX_CR1_FBDRAW_POS;
		GFX->CR1 = (GFX->CR1 & ~NILE_GFX_CR1_FBDISPLAY_MASK) | future_display_buffer << NILE_GFX_CR1_FBDISPLAY_POS;

		//If shadow screen reload pending, reload registers
		if (GFX->CR1 & NILE_GFX_CR1_SRELRQ) {
			if (GFX->RSEAR) {
				nile_gfx_screen *export_screen = (nile_gfx_screen*) GFX->RSEAR;
				export_screen->CR1 = GFX->CR1;
				for (uint32_t i = 0; i < NILE_GFX_CR1_DRQN_MAX; i++) {
					export_screen->DQ[i] = GFX->DQ[i];
				}
				export_screen->RSEAR = GFX->RSEAR;
			}
			uint32_t fb_config_mask = NILE_GFX_CR1_FBDISPLAY_MASK | NILE_GFX_CR1_FBDRAW_MASK;
			uint32_t fb_config = GFX->CR1 & fb_config_mask;
			uint32_t hw_status_mask = NILE_GFX_CR1_HWERR_MASK | NILE_GFX_CR1_HWRDY_MASK | NILE_GFX_CR1_CERR_MASK | NILE_GFX_CR1_GRENEN_MASK;
			uint32_t hw_status = GFX->CR1 & hw_status_mask;
			GFX->CR1 = fb_config | hw_status | (GFX->SCR1 & ~(fb_config_mask | hw_status_mask));
			for (uint32_t i = 0; i < NILE_GFX_CR1_DRQN_MAX; i++) {
				GFX->DQ[i] = GFX->SDQ[i];
			}
			GFX->RSEAR = GFX->SRSEAR;
			if (GFX->CR1 & NILE_GFX_CR1_CEN)
				GFX->CR1 |= NILE_GFX_CR1_CDIR; //if using cache, invalidate
			GFX->CR1 &= ~NILE_GFX_CR1_SRELRQ;
		}

		if (GFX->CR1 & NILE_GFX_CR1_GRENEN) {
			//No need to check for cache if not drawing
			//Need to update queue policies with respect to caching
			//queues that target cache can be disabled if cache is up to date
			uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
			for (uint32_t qid = 0; qid < drqn; qid++) {
				uint32_t qen_bit = (1 << (NILE_GFX_CR1_QEN_POS + qid));
				uint32_t qtcen_bit = (1 << (NILE_GFX_CR1_QTCEN_POS + qid));
				if (GFX->CR1 & qtcen_bit) {
					if (!(GFX->CR1 & NILE_GFX_CR1_CDIR)) {
						GFX->CR1 &= ~qen_bit; // Cache not dirty, no need to draw those queues
					} else {
						GFX->CR1 |= qen_bit; // Enable queue to redraw to dirty cache
					}
				}
			}
			GFX->CR1 &= ~NILE_GFX_CR1_CDIR; //dirty cache request fulfilled, can safely reset the dirty bit
		}

		//Logic of starting the next DMA2D fetching sequence
		nile_gfx_draw_op *first_op = draw_op_get_next(GFX);
		if (first_op) {
			if(GFX->BLSIG) *GFX->BLSIG = 0;
			if(GFX->ACTSIG) *GFX->ACTSIG = 1 << 16;
			gfx_dma2d_parse_op(GFX, first_op);
			DMA2D->IFCR = DMA2D_IFCR_CTCIF | DMA2D_IFCR_CTEIF | DMA2D_IFCR_CCEIF;
			DMA2D->CR |= DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE;
			LTDC->IER &= ~LTDC_IER_RRIE;
			gfx_dma2d_op_start();
		} else {
			LTDC->SRCR = LTDC_SRCR_VBR; //we'll check if there is anything to render next frame
		}

	}
}
void LTDC_ER_IRQHandler() {

}
