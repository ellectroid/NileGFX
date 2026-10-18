#include "nile_gfx/gfx.h"
#include "nile_gfx/gfx_api_draw.h"
#include "stm32f746xx.h"


static int32_t isqrt_q26_6_to_q13_3(uint32_t n) {
	if (n == 0)
		return 0;

	uint32_t res = 0;
	// Start with the highest possible “bit” for a 32-bit square‐root
	uint32_t bit = 1UL << 30;
	// Scale bit down to ≤ n
	while (bit > n)
		bit >>= 2;
	// Standard binary long‐division style sqrt
	while (bit) {
		if (n >= res + bit) {
			n -= res + bit;
			res = (res >> 1) + bit;
		} else {
			res = (res >> 1);
		}
		bit >>= 2;
	}
	// ‘res’ is sqrt(n) in Q16.6 → convert to Q13.3 by >>3
	return (int32_t) (res);
}

static void rasterize_circle(int16_t c_x, int16_t c_y, int16_t diameter, uint16_t *output_y_boundaries,  // [y_min, y_max]
		uint16_t *output_x_coord_list   // [x0, x1, x0, x1, …]
		) {
	if (diameter <= 0)
		return;

	// 1) Convert center to Q13.3 using 32-bit to avoid overflow:
	int32_t cx_fp = ((int32_t) c_x) << 3;
	int32_t cy_fp = ((int32_t) c_y) << 3;

	// 2) Compute radius in Q13.3: (diameter/2) * 2^3
	//    Use a 32-bit division to handle odd diameters correctly.
	int32_t r_fp = (((int32_t) diameter) << 3) / 2;
	if (diameter & 1) {
		r_fp -= (1 << 1);
	}
	else{
		r_fp += (1 << 1);
	}

	// 3) Precompute r^2 in Q26.6
	uint32_t r2_q26_6 = (uint32_t) (r_fp * r_fp);

	// 4) Determine integer Y bounds
	int16_t y_min = c_y - (diameter >> 1);
	int16_t y_max = c_y + (diameter >> 1);
	if (diameter & 1) {
		//y_min--;
		//y_max++;
	}

	if (y_min < 0)
		y_min = 0;
	if (y_max >= (int16_t)NILE_GFX_SCREEN_HEIGHT)
		y_max = (int16_t)NILE_GFX_SCREEN_HEIGHT - 1;

	output_y_boundaries[0] = (uint16_t) y_min;
	output_y_boundaries[1] = (uint16_t) y_max;

	// 5) Rasterize each scanline
	for (int16_t y = y_min; y <= y_max; ++y) {
		int row = y - y_min;

		// 5a) Vertical offset in Q13.3
		int32_t dy_fp = (((int32_t) y) << 3) - cy_fp;
		uint32_t dy2_q26_6 = (uint32_t) (dy_fp * dy_fp);
		if (dy2_q26_6 > r2_q26_6) {
			// Outside circle
			output_x_coord_list[row * 2] = 0;
			output_x_coord_list[row * 2 + 1] = 0;
			continue;
		}

		// 5b) Compute horizontal half-width in Q13.3
		int32_t dx_fp = isqrt_q26_6_to_q13_3(r2_q26_6 - dy2_q26_6);
		if ((diameter & 1) == 0) {
			dx_fp -= 1 << 2;
		}
		else{
			dx_fp -= 1 << 2;
		}

		// 5c) Left/right endpoints in Q13.3
		int32_t x0_fp = cx_fp - dx_fp;
		int32_t x1_fp = cx_fp + dx_fp;
		if ((diameter & 1) == 0) {
			x1_fp -= 1 << 1;
		}

		// 5d) Convert back to integer pixels:

		int16_t x0 = (int16_t) ((x0_fp + 1) >> 3);
		int16_t diameter_odd_correction = 0;
		if(diameter & 1) diameter_odd_correction = 7;
		int16_t x1 = (int16_t) ((x1_fp + diameter_odd_correction) >> 3);

		// 5e) Clamp to [0 … SCREEN_WIDTH-1]
		if (x0 < 0)
			x0 = 0;
		if (x1 >= (int16_t)NILE_GFX_SCREEN_WIDTH)
			x1 = (int16_t)NILE_GFX_SCREEN_WIDTH - 1;

		output_x_coord_list[row * 2] = (uint16_t) x0;
		output_x_coord_list[row * 2 + 1] = (uint16_t) x1;
	}
}

uint16_t gfx_rasterize_draw_circle(nile_gfx_draw_op *op_list, int16_t c_x, int16_t c_y, int16_t diameter, uint32_t argb8888) {
	if (!op_list)
		return 0;
	if (diameter <= 0)
		return 0;
	uint16_t y_coords[2] = { UINT16_MAX, UINT16_MAX };
	uint16_t x_coords[2 * NILE_GFX_SCREEN_HEIGHT];
	rasterize_circle(c_x, c_y, diameter, y_coords, x_coords);
	if (y_coords[0] == UINT16_MAX)
		return 0;
	uint16_t rect_it = 0;
	for (uint16_t i = y_coords[0]; i <= y_coords[1]; i++) {
		gfx_draw_rectangle(&op_list[rect_it], x_coords[2 * rect_it], i, x_coords[2 * rect_it + 1] + 1, i + 1, argb8888);
		rect_it++;
	}
	return rect_it;
}
