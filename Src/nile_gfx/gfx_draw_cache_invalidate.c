#include "nile_gfx/gfx.h"

void gfx_draw_cache_invalidate(){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	GFX->CR1 |= NILE_GFX_CR1_CDIR;
}
