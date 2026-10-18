#include "nile_gfx/hardware_specific/mcu_reset/gfx_mcu_reset.h"
void gfx_hw_init() {
	mcu_reset_sdram_init();
	mcu_reset_storage_qspi_init();
	mcu_reset_display_init();
	mcu_reset_graphics_dma2d_init();
}
