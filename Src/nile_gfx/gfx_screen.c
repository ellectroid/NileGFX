#include "nile_gfx/gfx_screen.h"

void gfx_screen_shadow_write(nile_gfx_screen* screen){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	GFX->SCR1 = screen->CR1;
	for(uint32_t i = 0; i < NILE_GFX_CR1_DRQN_MAX; i++){
		GFX->SDQ[i] = screen->DQ[i];
	}
	GFX->SRSEAR = screen->RSEAR;
}
void gfx_screen_shadow_blanking_reload(void){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	GFX->CR1 |= NILE_GFX_CR1_SRELRQ;
}

void gfx_screen_export_set(nile_gfx_screen* out_screen){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	GFX->RSEAR = (void*) out_screen;
}

uint32_t gfx_screen_reload_pending(void){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	return (GFX->CR1 & NILE_GFX_CR1_SRELRQ);
}
