#if !defined(__SOFT_FP__) && defined(__ARM_FP) && 0
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "stm32f746xx.h"
#include "nile_gfx/gfx.h"
#include "nile_gfx/gfx_screen.h"
#include "nile_gfx/gfx_api_pipeline.h"
#include "nile_gfx/gfx_api_draw.h"
#include "nile_gfx/gfx_seven_segment.h"

enum {
	SCREEN_SPLASH_SCREEN = 0,
	SCREEN_GAME,
	SCREEN_COUNT,
};

enum {
	GFX_CONTROL_PTID_REQUEST_STATUS = 0,
	GFX_CONTROL_PTID_GFX_ENABLE,
	GFX_CONTROL_PTID_GFX_DISABLE,
	GFX_CONTROL_PTID_SET_SCREEN,
	GFX_CONTROL_PTID_STATUS_REPLY,

	GFX_SCREEN_COUNT = SCREEN_COUNT,
};

void gfx_screen0_init(nile_gfx_screen *screen0, nile_gfx_draw_op *qspace_start, nile_gfx_draw_op **qspace_last) {
	screen0->CR1 |= NILE_GFX_CR1_DRQN_MAX << NILE_GFX_CR1_DRQN_POS; //number of queues in use
	screen0->CR1 |= NILE_GFX_CR1_QEN0 | NILE_GFX_CR1_QEN1 | NILE_GFX_CR1_QEN2 | NILE_GFX_CR1_QEN3; //queue enable
	screen0->CR1 |= NILE_GFX_CR1_CEN; //cache enable
	screen0->CR1 |= NILE_GFX_CR1_QTCEN0; //target cache enable
	screen0->RSEAR = (void*) screen0;

	const uint32_t S0_Q0_CAP = 7;
	const uint32_t S0_Q1_CAP = 1;
	const uint32_t S0_Q2_CAP = 16;
	const uint32_t S0_Q3_CAP = 56;

	screen0->DQ[NILE_GFX_Q0].capacity = S0_Q0_CAP;
	screen0->DQ[NILE_GFX_Q0].qspace_first = qspace_start;
	screen0->DQ[NILE_GFX_Q0].qspace_last = screen0->DQ[NILE_GFX_Q0].qspace_first + S0_Q0_CAP - 1;

	screen0->DQ[NILE_GFX_Q1].capacity = S0_Q1_CAP;
	screen0->DQ[NILE_GFX_Q1].qspace_first = screen0->DQ[NILE_GFX_Q0].qspace_last + 1;
	screen0->DQ[NILE_GFX_Q1].qspace_last = screen0->DQ[NILE_GFX_Q1].qspace_first + S0_Q1_CAP - 1;

	screen0->DQ[NILE_GFX_Q2].capacity = S0_Q2_CAP;
	screen0->DQ[NILE_GFX_Q2].qspace_first = screen0->DQ[NILE_GFX_Q1].qspace_last + 1;
	screen0->DQ[NILE_GFX_Q2].qspace_last = screen0->DQ[NILE_GFX_Q2].qspace_first + S0_Q2_CAP - 1;

	screen0->DQ[NILE_GFX_Q3].capacity = S0_Q3_CAP;
	screen0->DQ[NILE_GFX_Q3].qspace_first = screen0->DQ[NILE_GFX_Q2].qspace_last + 1;
	screen0->DQ[NILE_GFX_Q3].qspace_last = screen0->DQ[NILE_GFX_Q3].qspace_first + S0_Q3_CAP - 1;

	*qspace_last = screen0->DQ[NILE_GFX_Q3].qspace_last;

	gfx_screen_shadow_write(screen0);
	gfx_screen_shadow_blanking_reload();
	while (gfx_screen_reload_pending());

	nile_gfx_draw_op op = { };

	//Configure operations
	//Q0 - base to cache
	gfx_draw_rectangle(&op, 0, 0, NILE_GFX_SCREEN_WIDTH, NILE_GFX_SCREEN_HEIGHT, 0x00000000);
	gfx_draw_op_queue_push(NILE_GFX_Q0, &op);
	gfx_draw_rectangle(&op, 20, 20, 460, 252, 0xFFFFFF00);
	gfx_draw_op_queue_push(NILE_GFX_Q0, &op);

	//Q1 - base cache to framebuffer
	gfx_draw_texture(&op, 0, 0, NILE_GFX_SCREEN_WIDTH, NILE_GFX_SCREEN_HEIGHT, (void*) NILE_GFX_FB2_BASE_ADDR);
	gfx_draw_op_queue_push(NILE_GFX_Q1, &op);

	//Q2 - overlay
	gfx_draw_rectangle(&op, 40, 40, 440, 232, 0xFF0000FF);
	gfx_draw_op_queue_push(NILE_GFX_Q3, &op);

	//Force base to cache redraw
	gfx_draw_cache_invalidate();

	seven_segment_print_digit(0, NILE_GFX_SCREEN_WIDTH - 20, NILE_GFX_SCREEN_HEIGHT - 20, 12, 2, 0xFFFF0000, NILE_GFX_Q3);
}

//Returning character draw operation so we can move it from outside the function
nile_gfx_draw_op* gfx_screen1_init(nile_gfx_screen *screen1, nile_gfx_draw_op *qspace_start, nile_gfx_draw_op **qspace_last) {
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;

	screen1->CR1 |= NILE_GFX_CR1_DRQN_MAX << NILE_GFX_CR1_DRQN_POS; //number of queues in use
	screen1->CR1 |= NILE_GFX_CR1_QEN0 | NILE_GFX_CR1_QEN1 | NILE_GFX_CR1_QEN2 | NILE_GFX_CR1_QEN3; //queue enable
	screen1->CR1 |= NILE_GFX_CR1_CEN; //cache enable
	screen1->CR1 |= NILE_GFX_CR1_QTCEN0; //target cache enable (draw to cache)
	//screen1->CR1 |= NILE_GFX_CR1_QDRMEN2; //destructive read mode (empty the queue after drawing)
	screen1->RSEAR = (void*) screen1;

	const uint32_t S1_Q0_CAP = 16; //in this config, draw to cache ops
	const uint32_t S1_Q1_CAP = 4; //cache to screen
	const uint32_t S1_Q2_CAP = 64; //main drawing
	const uint32_t S1_Q3_CAP = 4; //optional additional ops

	screen1->DQ[NILE_GFX_Q0].capacity = S1_Q0_CAP;
	screen1->DQ[NILE_GFX_Q0].qspace_first = qspace_start;
	screen1->DQ[NILE_GFX_Q0].qspace_last = screen1->DQ[NILE_GFX_Q0].qspace_first + S1_Q0_CAP - 1;

	screen1->DQ[NILE_GFX_Q1].capacity = S1_Q1_CAP;
	screen1->DQ[NILE_GFX_Q1].qspace_first = screen1->DQ[NILE_GFX_Q0].qspace_last + 1;
	screen1->DQ[NILE_GFX_Q1].qspace_last = screen1->DQ[NILE_GFX_Q1].qspace_first + S1_Q1_CAP - 1;

	screen1->DQ[NILE_GFX_Q2].capacity = S1_Q2_CAP;
	screen1->DQ[NILE_GFX_Q2].qspace_first = screen1->DQ[NILE_GFX_Q1].qspace_last + 1;
	screen1->DQ[NILE_GFX_Q2].qspace_last = screen1->DQ[NILE_GFX_Q2].qspace_first + S1_Q2_CAP - 1;

	screen1->DQ[NILE_GFX_Q3].capacity = S1_Q3_CAP;
	screen1->DQ[NILE_GFX_Q3].qspace_first = screen1->DQ[NILE_GFX_Q2].qspace_last + 1;
	screen1->DQ[NILE_GFX_Q3].qspace_last = screen1->DQ[NILE_GFX_Q3].qspace_first + S1_Q3_CAP - 1;

	*qspace_last = screen1->DQ[NILE_GFX_Q3].qspace_last;

	gfx_screen_shadow_write(screen1);
	gfx_screen_shadow_blanking_reload();
	while (gfx_screen_reload_pending());

	nile_gfx_draw_op op = { };
	//Cache FB clear screen op (to prevent image retention from another screen)
	gfx_draw_rectangle(&op, 0, 0, NILE_GFX_SCREEN_WIDTH, NILE_GFX_SCREEN_HEIGHT, 0x00000000);
	gfx_draw_op_queue_push(NILE_GFX_Q0, &op);

	//Cache FB to screen FB
	gfx_draw_texture(&op, 0, 0, NILE_GFX_SCREEN_WIDTH, NILE_GFX_SCREEN_HEIGHT, GFX->FB_CACHE);
	gfx_draw_op_queue_push(NILE_GFX_Q1, &op);

	//For destructive mode (drawing)
	//gfx_draw_texture_blend(&op, 0, 0, NILE_GFX_SCREEN_WIDTH, NILE_GFX_SCREEN_HEIGHT, GFX->FB_DRAW);
	//gfx_draw_op_queue_push(NILE_GFX_Q1, &op);

	seven_segment_print_digit(1, NILE_GFX_SCREEN_WIDTH - 20, NILE_GFX_SCREEN_HEIGHT - 20, 12, 2, 0xFFFF0000, NILE_GFX_Q2);

	gfx_draw_texture(&op, 0, 0, 160, 80, (void*) (0x90000000 + 8192)); //background to cache
	gfx_draw_op_queue_push(NILE_GFX_Q0, &op);

	gfx_draw_texture_blend(&op, 0, 48, 12, 64, (void*) 0x90000000); //foreground redraw every time
	nile_gfx_draw_op *return_op = gfx_draw_op_queue_push(NILE_GFX_Q2, &op);

	gfx_draw_cache_invalidate(); //force redraw to cache from cache queue

//	gfx_draw_rectangle(&op, 0, 0, 20, 20, 0xFF000000);
//	gfx_draw_op_queue_push(NILE_GFX_Q3, &op);
//
//	gfx_draw_rectangle(&op, 20, 20, 40, 40, 0xFFFF0000);
//	gfx_draw_op_queue_push(NILE_GFX_Q3, &op);
//
//	gfx_draw_rectangle(&op, 40, 40, 60, 60, 0xFF00FF00);
//	gfx_draw_op_queue_push(NILE_GFX_Q3, &op);
//
//	gfx_draw_rectangle(&op, 60, 60, 80, 80, 0xFF0000FF);
//	gfx_draw_op_queue_push(NILE_GFX_Q3, &op);
	return return_op;

}

static void gfx_set_screen(nile_gfx_screen *screen, uint32_t new_screen_index, uint32_t screen_cnt, uint32_t *current_screen_ptr) {
	if (new_screen_index >= screen_cnt)
		return;
	gfx_screen_shadow_write(&screen[new_screen_index]);
	gfx_screen_shadow_blanking_reload();
	*current_screen_ptr = new_screen_index;
}

static void gfx_enable(void) {
	gfx_pipeline_global_render_enable_set(1);
}

static void gfx_disable(void) {
	gfx_pipeline_global_render_enable_set(0);
	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;
	for (uint32_t p = 0; p < NILE_GFX_SCREEN_WIDTH * NILE_GFX_SCREEN_HEIGHT; p++) {
		((uint32_t*) GFX->FB[0])[p] = 0;
	}
	for (uint32_t p = 0; p < NILE_GFX_SCREEN_WIDTH * NILE_GFX_SCREEN_HEIGHT; p++) {
		((uint32_t*) GFX->FB[1])[p] = 0;
	}
}

void mcu_reset_mcu_clock_set_max(void) {
	/* Activate High Speed External Oscillator */
	RCC->CR |= (0x01 << 16); //Enable
	while (!(RCC->CR & (0x01 << 17))); //Wait until HSE is ready

	/* Activate PWR peripheral */
	RCC->APB1ENR |= (0x01 << 28);
	while (!(RCC->APB1ENR & (0x01 << 28))); //Waste cycles

	/* Activate internal regulator, overdrive and switching overdrive */
	PWR->CR1 = (PWR->CR1 & ~(0x03 << 14)) | (0x03 << 14); //Regulator voltage scale 1 mode
	PWR->CR1 |= (0x01 << 16); //Overdrive enable
	while (!(PWR->CSR1 & (0x01 << 16))); //Wait until overdrive enabled
	PWR->CR1 |= (0x01 << 17); //Overdrive switching enable
	while (!(PWR->CSR1 & (0x01 << 17))); //Wait until overdrive switching enabled

	/* Configure PLL */
	RCC->PLLCFGR |= (0x01 << 22); //PLL Source: HSE
	RCC->PLLCFGR = (RCC->PLLCFGR & ~(0x1F << 0)) | (25U << 0); //PLLM = 25
	RCC->PLLCFGR = (RCC->PLLCFGR & ~(0x1FF << 6)) | (432U << 6); //PLLN = 432
	RCC->PLLCFGR = (RCC->PLLCFGR & ~(0x03 << 16)) | (0U << 16); //PLLP = 2
	RCC->PLLCFGR = (RCC->PLLCFGR & ~(0x0F << 24)) | (9U << 24); //PLLQ = 9 (48MHz to PLL48CLK)
	RCC->CR |= (0x01 << 24); //PLL enable
	while (!(RCC->CR & (0x01 << 25))); //Wait until PLL ready

	/* Configure Flash wait states, ART accelerator, prefetch */
	FLASH->ACR |= (0x07 << 0) | (0x01 << 8) | (0x01 << 9); //7 wait states, enable prefetch, enable ART accelerator

	/* Configure AHB/APB prescalers */
	RCC->CFGR = (RCC->CFGR & ~(0x0F << 4)) | (0x00 << 4); //AHB Prescaler = 1
	RCC->CFGR = (RCC->CFGR & ~(0x07 << 10)) | (0x05 << 10); //APB1 Prescaler = 4
	RCC->CFGR = (RCC->CFGR & ~(0x07 << 13)) | (0x04 << 13); //APB Prescaler = 2

	/* Switch to PLL as system clock */
	RCC->CFGR = (RCC->CFGR & (0x03 << 0)) | (0x02 << 0);
	while (!(RCC->CFGR & (0x03 << 2))); //Wait until system clock switched
}

int main(void) {
	mcu_reset_mcu_clock_set_max();

	gfx_hw_init(); //hw-specific initialization for graphics
	gfx_init(); //graphics kernel data structure init
	gfx_start(); //graphics kernel start
	gfx_disable();

	nile_gfx *GFX = (nile_gfx*) NILE_GFX_CONTROL_BASE_ADDR;

	uint32_t blanking_wake_up_flag = 0;
	uint32_t active_wake_up_flag = 0;

	GFX->BLSIG = &blanking_wake_up_flag; //can be used by OS for wake up
	GFX->ACTSIG = &active_wake_up_flag; //can be used by OS for wake up
	gfx_pipeline_global_render_enable_set(0);

	nile_gfx_draw_op *qspace_start = (nile_gfx_draw_op*) NILE_GFX_QSPACE_BASE_ADDR;
	nile_gfx_draw_op *qspace_last_used;

	nile_gfx_screen screen[SCREEN_COUNT] = { };

	gfx_screen0_init(&screen[SCREEN_SPLASH_SCREEN], qspace_start, &qspace_last_used);
	nile_gfx_draw_op *character = gfx_screen1_init(&screen[SCREEN_GAME], qspace_last_used + 1, &qspace_last_used);

	uint32_t current_screen = SCREEN_SPLASH_SCREEN;
	uint32_t new_screen_index = SCREEN_SPLASH_SCREEN;
	gfx_screen_shadow_write(&screen[SCREEN_SPLASH_SCREEN]);
	gfx_screen_shadow_blanking_reload();

	gfx_enable();


	//character moves back and forth
	int16_t character_move_speed = 2;
	int16_t character_move_total = 0;

	new_screen_index = SCREEN_SPLASH_SCREEN;
	gfx_set_screen(screen, new_screen_index, SCREEN_COUNT, &current_screen);
	for (uint32_t i = 0; i < 216000000UL / 16; i++);
	new_screen_index = SCREEN_GAME;
	gfx_set_screen(screen, new_screen_index, SCREEN_COUNT, &current_screen);
	for (;;) {
		for (uint32_t i = 0; i < 216000000UL / 16; i++);
		for (uint32_t i = 0; i < 40; i++) {
			while (*GFX->ACTSIG); //wait until blanking
			character->P1X += character_move_speed;
			character->P2X += character_move_speed;
			character_move_total += character_move_speed;
			if((character_move_total >= 160) || (character_move_total <= 0)){
				character_move_speed = -character_move_speed;
			}
			while (*GFX->BLSIG); //wait until active
		}

	}
}
