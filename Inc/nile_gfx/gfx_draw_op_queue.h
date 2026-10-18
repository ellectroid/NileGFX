#ifndef NILE_GFX_DRAW_OP_QUEUE_H_
#define NILE_GFX_DRAW_OP_QUEUE_H_

#include "gfx_draw_op.h"

typedef struct{
	uint32_t capacity;
	uint32_t used;
	uint32_t head; //first empty slot
	uint32_t tail; //first used slot
	nile_gfx_draw_op* qspace_first;
	nile_gfx_draw_op* qspace_last;
}
nile_gfx_draw_op_queue;

nile_gfx_draw_op* nile_gfx_draw_op_queue_push(nile_gfx_draw_op_queue* q, nile_gfx_draw_op* new_op);
nile_gfx_draw_op* nile_gfx_draw_op_queue_peek(nile_gfx_draw_op_queue* q);
void nile_gfx_draw_op_queue_pop(nile_gfx_draw_op_queue* q);
nile_gfx_draw_op* nile_gfx_draw_op_queue_peek_nth(nile_gfx_draw_op_queue* q, uint16_t peek_index);
nile_gfx_draw_op* nile_gfx_draw_op_queue_peek_next(nile_gfx_draw_op_queue* q, nile_gfx_draw_op* parent);
void nile_gfx_draw_op_queue_flush(nile_gfx_draw_op_queue* q);


#endif /* NILE_GFX_DRAW_OP_QUEUE_H_ */
