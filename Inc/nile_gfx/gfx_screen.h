#ifndef LIB_INC_GFX_GFX_SCREEN_H_
#define LIB_INC_GFX_GFX_SCREEN_H_
#include "nile_gfx/gfx.h"

typedef struct {
	uint32_t CR1; //control register 1
	nile_gfx_draw_op_queue DQ[NILE_GFX_CR1_DRQN >> NILE_GFX_CR1_DRQN_POS]; //draw queue
	void* RSEAR; //reload export address
}nile_gfx_screen;

void gfx_screen_shadow_write(nile_gfx_screen* screen);
void gfx_screen_shadow_blanking_reload(void);
void gfx_screen_export_set(nile_gfx_screen* out_screen);
uint32_t gfx_screen_reload_pending(void);



#endif /* LIB_INC_GFX_GFX_SCREEN_H_ */
