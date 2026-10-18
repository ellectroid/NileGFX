#include "mcu_hw/gpio.h"
#include "mcu_hw/qspi.h"
#include "mcu_hw/storage/MT25QL128ABA1EW9.h"

#define MT25QL128ABA1EW9_QIO_DTR_DUMMYCYC_CNT  8

void quadspi_hw_ic_init(void);
void quadspi_memory_mapped_mode_enable(void);
void mcu_reset_storage_qspi_init(void) {
	/* ------------ QSPI Clock Init ------------------- */

	RCC->AHB3ENR |= (0x01 << 1); //QUADSPI Clock
	RCC->AHB1ENR |= (0x01 << 1) | (0x01 << 3) | (0x01 << 4); //GPIOB, GPIOD, GPIOE Clock
	while (!(RCC->AHB3ENR & (0x01 << 1))); //waste cycles

	/* ------------ QSPI GPIO Init -------------------- */
	/*
	 QuadSPI:
	 NCS -  PB6
	 CLK -  PB2
	 D0  - PD11
	 D1  - PD12
	 D2  -  PE2
	 D3  - PD13
	 */

	mcu_hw_gpio_alternate_function_set(GPIOB, 2, 9U);
	mcu_hw_gpio_push_pull_set(GPIOB, 2);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOB, 2);
	mcu_hw_gpio_drive_speed_set(GPIOB, 2, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOB, 6, 10U);
	mcu_hw_gpio_push_pull_set(GPIOB, 6);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOB, 6);
	mcu_hw_gpio_drive_speed_set(GPIOB, 6, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 11, 9U);
	mcu_hw_gpio_push_pull_set(GPIOD, 11);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 11);
	mcu_hw_gpio_drive_speed_set(GPIOD, 11, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 12, 9U);
	mcu_hw_gpio_push_pull_set(GPIOD, 12);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 12);
	mcu_hw_gpio_drive_speed_set(GPIOD, 12, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOD, 13, 9U);
	mcu_hw_gpio_push_pull_set(GPIOD, 13);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOD, 13);
	mcu_hw_gpio_drive_speed_set(GPIOD, 13, 0xFF);

	mcu_hw_gpio_alternate_function_set(GPIOE, 2, 9U);
	mcu_hw_gpio_push_pull_set(GPIOE, 2);
	mcu_hw_gpio_pullup_pulldown_disable(GPIOE, 2);
	mcu_hw_gpio_drive_speed_set(GPIOE, 2, 0xFF);

	/* ------------ QSPI Initialization --------------- */

	QUADSPI->CR = (QUADSPI->CR & ~QUADSPI_CR_PRESCALER_Msk) | ((12U - 1U) << QUADSPI_CR_PRESCALER_Pos); //Default clock prescaler (3; max 216MHz/3 = 72MHz)
	QUADSPI->CR |= (QUADSPI_CR_SSHIFT_Msk); //sample shift off for STR
	QUADSPI->DCR |= ((4 - 1) << QUADSPI_DCR_CSHT_Pos); //chip select high time for read ops 20ns min; 50ns for write/prog/erase
	QUADSPI->DCR |= (24 - 1) << QUADSPI_DCR_FSIZE_Pos; //16MiB
	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags

	QUADSPI->CR |= QUADSPI_CR_EN;


	/* ---------- QSPI Flash Setup ---------- */
	quadspi_hw_ic_init();
	quadspi_memory_mapped_mode_enable();
}

void quadspi_hw_ic_init(void) {
	//Full reset regardless of Flash IC current state
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_RESET_ENABLE,
	QSPI_QUAD, 1);
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_RESET_MEMORY,
	QSPI_QUAD, 1);
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_RESET_ENABLE,
	QSPI_QUAD, 0);
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_RESET_MEMORY,
	QSPI_QUAD, 0);
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_RESET_ENABLE,
	QSPI_SINGLE, 0);
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_RESET_MEMORY,
	QSPI_SINGLE, 0);

	//Dummy cycles
	//XIP (no instruction) capability: ON (must be enabled via mode bit)
	//Wrap-around: continuous
	uint8_t VCR = ((uint8_t) MT25QL128ABA1EW9_QIO_DTR_DUMMYCYC_CNT << 4 | 0x00 << 3 | 0x03 << 0);
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_WRITE_ENABLE,
	QSPI_SINGLE, 0);
	mcu_hw_qspi_register_write_indirect(
	MT25QL128ABA1EW9_COMMAND_WRITE_VOLATILE_CONFIGURATION_REGISTER,
	QSPI_SINGLE, VCR, 0);

	uint8_t VCR_readback = 0;
	mcu_hw_qspi_register_read(
	MT25QL128ABA1EW9_COMMAND_READ_VOLATILE_CONFIGURATION_REGISTER,
	QSPI_SINGLE, &VCR_readback, 0);

	//Quad IO enable (0 << 7)
	//Double IO enable (0 << 6)
	//DTR enable (0 << 5)
	//Reser/hold enable (1 << 4)
	//Reserved (1 << 3)
	//30 Ohm (default) impedance (0x07 << 0)
	uint8_t EVCR = (0 << 7) | (1 << 6) | (1 << 5) | (0 << 4) | (1 << 3) | (0x07 << 0);
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_WRITE_ENABLE,
	QSPI_SINGLE, 0); //Enable write to register
	mcu_hw_qspi_register_write_indirect(
	MT25QL128ABA1EW9_COMMAND_WRITE_ENHANCED_VOLATILE_CONFIGURATION_REGISTER,
	QSPI_SINGLE, EVCR, 0);

	uint8_t EVCR_readback = 0;
	//QUADSPI->CR &= (~QUADSPI_CR_SSHIFT_Msk); //sample shift off for DTR
	//MT25QL128ABA1EW9_COMMAND_READ_ENHANCED_VOLATILE_CONFIGURATION_REGISTER
	mcu_hw_qspi_register_read(
	MT25QL128ABA1EW9_COMMAND_READ_ENHANCED_VOLATILE_CONFIGURATION_REGISTER,
	QSPI_QUAD, &EVCR_readback, 0);

	//mcu_hw_qspi_erase(MT25QL128ABA1EW9_COMMAND_4KB_SUBSECTOR_ERASE, 0, 0); //test

	QUADSPI->CR |= QUADSPI_CR_APMS; //automatic stop for polling

	QUADSPI->CR &= (~QUADSPI_CR_SSHIFT_Msk); //sample shift off for DTR
	QUADSPI->DCR &= ~QUADSPI_DCR_CSHT_Msk;
	QUADSPI->DCR |= ((2 - 1) << QUADSPI_DCR_CSHT_Pos); //chip select high time for read ops 20ns min; 50ns for write/prog/erase

	//mcu_hw_qspi_erase(MT25QL128ABA1EW9_COMMAND_4KB_SUBSECTOR_ERASE, 4096, 0); //test

}

void quadspi_memory_mapped_mode_enable(void) {
	//ERRATUM es0290 2.4.3
	QUADSPI->AR = 0;
	QUADSPI->CR |= QUADSPI_CR_ABORT;
	while (QUADSPI->CR & QUADSPI_CR_ABORT);
	//End of ERRATUM
	QUADSPI->CR &= ~QUADSPI_CR_EN;
	QUADSPI->DLR = 0;
	QUADSPI->CR |= ((12U - 1U) << QUADSPI_CR_PRESCALER_Pos); //Default clock prescaler (3; max 216MHz/3 = 72MHz)
	QUADSPI->CR |= (QUADSPI_CR_EN);

	//XIP mode initialization
	//read volatile configuration register
	uint8_t VCR;
	mcu_hw_qspi_register_read(
	MT25QL128ABA1EW9_COMMAND_READ_VOLATILE_CONFIGURATION_REGISTER,
	QSPI_QUAD, &VCR, 0);
	//modify (set XIP enable bit - bit 3 set to 0)
	VCR = VCR & ~(1 << 3); //XIP enable
	VCR &= ~0xF0;
	VCR |= ((uint8_t) MT25QL128ABA1EW9_QIO_DTR_DUMMYCYC_CNT << 4);
	//write volatile configuration register
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_WRITE_ENABLE,
	QSPI_QUAD, 0);
	mcu_hw_qspi_register_write_indirect(
	MT25QL128ABA1EW9_COMMAND_WRITE_VOLATILE_CONFIGURATION_REGISTER,
	QSPI_QUAD, VCR, 0);
	//operation completed (on the physical level)

	// Make sure there is no write in progress
	uint8_t sr_wip;
	do {
		mcu_hw_qspi_register_read(MT25QL128ABA1EW9_COMMAND_READ_STATUS_REGISTER, QSPI_QUAD, &sr_wip, 0);
	} while (sr_wip & (1 << 0));

	//Performing indirect read to activate XIP in the Flash IC

	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags
	QUADSPI->DLR = 1U - 1U; //Set number of bytes to read
	QUADSPI->CR = (QUADSPI->CR & ~QUADSPI_CR_FTHRES_Msk) | (0x00 << QUADSPI_CR_FTHRES_Pos); //Set FIFO threshold to 1

	/*
	 * Set communication configuration register
	 * Functional mode: 			Indirect read
	 * Data mode: 					4 Lines
	 * Instruction mode: 			4 Lines
	 * Address mode:				4 Lines
	 * Alternate byte mode/size     4 Lines/1 byte
	 * Address size:				24 Bits
	 * SDR/DTR:                     DTR
	 * Dummy cycles:				6 Cycles - 1byte*1cycle/byte = 5 Cycles
	 * Instruction: 				Quad Output Fast Read
	 *
	 * Set 24-bit Address
	 *
	 */
	QUADSPI->CR &= ~QUADSPI_CR_EN;
	QUADSPI->CR = (QUADSPI->CR & ~QUADSPI_CR_PRESCALER_Msk) | ((3U - 1U) << QUADSPI_CR_PRESCALER_Pos); //Default clock prescaler (3; max 216MHz/3 = 72MHz)
	QUADSPI->DCR |= ((2 - 1) << QUADSPI_DCR_CSHT_Pos); //chip select high time for read ops 20ns min; 50ns for write/prog/erase
	QUADSPI->CR &= ~(QUADSPI_CR_SSHIFT_Msk); //Sample shift 1/2 cycle disabled (for DTR)

	uint8_t XIP_activation_readout;
	QUADSPI->CCR = (1 << QUADSPI_CCR_DDRM_Pos) | (1 << QUADSPI_CCR_DHHC_Pos) | (QSPI_FMODE_INDIRECT_READ << QUADSPI_CCR_FMODE_Pos)
			| (QSPI_QUAD << QUADSPI_CCR_DMODE_Pos) | (QSPI_QUAD << QUADSPI_CCR_IMODE_Pos) | (QSPI_QUAD << QUADSPI_CCR_ADMODE_Pos)
			| (QSPI_ADSIZE_24 << QUADSPI_CCR_ADSIZE_Pos) | ((MT25QL128ABA1EW9_QIO_DTR_DUMMYCYC_CNT - 1) << QUADSPI_CCR_DCYC_Pos)
			| (MT25QL128ABA1EW9_COMMAND_DTR_QUAD_OUTPUT_FAST_READ << 0);
	QUADSPI->ABR = 0x00000000; //XIP bit set to 0 (XIP enable)
	QUADSPI->CCR = (QUADSPI->CCR & ~QUADSPI_CCR_ABMODE_Msk) | (QSPI_QUAD << QUADSPI_CCR_ABMODE_Pos); //alt byte 4 IO
	QUADSPI->CCR = (QUADSPI->CCR & ~QUADSPI_CCR_ABSIZE_Msk) | (0 << QUADSPI_CCR_ABSIZE_Pos); //1 alt byte
	QUADSPI->CR |= (QUADSPI_CR_EN);
	QUADSPI->AR = 0;

	/* ---------- Communication Starts Automatically ----------*/

	while (QUADSPI->SR & QUADSPI_SR_BUSY) {
		if (QUADSPI->SR & QUADSPI_SR_FTF) {
			XIP_activation_readout = *((uint8_t*) &(QUADSPI->DR)); //Read a byte from data register
		}
	}
	(void) XIP_activation_readout;
	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags

	//ERRATUM es0290 2.4.3
	QUADSPI->CR &= ~QUADSPI_CR_EN;
	QUADSPI->AR = 0;
	QUADSPI->CR |= (QUADSPI_CR_EN);
	QUADSPI->CR |= QUADSPI_CR_ABORT;
	while (QUADSPI->CR & QUADSPI_CR_ABORT);
	//End of ERRATUM

	//Enable memory-mapped XIP mode
	QUADSPI->CCR = (QUADSPI->CCR & ~(QUADSPI_CCR_FMODE_Msk | QUADSPI_CCR_IMODE_Msk)) | (QSPI_FMODE_MEMORY_MAPPED << QUADSPI_CCR_FMODE_Pos)
			| (QSPI_SKIP << QUADSPI_CCR_IMODE_Pos);

}

