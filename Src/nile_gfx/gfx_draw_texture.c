#include "nile_gfx/gfx.h"
#include "stm32f746xx.h"

void gfx_draw_texture(nile_gfx_draw_op* op, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void* texture_addr) {
    if (!op) return;
    // Set primitive type to TEXTURE, blend mode to NONE, no config error
    op->CR1 = NILE_GFX_DRAW_OP_CR1_PRIMTYPE_TEXTURE | NILE_GFX_DRAW_OP_CR1_BLM_NONE;
    // Define rectangle destination using P1R and P2R
    op->P1R = ((uint32_t)x1 << NILE_GFX_DRAW_OP_P1R_XPOS_POS) |
              ((uint32_t)y1 << NILE_GFX_DRAW_OP_P1R_YPOS_POS);
    op->P2R = ((uint32_t)x2 << NILE_GFX_DRAW_OP_P2R_XPOS_POS) |
              ((uint32_t)y2 << NILE_GFX_DRAW_OP_P2R_YPOS_POS);
    // Set external resource address for texture
    op->EXTRR = (uint32_t)(uintptr_t)texture_addr;
}

void gfx_draw_texture_blend(nile_gfx_draw_op* op, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void* texture_addr) {
    if (!op) return;
    // Set primitive type to TEXTURE, blend mode to BLEND WITH TARGET, no config error
    op->CR1 = NILE_GFX_DRAW_OP_CR1_PRIMTYPE_TEXTURE | NILE_GFX_DRAW_OP_CR1_BLM_TGT;
    // Define rectangle destination using P1R and P2R
    op->P1R = ((uint32_t)x1 << NILE_GFX_DRAW_OP_P1R_XPOS_POS) |
              ((uint32_t)y1 << NILE_GFX_DRAW_OP_P1R_YPOS_POS);
    op->P2R = ((uint32_t)x2 << NILE_GFX_DRAW_OP_P2R_XPOS_POS) |
              ((uint32_t)y2 << NILE_GFX_DRAW_OP_P2R_YPOS_POS);
    // Set external resource address for texture
    op->EXTRR = (uint32_t)(uintptr_t)texture_addr;
}

void gfx_draw_texture_display_buffer(nile_gfx_draw_op* op, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if (!op) return;
    // Set primitive type to TEXTURE, blend mode to NONE, no config error
    op->CR1 = NILE_GFX_DRAW_OP_CR1_PRIMTYPE_TEXTURE | NILE_GFX_DRAW_OP_CR1_BLM_TGT | NILE_GFX_DRAW_OP_CR1_SSRC_DISPLAYFB;
    // Define rectangle destination using P1R and P2R
    op->P1R = ((uint32_t)x1 << NILE_GFX_DRAW_OP_P1R_XPOS_POS) |
              ((uint32_t)y1 << NILE_GFX_DRAW_OP_P1R_YPOS_POS);
    op->P2R = ((uint32_t)x2 << NILE_GFX_DRAW_OP_P2R_XPOS_POS) |
              ((uint32_t)y2 << NILE_GFX_DRAW_OP_P2R_YPOS_POS);
    // Set external resource address for texture
    op->EXTRR = (uint32_t)(uintptr_t)0;
}

