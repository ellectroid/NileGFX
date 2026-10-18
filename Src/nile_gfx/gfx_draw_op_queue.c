#include "nile_gfx/gfx.h"

nile_gfx_draw_op* nile_gfx_draw_op_queue_push(nile_gfx_draw_op_queue *q, nile_gfx_draw_op *new_op) {
	if (q->used >= q->capacity) {
		// Queue full
		return 0;
	}

	// Copy the operation into the queue
	q->qspace_first[q->head] = *new_op;

	// Capture pointer to the inserted element
	nile_gfx_draw_op *inserted = &q->qspace_first[q->head];

	// Advance head with wraparound logic
	q->head++;
	if (q->head >= q->capacity) {
		q->head = 0;
	}

	q->used++;
	return inserted;
}

nile_gfx_draw_op* gfx_draw_op_queue_push(uint32_t qid, nile_gfx_draw_op *new_op) {
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if (qid >= drqn)
		return 0;
	return nile_gfx_draw_op_queue_push(&GFX->DQ[qid], new_op);
}

nile_gfx_draw_op* nile_gfx_draw_op_queue_peek(nile_gfx_draw_op_queue *q) {
	if (q->used == 0) {
		return 0;
	}

	return &q->qspace_first[q->tail];
}

nile_gfx_draw_op* gfx_draw_op_queue_peek(uint32_t qid) {
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if (qid >= drqn)
		return 0;
	return nile_gfx_draw_op_queue_peek(&GFX->DQ[qid]);
}

void nile_gfx_draw_op_queue_pop(nile_gfx_draw_op_queue *q) {
	if (q->used == 0) {
		return;
	}

	q->tail++;
	if (q->tail >= q->capacity) {
		q->tail = 0;
	}

	q->used--;
}

void gfx_draw_op_queue_pop(uint32_t qid) {
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if (qid >= drqn)
		return;
	nile_gfx_draw_op_queue_pop(&GFX->DQ[qid]);
}

nile_gfx_draw_op* nile_gfx_draw_op_queue_peek_nth(nile_gfx_draw_op_queue *q, uint16_t peek_index) {
	if (peek_index >= q->used) {
		return 0; // Out of bounds
	}

	uint32_t index = q->tail + peek_index;
	if (index >= q->capacity) {
		index -= q->capacity; // Wrap around
	}

	return &q->qspace_first[index];
}

nile_gfx_draw_op* gfx_draw_op_queue_peek_nth(uint32_t qid, uint16_t peek_index) {
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if (qid >= drqn)
		return 0;
	return nile_gfx_draw_op_queue_peek_nth(&GFX->DQ[qid], peek_index);
}

nile_gfx_draw_op* nile_gfx_draw_op_queue_peek_next(nile_gfx_draw_op_queue *q, nile_gfx_draw_op *parent) {
	nile_gfx_draw_op *next = parent + 1;

	if (next > q->qspace_last) {
		next = q->qspace_first; // Wrap around to start of buffer
	}

	// Mask out next if we've reached the head index
	return (nile_gfx_draw_op*) ((uint32_t) next & ((uint32_t) 0 - (uint32_t) ((uint32_t) (next - q->qspace_first) != q->head)));
}

void nile_gfx_draw_op_queue_flush(nile_gfx_draw_op_queue *q) {
	q->tail = 0;
	q->head = 0;
	q->used = 0;
}

void gfx_draw_op_queue_flush(uint32_t qid) {
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if (qid >= drqn)
		return;
	nile_gfx_draw_op_queue *q = &GFX->DQ[qid];
	q->tail = 0;
	q->head = 0;
	q->used = 0;
}
