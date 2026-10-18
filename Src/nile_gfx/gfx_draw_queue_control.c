#include "nile_gfx/gfx.h"

void gfx_pipeline_global_render_enable_set(uint32_t enable){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	GFX->CR1 = (GFX->CR1 & ~NILE_GFX_CR1_GRENEN_MASK) | ((!!enable) << NILE_GFX_CR1_GRENEN_POS);
	if(GFX->CR1 & NILE_GFX_CR1_CEN) GFX->CR1 |= NILE_GFX_CR1_CDIR;
}

uint32_t gfx_pipeline_global_render_enable_get(){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	return (GFX->CR1 & NILE_GFX_CR1_GRENEN_MASK);
}

void gfx_pipeline_queue_enable(uint32_t qid){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if(qid >= drqn) return;
	uint32_t qen_bit = (1 << (NILE_GFX_CR1_QEN_POS + qid));
	GFX->CR1 |= qen_bit;

}

void gfx_pipeline_queue_disable(uint32_t qid){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if(qid >= drqn) return;
	uint32_t qen_bit = (1 << (NILE_GFX_CR1_QEN_POS + qid));
	GFX->CR1 &= ~qen_bit;
}

void gfx_pipeline_queue_target_cache_enable(uint32_t qid){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if(qid >= drqn) return;
	uint32_t qtcen_bit = (1 << (NILE_GFX_CR1_QTCEN_POS + qid));
	GFX->CR1 |= qtcen_bit;
}

void gfx_pipeline_queue_target_cache_disable(uint32_t qid){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if(qid >= drqn) return;
	uint32_t qtcen_bit = (1 << (NILE_GFX_CR1_QTCEN_POS + qid));
	GFX->CR1 &= ~qtcen_bit;
}

void gfx_pipeline_queue_destructive_read_mode_enable(uint32_t qid){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if(qid >= drqn) return;
	uint32_t qdrmen_bit = (1 << (NILE_GFX_CR1_QDRMEN_POS + qid));
	GFX->CR1 |= qdrmen_bit;
}

void gfx_pipeline_queue_destructive_read_mode_disable(uint32_t qid){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	if(qid >= drqn) return;
	uint32_t qdrmen_bit = (1 << (NILE_GFX_CR1_QDRMEN_POS + qid));
	GFX->CR1 &= ~qdrmen_bit;
}

uint32_t gfx_pipeline_is_active(void){
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	uint32_t drqn = (GFX->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;
	uint32_t retval = 0;
	for(uint32_t i = 0; i < drqn; i++){
		retval |= GFX->CR1 & (1 << (NILE_GFX_CR1_QACTIVE_POS + i));
	}
	return retval;
}
