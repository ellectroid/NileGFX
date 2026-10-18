#ifndef LIB_INC_GFX_GFX_DRAW_API_H_
#define LIB_INC_GFX_GFX_DRAW_API_H_

#include "gfx_draw_op.h"

//Draw queue management
nile_gfx_draw_op* gfx_draw_op_queue_push(uint32_t qid, nile_gfx_draw_op *new_op);
nile_gfx_draw_op* gfx_draw_op_queue_peek(uint32_t qid);
void gfx_draw_op_queue_pop(uint32_t qid);
nile_gfx_draw_op* gfx_draw_op_queue_peek_nth(uint32_t qid, uint16_t peek_index);
void gfx_draw_op_queue_flush(uint32_t qid);

//Cache refresh
void gfx_draw_cache_invalidate();

//Generate operations
void gfx_draw_rectangle(nile_gfx_draw_op* op, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t argb8888);
void gfx_draw_texture(nile_gfx_draw_op* op, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void* texture_addr);
void gfx_draw_texture_blend(nile_gfx_draw_op* op, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void* texture_addr);
void gfx_draw_texture_display_buffer(nile_gfx_draw_op* op, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
uint16_t gfx_rasterize_draw_circle(nile_gfx_draw_op *op_list, int16_t c_x, int16_t c_y, int16_t diameter, uint32_t argb8888);
uint16_t gfx_rasterize_draw_triangle(nile_gfx_draw_op *op_list, int16_t p1_x, int16_t p1_y, int16_t p2_x, int16_t p2_y, int16_t p3_x, int16_t p3_y,
		uint32_t argb8888);

#endif /* LIB_INC_GFX_GFX_DRAW_API_H_ */
