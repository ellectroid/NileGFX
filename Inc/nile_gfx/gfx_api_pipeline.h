#ifndef LIB_INC_GFX_GFX_PIPELINE_API_H_
#define LIB_INC_GFX_GFX_PIPELINE_API_H_
#include "nile_gfx/gfx_stdtypes.h"

void gfx_pipeline_global_render_enable_set(uint32_t enable);
uint32_t gfx_pipeline_global_render_enable_get(void);
void gfx_pipeline_queue_enable(uint32_t qid);
void gfx_pipeline_queue_disable(uint32_t qid);
void gfx_pipeline_queue_target_cache_enable(uint32_t qid);
void gfx_pipeline_queue_target_cache_disable(uint32_t qid);
void gfx_pipeline_queue_destructive_read_mode_enable(uint32_t qid);
void gfx_pipeline_queue_destructive_read_mode_disable(uint32_t qid);

uint32_t gfx_pipeline_is_active(void);



#endif /* LIB_INC_GFX_GFX_PIPELINE_API_H_ */
