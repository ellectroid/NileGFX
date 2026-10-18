#include "nile_gfx/gfx.h"
#include "stm32f746xx.h"

static void gfx_dma2d_load_rectangle(nile_gfx *gfx, nile_gfx_draw_op *op);
static void gfx_dma2d_load_texture(nile_gfx *gfx, nile_gfx_draw_op *op);
static void* gfx_dma2_resolve_target_fb(nile_gfx *gfx);

void gfx_dma2d_parse_op(nile_gfx *gfx, nile_gfx_draw_op *op) {
	if (!gfx || !op)
		return;

	uint32_t primtype = op->CR1 & NILE_GFX_DRAW_OP_CR1_PRIMTYPE_MASK;

	switch (primtype) {
	case NILE_GFX_DRAW_OP_CR1_PRIMTYPE_RECT:
		gfx_dma2d_load_rectangle(gfx, op);
		break;

	case NILE_GFX_DRAW_OP_CR1_PRIMTYPE_TEXTURE:
		gfx_dma2d_load_texture(gfx, op);
		break;

	default:
		// Unsupported or dummy op
		break;
	}
}

void gfx_dma2d_op_start() {
	DMA2D->CR |= DMA2D_CR_START;
}

static void gfx_dma2d_load_rectangle(nile_gfx *gfx, nile_gfx_draw_op *op) {
	uint16_t x1 = (op->P1R & NILE_GFX_DRAW_OP_P1R_XPOS_MASK) >> NILE_GFX_DRAW_OP_P1R_XPOS_POS;
	uint16_t y1 = (op->P1R & NILE_GFX_DRAW_OP_P1R_YPOS_MASK) >> NILE_GFX_DRAW_OP_P1R_YPOS_POS;
	uint16_t x2 = (op->P2R & NILE_GFX_DRAW_OP_P2R_XPOS_MASK) >> NILE_GFX_DRAW_OP_P2R_XPOS_POS;
	uint16_t y2 = (op->P2R & NILE_GFX_DRAW_OP_P2R_YPOS_MASK) >> NILE_GFX_DRAW_OP_P2R_YPOS_POS;

	uint32_t width = x2 - x1;
	uint32_t height = y2 - y1;
	uint32_t color = op->COLR;

	void *output_fb = gfx_dma2_resolve_target_fb(gfx);
	if (!output_fb)
		return;

	if (op->CR1 & NILE_GFX_DRAW_OP_CR1_STGT_MASK) {
		switch (op->CR1 & NILE_GFX_DRAW_OP_CR1_STGT_MASK) {
		case (NILE_GFX_DRAW_OP_CR1_STGT_FB0):
			output_fb = gfx->FB[0];
			break;
		case (NILE_GFX_DRAW_OP_CR1_STGT_FB1):
			output_fb = gfx->FB[1];
			break;
		case (NILE_GFX_DRAW_OP_CR1_STGT_CACHE):
			output_fb = gfx->FB_CACHE;
			break;
		case (NILE_GFX_DRAW_OP_CR1_STGT_LIVE_DRAW_FB):
			output_fb = gfx->FB_DRAW;
			break;
		default:
			output_fb = 0;
		}
	}

	DMA2D->OCOLR = color;
	DMA2D->OMAR = (uint32_t) output_fb + (y1 * NILE_GFX_SCREEN_WIDTH + x1) * 4;
	DMA2D->OOR = NILE_GFX_SCREEN_WIDTH - width;
	DMA2D->NLR = (width << 16) | height;
	DMA2D->CR &= ~DMA2D_CR_MODE_Msk;
	DMA2D->CR |= 0x03 << DMA2D_CR_MODE_Pos;
}

//static void gfx_dma2d_load_texture(nile_gfx *gfx, nile_gfx_draw_op *op) {
//	uint16_t x1 = (op->P1R & NILE_GFX_DRAW_OP_P1R_XPOS_MASK) >> NILE_GFX_DRAW_OP_P1R_XPOS_POS;
//	uint16_t y1 = (op->P1R & NILE_GFX_DRAW_OP_P1R_YPOS_MASK) >> NILE_GFX_DRAW_OP_P1R_YPOS_POS;
//	uint16_t x2 = (op->P2R & NILE_GFX_DRAW_OP_P2R_XPOS_MASK) >> NILE_GFX_DRAW_OP_P2R_XPOS_POS;
//	uint16_t y2 = (op->P2R & NILE_GFX_DRAW_OP_P2R_YPOS_MASK) >> NILE_GFX_DRAW_OP_P2R_YPOS_POS;
//
//	uint32_t width = x2 - x1;
//	uint32_t height = y2 - y1;
//
//	void *source_fb = (void*) op->EXTRR;
//	void* output_fb = gfx_dma2_resolve_target_fb(gfx);
//	if(!output_fb) return;
//
//	DMA2D->FGPFCCR = 0; //ARGB8888
//	DMA2D->FGMAR = (uint32_t) source_fb;
//	DMA2D->FGOR = 0;
//
//	DMA2D->OMAR = (uint32_t) output_fb + (y1 * NILE_GFX_SCREEN_WIDTH + x1) * 4;
//	DMA2D->OOR = NILE_GFX_SCREEN_WIDTH - width;
//	DMA2D->NLR = (width << 16) | height;
//	DMA2D->CR &= ~DMA2D_CR_MODE_Msk;
//	DMA2D->CR |= 0x00 << DMA2D_CR_MODE_Pos;
//}

static void gfx_dma2d_load_texture(nile_gfx *gfx, nile_gfx_draw_op *op) {
	uint16_t x1 = (op->P1R & NILE_GFX_DRAW_OP_P1R_XPOS_MASK) >> NILE_GFX_DRAW_OP_P1R_XPOS_POS;
	uint16_t y1 = (op->P1R & NILE_GFX_DRAW_OP_P1R_YPOS_MASK) >> NILE_GFX_DRAW_OP_P1R_YPOS_POS;
	uint16_t x2 = (op->P2R & NILE_GFX_DRAW_OP_P2R_XPOS_MASK) >> NILE_GFX_DRAW_OP_P2R_XPOS_POS;
	uint16_t y2 = (op->P2R & NILE_GFX_DRAW_OP_P2R_YPOS_MASK) >> NILE_GFX_DRAW_OP_P2R_YPOS_POS;

	uint32_t width = x2 - x1;
	uint32_t height = y2 - y1;

	void *source_fb = (void*) op->EXTRR;
	if (op->CR1 & NILE_GFX_DRAW_OP_CR1_SSRC_MASK) {
		switch (op->CR1 & NILE_GFX_DRAW_OP_CR1_SSRC_MASK) {
		case (NILE_GFX_DRAW_OP_CR1_SSRC_DISPLAYFB):
			source_fb = gfx->FB[(gfx->CR1 & NILE_GFX_CR1_FBDISPLAY_MASK) >> NILE_GFX_CR1_FBDISPLAY_POS];
		}
	}

	void *output_fb = gfx_dma2_resolve_target_fb(gfx);
	if (op->CR1 & NILE_GFX_DRAW_OP_CR1_STGT_MASK) {
		switch (op->CR1 & NILE_GFX_DRAW_OP_CR1_STGT_MASK) {
		case (NILE_GFX_DRAW_OP_CR1_STGT_FB0):
			output_fb = gfx->FB[0];
			break;
		case (NILE_GFX_DRAW_OP_CR1_STGT_FB1):
			output_fb = gfx->FB[1];
			break;
		case (NILE_GFX_DRAW_OP_CR1_STGT_CACHE):
			output_fb = gfx->FB_CACHE;
			break;
		case (NILE_GFX_DRAW_OP_CR1_STGT_LIVE_DRAW_FB):
			output_fb = gfx->FB_DRAW;
			break;
		default:
			output_fb = 0;
		}
	}
	if (!output_fb)
		return;

	// FG = texture
	DMA2D->FGPFCCR = 0;          // ARGB8888
	DMA2D->FGMAR = (uint32_t) source_fb;// + (y1 * NILE_GFX_SCREEN_WIDTH + x1) * 4;
	DMA2D->FGOR = 0;

	// OMAR always points to the write‐back region
	DMA2D->OMAR = (uint32_t) output_fb + (y1 * NILE_GFX_SCREEN_WIDTH + x1) * 4;
	DMA2D->OOR = NILE_GFX_SCREEN_WIDTH - width;
	DMA2D->NLR = (width << 16) | height;

	// Blending?
	if (op->CR1 & NILE_GFX_DRAW_OP_CR1_BLM_TGT) {
		// BG = current frame as background
		DMA2D->BGPFCCR = 0;  // ARGB8888
		DMA2D->BGMAR = (uint32_t) output_fb + (y1 * NILE_GFX_SCREEN_WIDTH + x1) * 4;
		DMA2D->BGOR = NILE_GFX_SCREEN_WIDTH - width;

		// Set DMA2D mode = memory‐to‐memory blending
		DMA2D->CR &= ~DMA2D_CR_MODE_Msk;
		DMA2D->CR |= (2 << DMA2D_CR_MODE_Pos);
	} else {
		// Set DMA2D mode = plain memory‐to‐memory copy
		DMA2D->CR &= ~DMA2D_CR_MODE_Msk;
		DMA2D->CR |= (0x0 << DMA2D_CR_MODE_Pos);
	}
}

static void* gfx_dma2_resolve_target_fb(nile_gfx *gfx) {
	if (!gfx)
		return 0;

	uint32_t queue_count = (gfx->CR1 & NILE_GFX_CR1_DRQN_MASK) >> NILE_GFX_CR1_DRQN_POS;

	for (uint32_t i = 0; i < queue_count; ++i) {
		if (gfx->CR1 & (1 << (NILE_GFX_CR1_QACTIVE_POS + i))) {
			// Found active queue, now check target
			if (gfx->CR1 & (1 << (NILE_GFX_CR1_QTCEN_POS + i))) {
				return gfx->FB_CACHE;
			} else {
				uint32_t draw_index = (gfx->CR1 & NILE_GFX_CR1_FBDRAW_MASK) >> NILE_GFX_CR1_FBDRAW_POS;
				return gfx->FB[draw_index];
			}
		}
	}

	// No active queue found
	return 0;
}
