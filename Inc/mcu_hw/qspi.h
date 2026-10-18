#ifndef MCU_HW_QSPI_H_
#define MCU_HW_QSPI_H_

#include "stm32f7xx.h"

/* ---------- QUADSPI Definitions  ---------- */

#define QSPI_ADSIZE_8 ((uint8_t)0x00)
#define QSPI_ADSIZE_16 ((uint8_t)0x01)
#define QSPI_ADSIZE_24 ((uint8_t)0x02)
#define QSPI_ADSIZE_32 ((uint8_t)0x03)

#define QSPI_SKIP ((uint8_t)0x00)
#define QSPI_SINGLE ((uint8_t)0x01)
#define QSPI_DOUBLE ((uint8_t)0x02)
#define QSPI_QUAD ((uint8_t)0x03)

#define QSPI_FMODE_INDIRECT_WRITE ((uint8_t)0x00)
#define QSPI_FMODE_INDIRECT_READ ((uint8_t)0x01)
#define QSPI_FMODE_AUTOMATIC_POLLING ((uint8_t)0x02)
#define QSPI_FMODE_MEMORY_MAPPED ((uint8_t)0x03)


/* ---------- QUADSPI Functions ---------- */

void mcu_hw_qspi_instruction_write_indirect(uint8_t instruction, uint8_t dataLinesMode, uint8_t dtr);

void mcu_hw_qspi_register_write_indirect(uint8_t instruction, uint8_t dataLinesMode, uint8_t registerContent, uint8_t dtr);
void mcu_hw_qspi_register_read(uint8_t instruction, uint8_t dataLinesMode, uint8_t *destination, uint8_t dtr);

void mcu_hw_qspi_write_indirect(uint32_t address, uint32_t length, uint8_t* data, uint8_t dtr);

void mcu_hw_qspi_read_indirect(uint32_t address, uint32_t length, uint8_t* destination, uint8_t dtr);
void mcu_hw_qspi_erase(uint8_t erase_instruction, uint32_t address, uint8_t dtr);

void mcu_hw_qspi_memory_mapped_mode_enable(uint8_t instruction, uint8_t dummy_cycles, uint8_t dtr);


#endif /* MCU_HW_QSPI_H_ */
