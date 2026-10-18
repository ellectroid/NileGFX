#include "nile_gfx/gfx_api_draw.h"
#include "nile_gfx/gfx_config.h"

static void sevent_segment_coord_generate(uint16_t Sx, uint16_t Sy, uint16_t Lx, uint16_t Ly, uint16_t *coord_list_out);
static void seven_segment_coord_sanitize(uint16_t *coords);
static void seven_segment_coord_filter_digit(uint16_t *coords, uint16_t digit);
void seven_segment_print_digit(uint16_t digit, uint16_t x, uint16_t y, uint16_t height, uint16_t thickness, uint32_t argb, uint16_t queue) {
	uint16_t coords[28];
	uint16_t line_length = (height + thickness) / 2; //2x line length - line thickness = height

	sevent_segment_coord_generate(x, y, line_length, thickness, coords);
	seven_segment_coord_sanitize(coords);
	seven_segment_coord_filter_digit(coords, digit);

	nile_gfx_draw_op op;
	for (int i = 0; i < 7; i++) {
		if (coords[4 * i] != coords[4 * i + 2]) {
			gfx_draw_rectangle(&op, coords[4 * i + 0], coords[4 * i + 1], coords[4 * i + 2], coords[4 * i + 3], argb);
			gfx_draw_op_queue_push(queue, &op);
		}
	}
}

static void sevent_segment_coord_generate(uint16_t Sx, uint16_t Sy, uint16_t Lx, uint16_t Ly, uint16_t *coord_list_out) {
	//Drawing 7-segment at position S(x, y) with line parameters L(x, y)

	typedef struct {
		uint16_t x1;
		uint16_t y1;
		uint16_t x2;
		uint16_t y2;
	} rect;
	rect *out = (rect*) coord_list_out;

	//Top
	out[0].x1 = Sx;
	out[0].y1 = Sy;
	out[0].x2 = Sx + Lx;
	out[0].y2 = Sy + Ly;

	//Top-right
	out[1].x1 = Sx + Lx - Ly;
	out[1].y1 = Sy;
	out[1].x2 = Sx + Lx;
	out[1].y2 = Sy + Lx;

	//Bottom-right
	out[2].x1 = Sx + Lx - Ly;
	out[2].y1 = Sy + Lx - Ly;
	out[2].x2 = Sx + Lx;
	out[2].y2 = Sy + Lx - Ly + Lx;

	//Bottom
	out[3].x1 = Sx;
	out[3].y1 = Sy + Lx - Ly + Lx - Ly;
	out[3].x2 = Sx + Lx;
	out[3].y2 = Sy + Lx - Ly + Lx;

	//Bottom-left
	out[4].x1 = Sx;
	out[4].y1 = Sy + Lx - Ly;
	out[4].x2 = Sx + Ly;
	out[4].y2 = Sy + Lx - Ly + Lx;

	//Top-left
	out[5].x1 = Sx;
	out[5].y1 = Sy;
	out[5].x2 = Sx + Ly;
	out[5].y2 = Sy + Lx;

	//Middle
	out[6].x1 = Sx;
	out[6].y1 = Sy + Lx - Ly;
	out[6].x2 = Sx + Lx;
	out[6].y2 = Sy + Lx;
}

static void seven_segment_coord_sanitize(uint16_t *coords) {
	typedef struct {
		uint16_t x1;
		uint16_t y1;
		uint16_t x2;
		uint16_t y2;
	} rect;
	rect *out = (rect*) coords;
	for (uint32_t i = 0; i < 7; i++) {
		if ((out[i].x1 >= NILE_GFX_SCREEN_WIDTH) || (out[i].y1 >= NILE_GFX_SCREEN_HEIGHT)) {
			out[i].x1 = 0;
			out[i].y1 = 0;
			out[i].x2 = 0;
			out[i].y2 = 0;
			continue;
		}
		if (out[i].x2 > NILE_GFX_SCREEN_WIDTH) {
			out[i].x2 = NILE_GFX_SCREEN_WIDTH;
		}
		if (out[i].y2 > NILE_GFX_SCREEN_HEIGHT) {
			out[i].y2 = NILE_GFX_SCREEN_HEIGHT;
		}
	}
}

static void seven_segment_coord_filter_digit(uint16_t *coords, uint16_t digit) {
	if (digit > 9) {
		for (int i = 0; i < 28; i++) {
			coords[i] = 0;
		}
		return;
	}
	const uint8_t dbm[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F }; //digit bitmask

	for (uint32_t i = 0; i < 7; i++) {
		if (!((dbm[digit] >> i) & 1)) {
			coords[4 * i + 0] = 0;
			coords[4 * i + 1] = 0;
			coords[4 * i + 2] = 0;
			coords[4 * i + 3] = 0;
		}
	}
}
