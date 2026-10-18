#include "nile_gfx/gfx.h"

void gfx_init() {
	//Zero out the memory
	uint32_t *gfx_mem_start = (uint32_t*) NILE_GFX_MEMSPACE_BASE_ADDR;
	uint32_t *gfx_mem_after_end = (uint32_t*) (NILE_GFX_MEMSPACE_BASE_ADDR + NILE_GFX_MEMSPACE_SIZE);
	for (uint32_t *mem = gfx_mem_start; mem < gfx_mem_after_end; mem++) {
		*mem = 0;
	}

	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;

	//Configure control register
	GFX->CR1 |= 4 << NILE_GFX_CR1_DRQN_POS; //4 queues
	GFX->CR1 |= 0 << NILE_GFX_CR1_FBDISPLAY_POS; //FB0 is display FB
	GFX->CR1 |= 1 << NILE_GFX_CR1_FBDRAW_POS; //FB1 is draw FB

	GFX->CR1 |= NILE_GFX_CR1_HWRDY; //just set it to ready for now
	GFX->HWINITR = 0; //to be configured later in development

	//Configure screen info
	GFX->SCR |= NILE_GFX_SCREEN_WIDTH << NILE_GFX_SCR_SWIDTH_POS;
	GFX->SCR |= NILE_GFX_SCREEN_HEIGHT << NILE_GFX_SCR_SHEIGHT_POS;

	//Configure framebuffers
	GFX->FB[0] = (void*) NILE_GFX_FB0_BASE_ADDR;
	GFX->FB[1] = (void*) NILE_GFX_FB1_BASE_ADDR;
	GFX->FB_CACHE = (void*) NILE_GFX_FB2_BASE_ADDR;
	GFX->FB_DRAW = (void*) NILE_GFX_FB3_BASE_ADDR;

	GFX->CR1 |= NILE_GFX_CR1_CEN; //cache on

	//Zero out framebuffers
	gfx_mem_start = (uint32_t*) GFX->FB[0];
	gfx_mem_after_end = gfx_mem_start + NILE_GFX_FB_SIZE / 4;
	for (uint32_t *mem = gfx_mem_start; mem < gfx_mem_after_end; mem++) {
		*mem = 0;
	}
	gfx_mem_start = (uint32_t*) GFX->FB[1];
	gfx_mem_after_end = gfx_mem_start + NILE_GFX_FB_SIZE / 4;
	for (uint32_t *mem = gfx_mem_start; mem < gfx_mem_after_end; mem++) {
		*mem = 0;
	}
	if (GFX->FB_CACHE) {
		gfx_mem_start = (uint32_t*) GFX->FB_CACHE;
		gfx_mem_after_end = gfx_mem_start + NILE_GFX_FB_SIZE / 4;
		for (uint32_t *mem = gfx_mem_start; mem < gfx_mem_after_end; mem++) {
			*mem = 0;
		}
	}
	if (GFX->FB_DRAW) {
		gfx_mem_start = (uint32_t*) GFX->FB_DRAW;
		gfx_mem_after_end = gfx_mem_start + NILE_GFX_FB_SIZE / 4;
		for (uint32_t *mem = gfx_mem_start; mem < gfx_mem_after_end; mem++) {
			*mem = 0;
		}
	}

}

void* gfx_get() {
	return (void*) NILE_GFX_CONTROL_BASE_ADDR;
}
