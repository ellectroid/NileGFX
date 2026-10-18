#include "mcu_hw/gpio.h"
#include "mcu_hw/display/RK043FN02H-CT.h"

void mcu_reset_display_init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;

	//configure PLLSAI for LTDC pixel clock. 25MHz HSE, 1MHz input to PLLSAI. Need PLLSAIN and PLLSAIR.
	RCC->PLLSAICFGR = (192U << RCC_PLLSAICFGR_PLLSAIN_Pos) | (4U << RCC_PLLSAICFGR_PLLSAIR_Pos);
	RCC->DCKCFGR1 = (RCC->DCKCFGR1 & ~(0x03 << RCC_DCKCFGR1_PLLSAIDIVR_Pos)) | (0x01 << RCC_DCKCFGR1_PLLSAIDIVR_Pos); // divide by 4 (1, 2, 4, 8)
	//clock 12MHz
	RCC->CR |= RCC_CR_PLLSAION;
	while (!(RCC->CR & RCC_CR_PLLSAIRDY));  //wait until PLLSAI is stable

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOIEN | RCC_AHB1ENR_GPIOJEN | RCC_AHB1ENR_GPIOKEN;

	/*
	 * 4 function pins
	 * 24  color pins
	 * display enable GPIO pin
	 * */

	/* PI10 - LCD_HSYNC */
	mcu_hw_gpio_alternate_function_set(GPIOI, 10, 14U);
	mcu_hw_gpio_push_pull_set(GPIOI, 10);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOI, 10);
	mcu_hw_gpio_drive_speed_set(GPIOI, 10, 0xFF);

	/* PI9 - LCD_VSYNC */
	mcu_hw_gpio_alternate_function_set(GPIOI, 9, 14U);
	mcu_hw_gpio_push_pull_set(GPIOI, 9);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOI, 9);
	mcu_hw_gpio_drive_speed_set(GPIOI, 9, 0xFF);

	/* PI14 - LCD_CLK */
	mcu_hw_gpio_alternate_function_set(GPIOI, 14, 14U);
	mcu_hw_gpio_push_pull_set(GPIOI, 14);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOI, 14);
	mcu_hw_gpio_drive_speed_set(GPIOI, 14, 0xFF);

	/* PK7 - LCD_DE */
	mcu_hw_gpio_alternate_function_set(GPIOK, 7, 14U);
	mcu_hw_gpio_push_pull_set(GPIOK, 7);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOK, 7);
	mcu_hw_gpio_drive_speed_set(GPIOK, 7, 0xFF);

	/* PI15 - LCD_R0 */
	mcu_hw_gpio_alternate_function_set(GPIOI, 15, 14U);
	mcu_hw_gpio_push_pull_set(GPIOI, 15);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOI, 15);
	mcu_hw_gpio_drive_speed_set(GPIOI, 15, 0xFF);

	/* PJ0 - LCD_R1 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 0, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 0);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 0);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 0, 0xFF);

	/* PJ1 - LCD_R2 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 1, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 1);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 1);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 1, 0xFF);

	/* PJ2 - LCD_R3 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 2, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 2);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 2);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 2, 0xFF);

	/* PJ3 - LCD_R4 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 3, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 3);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 3);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 3, 0xFF);

	/* PJ4 - LCD_R5 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 4, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 4);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 4);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 4, 0xFF);

	/* PJ5 - LCD_R6 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 5, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 5);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 5);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 5, 0xFF);

	/* PJ6 - LCD_R7 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 6, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 6);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 6);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 6, 0xFF);

	/* PJ7 - LCD_G0 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 7, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 7);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 7);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 7, 0xFF);

	/* PJ8 - LCD_G1 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 8, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 8);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 8);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 8, 0xFF);

	/* PJ9 - LCD_G2 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 9, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 9);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 9);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 9, 0xFF);

	/* PJ10 - LCD_G3 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 10, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 10);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 10);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 10, 0xFF);

	/* PJ11 - LCD_G4 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 11, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 11);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 11);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 11, 0xFF);

	/* PK0 - LCD_G5 */
	mcu_hw_gpio_alternate_function_set(GPIOK, 0, 14U);
	mcu_hw_gpio_push_pull_set(GPIOK, 0);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOK, 0);
	mcu_hw_gpio_drive_speed_set(GPIOK, 0, 0xFF);

	/* PK1 - LCD_G6 */
	mcu_hw_gpio_alternate_function_set(GPIOK, 1, 14U);
	mcu_hw_gpio_push_pull_set(GPIOK, 1);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOK, 1);
	mcu_hw_gpio_drive_speed_set(GPIOK, 1, 0xFF);

	/* PK2 - LCD_G7 */
	mcu_hw_gpio_alternate_function_set(GPIOK, 2, 14U);
	mcu_hw_gpio_push_pull_set(GPIOK, 2);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOK, 2);
	mcu_hw_gpio_drive_speed_set(GPIOK, 2, 0xFF);

	/* PE4 - LCD_B0 */
	mcu_hw_gpio_alternate_function_set(GPIOE, 4, 14U);
	mcu_hw_gpio_push_pull_set(GPIOE, 4);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 4);
	mcu_hw_gpio_drive_speed_set(GPIOE, 4, 0xFF);

	/* PJ13 - LCD_B1 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 13, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 13);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 13);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 13, 0xFF);

	/* PJ14 - LCD_B2 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 14, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 14);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 14);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 14, 0xFF);

	/* PJ15 - LCD_B3 */
	mcu_hw_gpio_alternate_function_set(GPIOJ, 15, 14U);
	mcu_hw_gpio_push_pull_set(GPIOJ, 15);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOJ, 15);
	mcu_hw_gpio_drive_speed_set(GPIOJ, 15, 0xFF);

	/* PG12 - LCD_B4 */
	mcu_hw_gpio_alternate_function_set(GPIOG, 12, 14U);
	mcu_hw_gpio_push_pull_set(GPIOG, 12);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOG, 12);
	mcu_hw_gpio_drive_speed_set(GPIOG, 12, 0xFF);

	/* PK4 - LCD_B5 */
	mcu_hw_gpio_alternate_function_set(GPIOK, 4, 14U);
	mcu_hw_gpio_push_pull_set(GPIOK, 4);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOK, 4);
	mcu_hw_gpio_drive_speed_set(GPIOK, 4, 0xFF);

	/* PK5 - LCD_B6 */
	mcu_hw_gpio_alternate_function_set(GPIOK, 5, 14U);
	mcu_hw_gpio_push_pull_set(GPIOK, 5);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOK, 5);
	mcu_hw_gpio_drive_speed_set(GPIOK, 5, 0xFF);

	/* PK6 - LCD_B7 */
	mcu_hw_gpio_alternate_function_set(GPIOK, 6, 14U);
	mcu_hw_gpio_push_pull_set(GPIOK, 6);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOK, 6);
	mcu_hw_gpio_drive_speed_set(GPIOK, 6, 0xFF);

	/*PI12 - LCD_DISP GPIO OUTPUT */
	GPIOI->MODER = ((GPIOI->MODER) & ~(0x03 << GPIO_MODER_MODER12_Pos)) | (0x01 << GPIO_MODER_MODER12_Pos);
	mcu_hw_gpio_push_pull_set(GPIOI, 12);
	mcu_hw_gpio_drive_speed_set(GPIOI, 12, 0x00);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOI, 12);
	GPIOI->ODR |= GPIO_ODR_ODR_12; //output HIGH

	/*
	 *	Pixel clock = DCLK: 8MHz
	 * 	1 pixel clock cycle is 1/8MHz = 125ns
	 * */

	/*
	 *	Synchronization size configuration register
	 *	HSync width: 	min 1DCLK
	 *	VSync height:  	min 12ns
	 * */
	LTDC->SSCR = ((RK043FN02H_HSYNC_CYCLES - 1U) << LTDC_SSCR_HSW_Pos) | ((RK043FN02H_VSYNC_LINES - 1U) << LTDC_SSCR_VSH_Pos);

	/*
	 *	Back porch configuration register (cumulative)
	 * */
	LTDC->BPCR = ((RK043FN02H_HSYNC_CYCLES + RK043FN02H_HBP_CYCLES - 1U) << LTDC_BPCR_AHBP_Pos)
			| ((RK043FN02H_VSYNC_LINES + RK043FN02H_VBP_LINES - 1U) << LTDC_BPCR_AVBP_Pos);

	/*
	 *	Active width configuration register (cumulative)
	 * */
	LTDC->AWCR = ((RK043FN02H_HSYNC_CYCLES + RK043FN02H_HBP_CYCLES + RK043FN02H_ACTIVEWIDTH - 1U) << LTDC_AWCR_AAW_Pos)
			| ((RK043FN02H_VSYNC_LINES + RK043FN02H_VBP_LINES + RK043FN02H_ACTIVEHEIGHT - 1U) << LTDC_AWCR_AAH_Pos);

	/*
	 *	Total width configuration register (configures height too, stupid register name; cumulative)

	 * */
	LTDC->TWCR = ((RK043FN02H_HSYNC_CYCLES + RK043FN02H_HBP_CYCLES + RK043FN02H_ACTIVEWIDTH + RK043FN02H_HFP_CYCLES - 1U) << LTDC_TWCR_TOTALW_Pos)
			| ((RK043FN02H_VSYNC_LINES + RK043FN02H_VBP_LINES + RK043FN02H_ACTIVEHEIGHT + RK043FN02H_VFP_LINES - 1U) << LTDC_TWCR_TOTALH_Pos);

	/*
	 *	Global control register
	 *	HSYNC polarity low
	 *	VSYNC polarity low
	 *	DE	  polarity LOW
	 *	Pixel clock polarity LOW
	 * */
	LTDC->GCR = (0x00 << LTDC_GCR_HSPOL_Pos) | (0x00 << LTDC_GCR_VSPOL_Pos) | (0x00 << LTDC_GCR_DEPOL_Pos) | (0x00 << LTDC_GCR_PCPOL_Pos);

	/* Background color */
	LTDC->BCCR = 0x00000000;

	/* Configure layer 2 (top) properties */

	LTDC_Layer2->WHPCR = ((RK043FN02H_HSYNC_CYCLES + RK043FN02H_HBP_CYCLES + RK043FN02H_ACTIVEWIDTH - 1U)  //-1 necessary
	<< LTDC_LxWHPCR_WHSPPOS_Pos) | ((RK043FN02H_HSYNC_CYCLES + RK043FN02H_HBP_CYCLES) << LTDC_LxWHPCR_WHSTPOS_Pos);

	LTDC_Layer2->WVPCR = ((RK043FN02H_VSYNC_LINES + RK043FN02H_VBP_LINES + RK043FN02H_ACTIVEHEIGHT) << LTDC_LxWVPCR_WVSPPOS_Pos)
			| ((RK043FN02H_VSYNC_LINES + RK043FN02H_VBP_LINES) << LTDC_LxWVPCR_WVSTPOS_Pos);

	LTDC_Layer2->PFCR = 0x00;  //ARGB8888

	LTDC_Layer2->CFBLR = (((RK043FN02H_ACTIVEWIDTH) * 4) << LTDC_LxCFBLR_CFBP_Pos) | ((RK043FN02H_ACTIVEWIDTH * 4 + 3) << LTDC_LxCFBLR_CFBLL_Pos); //color frame buffer pitch/line length

	LTDC_Layer2->CFBLNR = ((RK043FN02H_ACTIVEHEIGHT) << LTDC_LxCFBLNR_CFBLNBR_Pos);  //no of lines

	/* Configure layer 1 (lower) layer properties */

	LTDC_Layer1->WHPCR = ((RK043FN02H_HSYNC_CYCLES + RK043FN02H_HBP_CYCLES + RK043FN02H_ACTIVEWIDTH - 1U)  //-1 necessary
	<< LTDC_LxWHPCR_WHSPPOS_Pos) | ((RK043FN02H_HSYNC_CYCLES + RK043FN02H_HBP_CYCLES) << LTDC_LxWHPCR_WHSTPOS_Pos);

	LTDC_Layer1->WVPCR = ((RK043FN02H_VSYNC_LINES + RK043FN02H_VBP_LINES + RK043FN02H_ACTIVEHEIGHT) << LTDC_LxWVPCR_WVSPPOS_Pos)
			| ((RK043FN02H_VSYNC_LINES + RK043FN02H_VBP_LINES) << LTDC_LxWVPCR_WVSTPOS_Pos);

	LTDC_Layer1->PFCR = 0x00;  //ARGB8888

	LTDC_Layer1->CFBLR = (((RK043FN02H_ACTIVEWIDTH) * 4) << LTDC_LxCFBLR_CFBP_Pos) | ((RK043FN02H_ACTIVEWIDTH * 4 + 3) << LTDC_LxCFBLR_CFBLL_Pos); //color frame buffer pitch/line length

	LTDC_Layer1->CFBLNR = ((RK043FN02H_ACTIVEHEIGHT) << LTDC_LxCFBLNR_CFBLNBR_Pos);  //no of lines

	LTDC->SRCR = LTDC_SRCR_IMR;	//force reload shadow registers immediately

	LTDC->GCR |= (0x01 << LTDC_GCR_LTDCEN_Pos); //enable LTDC and pray that it works

	/* Final config and start */

	for (int i = 0; i < 480 * 272 * 2; i++) {
		*(uint32_t*) (0xC0000000 + i * 4) = 0xFFFFFFFF;
	}

	//	for (int i = 0; i < 480 * 272 / 2; i++) {
	//		*(uint32_t*) (0xC0000000 + i * 4) = 0x8800FF00;
	//	}
	//	for (int i = 480 * 272 / 2; i < 480 * 272; i++) {
	//		*(uint32_t*) (0xC0000000 + i * 4) = 0x88FF0000;
	//	}
	//	for (int i = 480 * 272; i < 480 * 272 + 480 * 272 / 4; i++) {
	//		*(uint32_t*) (0xC0000000 + i * 4) = 0x880000FF;
	//	}
	//	for (int i = 480 * 272 + 480 * 272 / 2; i < 480 * 272 + 480 * 272 / 4 * 3; i++) {
	//		*(uint32_t*) (0xC0000000 + i * 4) = 0x880000FF;
	//	}

	LTDC_Layer2->CFBAR = 0xC0000000;
	//LTDC_Layer1->CFBAR = 0xC0000000 + 480 * 272 * 4;

	LTDC_Layer1->BFCR = (0x06 << LTDC_LxBFCR_BF1_Pos) | (0x07 << LTDC_LxBFCR_BF2_Pos); // BF1: pixel alpha, BF2: constant alpha
	//LTDC_Layer2->BFCR = (0x06 << LTDC_LxBFCR_BF1_Pos) | (0x07 << LTDC_LxBFCR_BF2_Pos);

	LTDC_Layer1->CACR = 255;
	//LTDC_Layer2->CACR = 255;

	LTDC_Layer2->CR |= LTDC_LxCR_LEN;
	//LTDC_Layer1->CR |= LTDC_LxCR_LEN;

	LTDC->LIPCR = 0; //line interrupt at line 0

	LTDC->SRCR = LTDC_SRCR_IMR;

}
