#include "nile_gfx/gfx.h"

void gfx_draw_rectangle(nile_gfx_draw_op* op, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t argb8888) {
    if (!op) return;
    // Set primitive type to RECT, blend mode to NONE, clear error flag
    op->CR1 = NILE_GFX_DRAW_OP_CR1_PRIMTYPE_RECT | NILE_GFX_DRAW_OP_CR1_BLM_NONE;
    // Set point 1 (upper-left corner)
    op->P1R = ((uint32_t)x1 << NILE_GFX_DRAW_OP_P1R_XPOS_POS) |
              ((uint32_t)y1 << NILE_GFX_DRAW_OP_P1R_YPOS_POS);
    // Set point 2 (bottom-right corner)
    op->P2R = ((uint32_t)x2 << NILE_GFX_DRAW_OP_P2R_XPOS_POS) |
              ((uint32_t)y2 << NILE_GFX_DRAW_OP_P2R_YPOS_POS);
    // Set color
    op->COLR = argb8888;
}
