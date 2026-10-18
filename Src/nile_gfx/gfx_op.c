#include "nile_gfx/gfx.h"

static inline int queue_is_enabled(nile_gfx *gfx, int qid) {
	return (gfx->CR1 & (1 << (NILE_GFX_CR1_QEN_POS + qid))) != 0;
}

static inline int queue_targets_cache(nile_gfx *gfx, int qid) {
	return (gfx->CR1 & (1 << (NILE_GFX_CR1_QTCEN_POS + qid))) != 0;
}

static inline int queue_destructive_read(nile_gfx *gfx, int qid) {
	return (gfx->CR1 & (1 << (NILE_GFX_CR1_QDRMEN_POS + qid))) != 0;
}

static inline void deactivate_queue(nile_gfx *gfx, int qid) {
	gfx->CR1 &= ~(1 << (NILE_GFX_CR1_QACTIVE_POS + qid));
	gfx->QAR[qid] = 0;
	gfx->CDOPR = 0;
	if (queue_destructive_read(gfx, qid)) {
			nile_gfx_draw_op_queue_flush(&gfx->DQ[qid]);
	}
}

nile_gfx_draw_op* draw_op_get_next(nile_gfx *gfx) {
	// Abort if hardware error or configuration error is flagged
	if ((gfx->CR1 & (NILE_GFX_CR1_HWERR | NILE_GFX_CR1_CERR)) || !(gfx->CR1 & NILE_GFX_CR1_GRENEN)) {
		return 0;
	}

	uint32_t queue_count = (gfx->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	uint32_t cache_enabled = gfx->CR1 & NILE_GFX_CR1_CEN;

	int active_qid = -1;
	for (uint32_t i = 0; i < queue_count; ++i) {
		if (gfx->CR1 & (1 << (NILE_GFX_CR1_QACTIVE_POS + i))) {
			active_qid = i;
			break;
		}
	}

	// No active queue yet
	if (active_qid == -1) {
		nile_gfx_draw_op *first_job = 0;
		for (uint32_t i = 0; i < queue_count; ++i) {
			if (!queue_is_enabled(gfx, i))
				continue;

			first_job = nile_gfx_draw_op_queue_peek(&gfx->DQ[i]);
			if (first_job == 0)
				continue; // empty queue, skip

			// Valid job found, activate queue
			gfx->CR1 |= (1 << (NILE_GFX_CR1_QACTIVE_POS + i));

			// Cache mismatch check
			if (queue_targets_cache(gfx, i) && !cache_enabled) {
				gfx->CR1 |= NILE_GFX_CR1_CERR;
				deactivate_queue(gfx, i);
				return 0;
			}

			gfx->QAR[i] = 0;
			gfx->CDOPR = first_job;
			return first_job;
		}

		// No enabled queue had any jobs
		return 0;
	}

	// Continuing from previous operation
	nile_gfx_draw_op *next = nile_gfx_draw_op_queue_peek_next(&gfx->DQ[active_qid], gfx->CDOPR);
	if (!next) {
		// Exhausted active queue
		deactivate_queue(gfx, active_qid);
		// Try to activate next enabled queue that has jobs
		for (uint32_t i = active_qid + 1; i < queue_count; ++i) {
			if (!queue_is_enabled(gfx, i))
				continue;

			nile_gfx_draw_op *first_job = nile_gfx_draw_op_queue_peek(&gfx->DQ[i]);
			if (first_job == 0)
				continue;

			gfx->CR1 |= (1 << (NILE_GFX_CR1_QACTIVE_POS + i));

			if (queue_targets_cache(gfx, i) && !cache_enabled) {
				gfx->CR1 |= NILE_GFX_CR1_CERR;
				deactivate_queue(gfx, i);
				return 0;
			}

			gfx->QAR[i] = 0;
			gfx->CDOPR = first_job;
			return first_job;
		}

		return 0;
	}

	gfx->QAR[active_qid]++;
	gfx->CDOPR = next;
	return next;
}

void gfx_queue_traverse_reset(nile_gfx *gfx){
	uint32_t queue_count = (gfx->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	for(unsigned int i = 0; i < queue_count; i++){
		gfx->QAR[i] = 0;
	}
	gfx->CR1 &= ~NILE_GFX_CR1_QACTIVE_MASK;
	gfx->CDOPR = 0;
}

void gfx_error_reset(nile_gfx *gfx){
	gfx->CR1 &= ~(NILE_GFX_CR1_CERR | NILE_GFX_CR1_HWERR);
}
