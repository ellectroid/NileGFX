#include "stm32f7xx.h"
#include "nile_gfx/gfx_stdtypes.h"
#include "mcu_hw/gpio.h"

void mcu_reset_sdram_init(void) {
	/* ---------- Enable Flexible Memory Controller & GPIO Clock ---------- */
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN
			| RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN
			| RCC_AHB1ENR_GPIOHEN;

	/*
	 * FMC SDRAM Bank 1
	 *
	 * FMC D0	-	PD14
	 * FMC D1	-	PD15
	 * FMC D2	-	PD0
	 * FMC D3	-	PD1
	 * FMC D4	-	PE7
	 * FMC D5	-	PE8
	 * FMC D6	-	PE9
	 * FMC D7	-	PE10
	 * FMC D8	-	PE11
	 * FMC D9	-	PE12
	 * FMC D10	-	PE13
	 * FMC D11	-	PE14
	 * FMC D12	-	PE15
	 * FMC D13	-	PD8
	 * FMC D14	-	PD9
	 * FMC D15	-	PD10
	 *
	 * FMC A0	-	PF0
	 * FMC A1	-	PF1
	 * FMC A2	-	PF2
	 * FMC A3	-	PF3
	 * FMC A4	-	PF4
	 * FMC A5	-	PF5
	 * FMC A6	-	PF12
	 * FMC A7	-	PF13
	 * FMC A8	-	PF14
	 * FMC A9	-	PF15
	 * FMC A10	-	PG0
	 * FMC A11	-	PG1
	 *
	 * FMC SDNRAS-	PF11
	 * FMC SDNCAS-	PG15
	 *
	 * FMC SDCLK-	PG8
	 * FMC SDCKE-	PC3
	 * FMC SDNWE-	PH5
	 * FMC SDNE0-	PH3
	 *
	 * FMC NBL0	-	PE0
	 * FMC BNBL1-	PE1
	 *
	 * FMC BA0	-	PG4
	 * FMC BA1	-	PG5
	 * */

	mcu_hw_gpio_alternate_function_set(GPIOD, 14, 12U);
	mcu_hw_gpio_push_pull_set(GPIOD, 14);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 14);
	mcu_hw_gpio_drive_speed_set(GPIOD, 14, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 15, 12U);
	mcu_hw_gpio_push_pull_set(GPIOD, 15);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 15);
	mcu_hw_gpio_drive_speed_set(GPIOD, 15, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 0, 12U);
	mcu_hw_gpio_push_pull_set(GPIOD, 0);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 0);
	mcu_hw_gpio_drive_speed_set(GPIOD, 0, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 1, 12U);
	mcu_hw_gpio_push_pull_set(GPIOD, 1);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 1);
	mcu_hw_gpio_drive_speed_set(GPIOD, 1, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 7, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 7);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 7);
	mcu_hw_gpio_drive_speed_set(GPIOE, 7, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 8, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 8);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 8);
	mcu_hw_gpio_drive_speed_set(GPIOE, 8, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 9, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 9);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 9);
	mcu_hw_gpio_drive_speed_set(GPIOE, 9, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 10, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 10);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 10);
	mcu_hw_gpio_drive_speed_set(GPIOE, 10, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 11, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 11);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 11);
	mcu_hw_gpio_drive_speed_set(GPIOE, 11, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 12, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 12);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 12);
	mcu_hw_gpio_drive_speed_set(GPIOE, 12, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 13, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 13);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 13);
	mcu_hw_gpio_drive_speed_set(GPIOE, 13, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 14, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 14);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 14);
	mcu_hw_gpio_drive_speed_set(GPIOE, 14, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 15, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 15);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 15);
	mcu_hw_gpio_drive_speed_set(GPIOE, 15, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 8, 12U);
	mcu_hw_gpio_push_pull_set(GPIOD, 8);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 8);
	mcu_hw_gpio_drive_speed_set(GPIOD, 8, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 9, 12U);
	mcu_hw_gpio_push_pull_set(GPIOD, 9);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 9);
	mcu_hw_gpio_drive_speed_set(GPIOD, 9, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 10, 12U);
	mcu_hw_gpio_push_pull_set(GPIOD, 10);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 10);
	mcu_hw_gpio_drive_speed_set(GPIOD, 10, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 0, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 0);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 0);
	mcu_hw_gpio_drive_speed_set(GPIOF, 0, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 1, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 1);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 1);
	mcu_hw_gpio_drive_speed_set(GPIOF, 1, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 2, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 2);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 2);
	mcu_hw_gpio_drive_speed_set(GPIOF, 2, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 3, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 3);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 3);
	mcu_hw_gpio_drive_speed_set(GPIOF, 3, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 4, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 4);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 4);
	mcu_hw_gpio_drive_speed_set(GPIOF, 4, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 5, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 5);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 5);
	mcu_hw_gpio_drive_speed_set(GPIOF, 5, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 12, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 12);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 12);
	mcu_hw_gpio_drive_speed_set(GPIOF, 12, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 13, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 13);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 13);
	mcu_hw_gpio_drive_speed_set(GPIOF, 13, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 14, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 14);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 14);
	mcu_hw_gpio_drive_speed_set(GPIOF, 14, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 15, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 15);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 15);
	mcu_hw_gpio_drive_speed_set(GPIOF, 15, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOG, 0, 12U);
	mcu_hw_gpio_push_pull_set(GPIOG, 0);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOG, 0);
	mcu_hw_gpio_drive_speed_set(GPIOG, 0, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOG, 1, 12U);
	mcu_hw_gpio_push_pull_set(GPIOG, 1);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOG, 1);
	mcu_hw_gpio_drive_speed_set(GPIOG, 1, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOF, 11, 12U);
	mcu_hw_gpio_push_pull_set(GPIOF, 11);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOF, 11);
	mcu_hw_gpio_drive_speed_set(GPIOF, 11, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOG, 15, 12U);
	mcu_hw_gpio_push_pull_set(GPIOG, 15);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOG, 15);
	mcu_hw_gpio_drive_speed_set(GPIOG, 15, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOG, 8, 12U);
	mcu_hw_gpio_push_pull_set(GPIOG, 8);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOG, 8);
	mcu_hw_gpio_drive_speed_set(GPIOG, 8, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOC, 3, 12U);
	mcu_hw_gpio_push_pull_set(GPIOC, 3);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOC, 3);
	mcu_hw_gpio_drive_speed_set(GPIOC, 3, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOH, 5, 12U);
	mcu_hw_gpio_push_pull_set(GPIOH, 5);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOH, 5);
	mcu_hw_gpio_drive_speed_set(GPIOH, 5, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOH, 3, 12U);
	mcu_hw_gpio_push_pull_set(GPIOH, 3);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOH, 3);
	mcu_hw_gpio_drive_speed_set(GPIOH, 3, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 0, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 0);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 0);
	mcu_hw_gpio_drive_speed_set(GPIOE, 0, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 1, 12U);
	mcu_hw_gpio_push_pull_set(GPIOE, 1);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 1);
	mcu_hw_gpio_drive_speed_set(GPIOE, 1, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOG, 4, 12U);
	mcu_hw_gpio_push_pull_set(GPIOG, 4);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOG, 4);
	mcu_hw_gpio_drive_speed_set(GPIOG, 4, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOG, 5, 12U);
	mcu_hw_gpio_push_pull_set(GPIOG, 5);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOG, 5);
	mcu_hw_gpio_drive_speed_set(GPIOG, 5, 0xFF);

	/*
	 * Set up SDRAM Bank 1 control register
	 *
	 * Number of column address bits: 	8
	 * Number of row address bits: 		12
	 * Memory data bus width: 			16
	 * Number of internal banks:		4
	 * CAS latency:						2 Cycles
	 * Write protection:				Write access allowed
	 * SDRAM Clock Config:				Tsdclk = 2xThclk (108MHz @ 216MHz CPU clock)
	 * Burst read:						Single read requests NOT managed as bursts
	 * Read Pipe:						0 HCLK cycle delay
	 *
	 * */
	FMC_Bank5_6->SDCR[0] = (0x00 << FMC_SDCR1_NC_Pos)
			| (0x01 << FMC_SDCR1_NR_Pos) | (0x01 << FMC_SDCR1_MWID_Pos)
			| (0x01 << FMC_SDCR1_NB_Pos) | (0x02 << FMC_SDCR1_CAS_Pos)
			| (0x00 << FMC_SDCR1_WP_Pos) | (0x02 << FMC_SDCR1_SDCLK_Pos)
			| (0x00 << FMC_SDCR1_RBURST_Pos) | (0x00 << FMC_SDCR1_RPIPE_Pos);

	/*
	 * Set up device timing
	 *
	 * Set Load Mode Register to Active Tmrd:	12ns (in no. of cycles, rounded up)
	 * Exit Self-refresh delay Txsr:			70ns (in no. of cycles, rounded up)
	 * Self-refresh time Tras:					42ns (in no. of cycles, rounded up)
	 * Row cycle delay Trc:						6 Cycles for CAS latency 2, 10 cycles for CAS latency 3
	 * Recovery delay Twr:						Twr>=Tras-Trcd and Twr>=TRC-Trcd-Trp in cycles
	 * Row precharge delay Trp:					18ns (in no. of cycles, rounded up)
	 * Row to column delay Trcd:				18ns (in no. of cycles, rounded up)
	 *
	 * */
	FMC_Bank5_6->SDTR[0] = ((2U - 1U) << FMC_SDTR1_TMRD_Pos)		//2
	| ((8U - 1U) << FMC_SDTR1_TXSR_Pos)		//8
			| ((5U - 1U) << FMC_SDTR1_TRAS_Pos)		//5
			| ((6U - 1U) << FMC_SDTR1_TRC_Pos)		//6
			| ((3U - 1U) << FMC_SDTR1_TWR_Pos)		//3
			| ((2U - 1U) << FMC_SDTR1_TRP_Pos)		//2
			| ((2U - 1U) << FMC_SDTR1_TRCD_Pos);	//2

	/*
	 * Initialize SDRAM IC via Command Mode register
	 * Writing to MODE field issues commands
	 * */
	FMC_Bank5_6->SDCMR = (0x01 << FMC_SDCMR_CTB1_Pos)
			| ((1U - 1U) << FMC_SDCMR_NRFS_Pos)  //number of auto-refreshes
			| (0x01 << FMC_SDCMR_MODE_Pos); //start delivering clock to SDRAM bank 1 (SDCKE->HIGH)
	//system_msdelay(1U);  //wait at least 100us after power up
	for (uint32_t counter = 0x00; counter < 70000; counter++);

	FMC_Bank5_6->SDCMR = (0x01 << FMC_SDCMR_CTB1_Pos)  //set bank 1
	| ((1U - 1U) << FMC_SDCMR_NRFS_Pos)  //number of auto-refreshes
			| (0x02 << FMC_SDCMR_MODE_Pos);  //issue precharge all

	FMC_Bank5_6->SDCMR = (0x01 << FMC_SDCMR_CTB1_Pos)  //set bank 1
	| ((8U - 1U) << FMC_SDCMR_NRFS_Pos)  //number of auto-refreshes
			| (0x03 << FMC_SDCMR_MODE_Pos);  //issue auto-refreshes

	/*
	 * Create mode register value and load it into SDRAM mode register
	 * Burst length:		1 (1 2 4 8 Page)
	 * Burst type: 			Sequential
	 * Latency mode: 		CAS latency 2
	 * Operating mode:		Standard operation
	 * Write burst mode:	Programmed burst length
	 * Reserved:			Always 0
	 *
	 * 	 * */
	uint32_t SDRAM_modeRegisterContent = (0x00 << 0x00) //[0:3]			Burst length
	| (0x00 << 0x03)  //[3:1]		Burst type
			| (0x02 << 0x04)  //[4:3]		Latency mode
			| (0x00 << 0x07)  //[7:2]		Operating mode
			| (0x00 << 0x09)  //[9:1]		Write burst mode
			| (0x00 << 0x0A);  //[10:4]		Reserved

	FMC_Bank5_6->SDCMR = (0x01 << FMC_SDCMR_CTB1_Pos)  //set bank 1
	| (SDRAM_modeRegisterContent << FMC_SDCMR_MRD_Pos) //SDRAM mode register content
			| ((1U - 1U) << FMC_SDCMR_NRFS_Pos)  //number of auto-refreshes
			| (0x04 << FMC_SDCMR_MODE_Pos); //issue set mode register in the SDRAM;

	//system_msdelay(1U);
	for (uint32_t counter = 0x00; counter < 70000; counter++);
	/*
	 * SDRAM Refresh Timer
	 * Refresh period: 		64ms
	 * Number of rows: 		4096
	 * Refresh rate: 		64ms/4096rows = 15.62us
	 * Count value:			15.62*108MHz - 20 = 1686.96 - 20 = ~1666
	 *
	 * RES Interrupt disable
	 * Clear refresh error flag
	 * */
	uint32_t SDRAM_refreshCount = 1666U;  //1666U = 0x0682
	FMC_Bank5_6->SDRTR = (0x00 << FMC_SDRTR_REIE_Pos) //RES interrupt enable OFF
	| (SDRAM_refreshCount << FMC_SDRTR_COUNT_Pos) //load refresh count
			| (0x01 << FMC_SDRTR_CRE_Pos); //clear refresh error flag
	//system_msdelay(1U);
	for (uint32_t counter = 0x00; counter < 70000; counter++);

	*(uint32_t*) (0xC0000000) = 0xBEEFCAFE;

}
