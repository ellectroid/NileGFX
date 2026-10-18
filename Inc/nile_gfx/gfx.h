#ifndef LIB_INC_NILE_GFX_NILE_RENDERER_H_
#define LIB_INC_NILE_GFX_NILE_RENDERER_H_

#include "nile_gfx/gfx_control.h"
#include "nile_gfx/gfx_config.h"
#include "nile_gfx/gfx_stdtypes.h"

void gfx_hw_init(void); //for low level hardware configuration
void gfx_init(void); //for graphics kernel configuration
void gfx_start(void); //for starting the gfx kernel

nile_gfx_draw_op* draw_op_get_next(nile_gfx *gfx);
void gfx_queue_traverse_reset(nile_gfx *gfx);
void gfx_error_reset(nile_gfx *gfx);

void gfx_dma2d_parse_op(nile_gfx *gfx, nile_gfx_draw_op *op);
void gfx_dma2d_op_start(void);


#endif /* LIB_INC_NILE_GFX_NILE_RENDERER_H_ */
