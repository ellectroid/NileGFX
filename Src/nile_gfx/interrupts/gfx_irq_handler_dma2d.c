#include "stm32f746xx.h"
#include "nile_gfx/gfx.h"

void DMA2D_IRQHandler(void) {
	//nile_kernel *kk = (nile_kernel*) NILE_KERNEL_MEMORY_START;
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_MEMSPACE_BASE_ADDR;

	if ((DMA2D->ISR & (DMA2D_ISR_CEIF | DMA2D_ISR_TEIF)) && (DMA2D->CR & (DMA2D_CR_CEIE | DMA2D_CR_TEIE)))
		goto error_handling;

	if ((DMA2D->ISR & DMA2D_ISR_TCIF) && (DMA2D->CR & DMA2D_CR_TCIE)) {
		// transfer complete handling
		DMA2D->IFCR = DMA2D_IFCR_CTCIF;
		// get the next operation in the queue to apply to DMA2D
		nile_gfx_draw_op *new_op = draw_op_get_next(GFX);
		// if returned nullptr, we're done for this sequence, disable DMA2D interrupts
		if (!new_op) {
			gfx_queue_traverse_reset(GFX);
			DMA2D->CR &= ~(DMA2D_CR_CEIE | DMA2D_CR_TEIE | DMA2D_CR_TCIE);
			DMA2D->IFCR = DMA2D_IFCR_CCEIF | DMA2D_IFCR_CTEIF | DMA2D_IFCR_CTCIF;
			LTDC->ICR = LTDC_ICR_CRRIF; //clear register reload interrupt flag
			LTDC->SRCR = LTDC_SRCR_VBR; //make it auto-reload when next frame finishes
			LTDC->IER |= LTDC_IER_RRIE; //enable wait for the next blanking to switch buffers and draw again
			// the queue drawing state already reset itself upon nullptr return
			if(GFX->BLSIG) *GFX->BLSIG = 1 << 16;
			if(GFX->ACTSIG) *GFX->ACTSIG = 0;
			goto transfer_complete_end;
		}
		gfx_dma2d_parse_op(GFX, new_op);
		gfx_dma2d_op_start();
		transfer_complete_end: return;
	}

	error_handling:
	// configuration error handling
	// transfer error handling
	GFX->CR1 |= NILE_GFX_CR1_HWERR; //DMA2D error happened, the renderer should know
	gfx_queue_traverse_reset(GFX);
	DMA2D->CR &= ~(DMA2D_CR_CEIE | DMA2D_CR_TEIE | DMA2D_CR_TCIE);
	DMA2D->IFCR = DMA2D_IFCR_CCEIF | DMA2D_IFCR_CTEIF | DMA2D_IFCR_CTCIF;

}
