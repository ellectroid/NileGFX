#include "nile_gfx/gfx.h"
#include "stm32f7xx.h"

void gfx_start(void) {

	DMA2D->CR &= ~(DMA2D_CR_CEIE | DMA2D_CR_TEIE | DMA2D_CR_TCIE);
	DMA2D->IFCR = DMA2D_IFCR_CCEIF | DMA2D_IFCR_CTEIF | DMA2D_IFCR_CTCIF;
	NVIC_EnableIRQ (DMA2D_IRQn);

	LTDC->IER |= LTDC_IER_RRIE;
	//LTDC->ICR = LTDC_ICR_CRRIF;
	LTDC_Layer2->CFBAR = NILE_GFX_FB1_BASE_ADDR;
	NVIC_EnableIRQ (LTDC_IRQn);

	//simulating having shadow registers preloaded like with interrupt procedure for the first interrupt entry
	LTDC->SRCR = LTDC_SRCR_VBR; //vertical blanking interrupt will start framebuffer switch cycle

}

