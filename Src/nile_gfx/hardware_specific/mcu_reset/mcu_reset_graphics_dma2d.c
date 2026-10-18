#include "mcu_hw/gpio.h"

void mcu_reset_graphics_dma2d_init(void) {
	/* Activate DMA2D Clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;

	/* Clear all interrupt flags */
	DMA2D->IFCR = 0xFFFFFFFF;
//	DMA2D->CR |= DMA2D_CR_TEIE | DMA2D_CR_TCIE | DMA2D_CR_TWIE | DMA2D_CR_CAEIE
//			| DMA2D_CR_CTCIE | DMA2D_CR_CEIE;
}
