#include "nile_gfx/gfx.h"
#include "nile_gfx/gfx_api_draw.h"
#include "stm32f746xx.h"

// Divide two integers and return a Q13.3 fixed-point result:
//   num_int/den_int in Q13.3
static int32_t div_to_q13_3(int32_t num_int, int32_t den_int) {
    // Multiply numerator by 2^3 before division
    return (den_int != 0)
        ? (int32_t)((num_int << 3) / den_int)
        : 0;
}

void rasterize_triangle(
    int16_t   p1_x, int16_t   p1_y,  // top vertex
    int16_t   p2_x, int16_t   p2_y,  // middle vertex
    int16_t   p3_x, int16_t   p3_y,  // bottom vertex
    uint16_t* output_y_bounds,       // [y_min, y_max]
    uint16_t* output_x_coords        // [x0, x1, x0, x1, …]
) {
    // 1) Require p1_y ≤ p2_y ≤ p3_y
    if (p1_y > p2_y || p2_y > p3_y) return;
    if (p1_y == p3_y) return;  // no height

    // 2) Convert vertices to Q13.3
    int32_t x1_fp = ((int32_t)p1_x) << 3;
    int32_t y1_fp = ((int32_t)p1_y) << 3;
    int32_t x2_fp = ((int32_t)p2_x) << 3;
//    int32_t y2_fp = ((int32_t)p2_y) << 3;
//    int32_t x3_fp = ((int32_t)p3_x) << 3;
    int32_t y3_fp = ((int32_t)p3_y) << 3;
    y1_fp += 7;
    y3_fp -= 7;

    // 3) Compute integer Y bounds
    int16_t y_min = p1_y < 0 ? 0 : p1_y;
    int16_t y_max = p3_y >= NILE_GFX_SCREEN_HEIGHT
                  ? NILE_GFX_SCREEN_HEIGHT - 1
                  : p3_y;
    output_y_bounds[0] = (uint16_t)y_min;
    output_y_bounds[1] = (uint16_t)y_max;

    // 4) Precompute inverse slopes in Q13.3:
    //    long edge from p1→p3
    int32_t inv_long = div_to_q13_3(p3_x - p1_x, p3_y - p1_y);
    //    top half from p1→p2
    int32_t inv_top  = div_to_q13_3(p2_x - p1_x, p2_y - p1_y);
    //    bottom half from p2→p3
    int32_t inv_bot  = div_to_q13_3(p3_x - p2_x, p3_y - p2_y);

    // 5) Initialize edge X positions at scanline y_min
    //    Use Q13.3 and center at pixel by +4 if desired
    int32_t x_long_fp  = x1_fp;
    int32_t x_short_fp = x1_fp;

    int idx = 0;

    // 6) Rasterize top half (y from p1_y to p2_y)
    for (int16_t y = y_min; y <= p2_y && y <= y_max; ++y) {
        // Round to integer X: nearest pixel
        int16_t xs = (int16_t)((x_short_fp + 4) >> 3);
        int16_t xl = (int16_t)((x_long_fp  + 4) >> 3);
        if (xs > xl) { int16_t t=xs; xs=xl; xl=t; }
        if (xs < 0) xs = 0;
        if (xl >= NILE_GFX_SCREEN_WIDTH) xl = NILE_GFX_SCREEN_WIDTH - 1;
        output_x_coords[idx++] = (uint16_t)xs;
        output_x_coords[idx++] = (uint16_t)xl;

        // Advance edges by one scanline
        x_short_fp += inv_top;
        x_long_fp  += inv_long;
    }

    // 7) Reset short edge to p2_x and continue bottom half
    x_short_fp = x2_fp;

    for (int16_t y = p2_y + 1; y <= y_max; ++y) {
        int16_t xs = (int16_t)((x_short_fp + 4) >> 3);
        int16_t xl = (int16_t)((x_long_fp  + 4) >> 3);
        if (xs > xl) { int16_t t=xs; xs=xl; xl=t; }
        if (xs < 0) xs = 0;
        if (xl >= NILE_GFX_SCREEN_WIDTH) xl = NILE_GFX_SCREEN_WIDTH - 1;
        output_x_coords[idx++] = (uint16_t)xs;
        output_x_coords[idx++] = (uint16_t)xl;

        x_short_fp += inv_bot;
        x_long_fp  += inv_long;
    }
}


uint16_t gfx_rasterize_draw_triangle(nile_gfx_draw_op *op_list, int16_t p1_x, int16_t p1_y, int16_t p2_x, int16_t p2_y, int16_t p3_x, int16_t p3_y,
		uint32_t argb8888) {
	if (!op_list)
		return 0;

	uint16_t y_coords[2] = { UINT16_MAX, UINT16_MAX };
	uint16_t x_coords[2 * NILE_GFX_SCREEN_HEIGHT];
//sort by y
	if (p2_y < p1_y) {
		uint16_t temp_x = p1_x;
		uint16_t temp_y = p1_y;
		p1_x = p2_x;
		p1_y = p2_y;
		p2_x = temp_x;
		p2_y = temp_y;
	}
	if (p3_y < p1_y) {
		uint16_t temp_x = p1_x;
		uint16_t temp_y = p1_y;
		p1_x = p3_x;
		p1_y = p3_y;
		p3_x = temp_x;
		p3_y = temp_y;
	}
	if (p3_y < p2_y) {
		uint16_t temp_x = p2_x;
		uint16_t temp_y = p2_y;
		p2_x = p3_x;
		p2_y = p3_y;
		p3_x = temp_x;
		p3_y = temp_y;
	}

	rasterize_triangle(p1_x, p1_y, p2_x, p2_y, p3_x, p3_y, y_coords, x_coords);
	if (y_coords[0] == UINT16_MAX)
		return 0;
	uint16_t rect_it = 0;
	for (uint16_t i = y_coords[0]; i <= y_coords[1]; i++) {
		gfx_draw_rectangle(&op_list[rect_it], x_coords[2 * rect_it], i, x_coords[2 * rect_it + 1] + 1, i + 1, argb8888);
		rect_it++;
	}
	return rect_it;
}
