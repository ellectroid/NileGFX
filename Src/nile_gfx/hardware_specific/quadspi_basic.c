#include "mcu_hw/qspi.h"
#include "mcu_hw/storage/MT25QL128ABA1EW9.h"

void mcu_hw_qspi_register_read(uint8_t instruction, uint8_t dataLinesMode, uint8_t *destination, uint8_t dtr) {

	while (QUADSPI->SR & QUADSPI_SR_BUSY); //Make sure no operation is going on

	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags
	QUADSPI->DLR = 1U - 1U; //Set data length to 1
	QUADSPI->CR = (QUADSPI->CR & ~(0x1F << 8)) | (0x00 << 8); //Set FIFO threshold to 1

	/*
	 * Set communication configuration register
	 * Functional mode: 			indirect read
	 * Data mode: 					as per param dataLinesMode
	 * Instruction mode: 			as per param dataLinesMode
	 * Instruction: 				as per param instruction
	 *
	 */
	QUADSPI->CCR = ((dtr != 0) << 31) | (QSPI_FMODE_INDIRECT_READ << 26) | (dataLinesMode << 24) | (dataLinesMode << 8) | (instruction << 0);

	/* ---------- Communication Starts Automatically ----------*/

	while (!(QUADSPI->SR & QUADSPI_SR_FLEVEL_Msk)); //Wait for the byte of data to arrive
	*destination = *((uint8_t*) (&(QUADSPI->DR))); //Read byte from data register and place it into provided memory; Byte access
	while (QUADSPI->SR & QUADSPI_SR_BUSY); //Make sure no operation is going on

}

/*
 * This function writes a single instruction via extended SPI, Dual SPI or Quad SPI
 *
 * @param		instruction		8-bit instruction
 * @param		dataLinesMode	1 = 1 data line
 * 								2 = 2 data lines
 * 								3 = 4 data lines
 * */
void mcu_hw_qspi_instruction_write_indirect(uint8_t instruction, uint8_t dataLinesMode, uint8_t dtr) {

	while (QUADSPI->SR & QUADSPI_SR_BUSY); //Make sure no operation is going on
	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags

	/*
	 * Set communication configuration register
	 * Functional mode: 			indirect write
	 * Instruction mode: 			as per param dataLines
	 * Instruction: 				as per param instruction
	 *
	 */
	QUADSPI->CCR = ((dtr != 0) << 31) | (QSPI_FMODE_INDIRECT_WRITE << 26) | (dataLinesMode << 8) | (instruction << 0);

	/* ---------- Communication Starts Automatically ----------*/

	while (!(QUADSPI->SR & QUADSPI_SR_TCF)); //Wait for operation to complete

}

/*
 * This function writes a byte to a register of QSPI Flash via extended SPI, Dual SPI or Quad SPI
 *
 * @param		instruction		8-bit instruction (write to register command)
 * @param		registerContent	New register value
 * @param		dataLinesMode	1 = 1 data line
 * 								2 = 2 data lines
 * 								3 = 4 data lines
 * */
void mcu_hw_qspi_register_write_indirect(uint8_t instruction, uint8_t dataLinesMode, uint8_t registerContent, uint8_t dtr) {

	while (QUADSPI->SR & QUADSPI_SR_BUSY); //Make sure no operation is going on
	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags

	QUADSPI->DLR = 1U - 1U; //Set number of bytes to write: 1
	QUADSPI->CR = (QUADSPI->CR & ~QUADSPI_CR_FTHRES_Msk) | (0x00 << QUADSPI_CR_FTHRES_Pos); //Set FIFO threshold to 1

	/*
	 * Set communication configuration register
	 * Functional mode: 			indirect write
	 * Data mode: 					as per param dataLinesMode
	 * Instruction mode: 			as per param dataLinesMode
	 * Instruction: 				as per param instruction
	 *
	 * Load data
	 *
	 */
	QUADSPI->CCR = ((dtr != 0) << 31) | (QSPI_FMODE_INDIRECT_WRITE << 26) | (dataLinesMode << 24) | (dataLinesMode << 8) | (instruction << 0);
	QUADSPI->DR = registerContent; //Load new register value into data register

	/* ---------- Communication Starts Automatically ----------*/

	while (!(QUADSPI->SR & QUADSPI_SR_TCF)); //Wait for operation to complete

}

/*
 * This function reads QSPI memory via Quad SPI
 *
 * @param		address			address of the first byte to read
 * @param		length			how many bytes to read
 * @param		destination		destination array
 *
 * */
void mcu_hw_qspi_read_indirect(uint32_t address, uint32_t length, uint8_t *destination, uint8_t dtr) {

	while (QUADSPI->SR & QUADSPI_SR_BUSY); //Make sure no operation is going on
	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags
	QUADSPI->DLR = length - 1U; //Set number of bytes to read
	QUADSPI->CR = (QUADSPI->CR & ~QUADSPI_CR_FTHRES_Msk) | (0x00 << QUADSPI_CR_FTHRES_Pos); //Set FIFO threshold to 1

	/*
	 * Set communication configuration register
	 * Functional mode: 			Indirect read
	 * Data mode: 					4 Lines
	 * Instruction mode: 			4 Lines
	 * Address mode:				4 Lines
	 * Address size:				24 Bits
	 * Dummy cycles:				6 Cycles
	 * Instruction: 				Quad Output Fast Read
	 *
	 * Set 24-bit Address
	 *
	 */

//	QUADSPI->CCR = ((dtr != 0) << 31) | (QSPI_FMODE_INDIRECT_READ << 26) | (QSPI_QUAD << 24) | (QSPI_QUAD << 8) | (QSPI_QUAD << 10) | (QSPI_ADSIZE_24 << 12) | (0x06 << 18)
//			| (MT25QL128ABA1EW9_COMMAND_QUAD_OUTPUT_FAST_READ << 0);
	QUADSPI->CCR = ((dtr != 0) << 31) | (QSPI_FMODE_INDIRECT_READ << 26) | (QSPI_QUAD << 24) | (QSPI_QUAD << 8) | (QSPI_QUAD << 10) | (QSPI_ADSIZE_24 << 12) | (0x06 << 18)
				| (MT25QL128ABA1EW9_COMMAND_FAST_READ << 0);
	QUADSPI->AR = (0x00FFFFFF) & address;

	/* ---------- Communication Starts Automatically ----------*/

	while (QUADSPI->SR & QUADSPI_SR_BUSY) {
		if (QUADSPI->SR & QUADSPI_SR_FTF) {
			*destination = *((uint8_t*) &(QUADSPI->DR)); //Read a byte from data register, byte access
			destination++;
		}
	}
}

/*
 * This function erases QSPI memory via Quad SPI
 *
 * @param		address			address of the first byte of subsector to erase
 * @param		instruction		8-bit instruction (various erase commands)
 *
 * */
void mcu_hw_qspi_erase(uint8_t instruction, uint32_t address, uint8_t dtr) {

	while (QUADSPI->SR & QUADSPI_SR_BUSY); //Make sure no operation is going on
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_WRITE_ENABLE,
	QSPI_QUAD, dtr); //erase requires write enable command first
	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags

	/*
	 * Set communication configuration register
	 * Functional mode: 			indirect write
	 * Address mode:				4 lines
	 * Address size:				24 bits
	 * Address:						as per param address (any address in subsector is valid)
	 * Instruction mode: 			4 lines
	 * Instruction: 				as per param instruction
	 *
	 * Set 24-bit Address
	 *
	 */
	QUADSPI->CCR = ((dtr != 0) << 31) | (QSPI_FMODE_INDIRECT_WRITE << 26) | (QSPI_QUAD << 10) | (0x02 << 12) | (QSPI_QUAD << 8) | (instruction << 0);
	QUADSPI->AR = 0x00FFFFFF & address;

	/* ---------- Communication Starts Automatically ----------*/

	while (QUADSPI->SR & QUADSPI_SR_BUSY); //Make sure no operation is going on

	/* ---------- Waiting for erase process to end ---------- */

	uint8_t eraseEnded = 0x00;
	do {
		mcu_hw_qspi_register_read(MT25QL128ABA1EW9_COMMAND_READ_FLAG_STATUS_REGISTER,
		QSPI_QUAD, &eraseEnded, dtr);
		eraseEnded &= 0x80;
	} while (eraseEnded == 0x00);

}

void mcu_hw_qspi_write_indirect(uint32_t address, uint32_t length, uint8_t data[], uint8_t dtr) {

	while (QUADSPI->SR & QUADSPI_SR_BUSY); //Make sure no operation is going on
	mcu_hw_qspi_instruction_write_indirect(MT25QL128ABA1EW9_COMMAND_WRITE_ENABLE,
	QSPI_QUAD, dtr); //program requires write enable
	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags

	QUADSPI->DLR = length - 1U; //Set number of bytes to write
	QUADSPI->CR = (QUADSPI->CR & ~QUADSPI_CR_FTHRES_Msk) | (0x00 << QUADSPI_CR_FTHRES_Pos); //Set FIFO threshold to 1

	/*
	 * Set communication configuration register
	 * Functional mode: 			Indirect write
	 * Data mode: 					4 lines
	 * Instruction mode: 			4 lines
	 * Address mode:				4 lines
	 * Address size:				24 bits
	 * Instruction: 				Quad Input Fast Program
	 *
	 * Write 24-bit Address
	 *
	 */
	QUADSPI->CCR = ((dtr != 0) << 31) | (QSPI_FMODE_INDIRECT_WRITE << 26) | (QSPI_QUAD << 24) | (QSPI_QUAD << 8) | (QSPI_QUAD << 10) | (QSPI_ADSIZE_24 << 12)
			| (MT25QL128ABA1EW9_COMMAND_QUAD_INPUT_FAST_PROGRAM << 0);
	QUADSPI->AR = 0x00FFFFFF & address;
	uint32_t data_pointer = 0;
	do {
		*((uint8_t*) (&QUADSPI->DR)) = data[data_pointer]; //place data - byte access
		data_pointer++;
		while ((QUADSPI->SR & QUADSPI_SR_FLEVEL_Msk) != 0x00); //wait for the data to be shifted out
	} while (QUADSPI->SR & QUADSPI_SR_BUSY);

	/* ---------- Communication Starts Automatically, ends at this point ----------*/

	/* ---------- Checking status register to make sure writing operation has finished ---------- */
	uint8_t readyStatusFlag = 0x00;
	QUADSPI->FCR = 0xFFFFFFFF; //Clear all flags
	do {
		mcu_hw_qspi_register_read(MT25QL128ABA1EW9_COMMAND_READ_FLAG_STATUS_REGISTER,
		QSPI_QUAD, &readyStatusFlag, dtr);
		readyStatusFlag &= 0x80;
	} while (readyStatusFlag == 0x00);

}

void mcu_hw_qspi_memory_mapped_mode_enable(uint8_t instruction, uint8_t dummy_cycles, uint8_t dtr) {
	//ERRATUM es0290 2.4.3
	QUADSPI->AR = 0;
	QUADSPI->CR |= QUADSPI_CR_ABORT;
	while(QUADSPI->CR & QUADSPI_CR_ABORT);
	//End of ERRATUM
	QUADSPI->CR &= ~QUADSPI_CR_EN;

	QUADSPI->DLR = 0;
	QUADSPI->CR |= (QUADSPI_CR_EN);

	//QUADSPI->CR = (QUADSPI->CR & ~(QUADSPI_CR_FTHRES_Msk)) | (0x03 << 8); //Set FIFO threshold to 4
	QUADSPI->CR |= (0x01 << QUADSPI_CR_SSHIFT_Pos); //Sample shift 1/2 cycle
	if(dtr != 0){
		QUADSPI->CR &= ~(QUADSPI_CR_SSHIFT_Msk); //Sample shift 1/2 cycle
	}
	QUADSPI->DCR |= ((2 - 1) << QUADSPI_DCR_CSHT_Pos); //chip select high time for read ops 20ns min; 50ns for write/prog/erase
	QUADSPI->CCR = ((dtr != 0) << 31) | ((dtr != 0) << 30) | (QSPI_FMODE_MEMORY_MAPPED << 26) | (QSPI_QUAD << 24) | (QSPI_QUAD << 8) | (QSPI_QUAD << 10) | (QSPI_ADSIZE_24 << 12) | ((dummy_cycles) << 18)
			| (instruction << 0);
	//QUADSPI->LPTR = 0x0000; //release chip select after...


}
