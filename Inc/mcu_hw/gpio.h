#ifndef MCU_HW_GPIO_H_
#define MCU_HW_GPIO_H_

#include "stm32f7xx.h"
#include "nile_gfx/gfx_stdtypes.h"

void mcu_hw_gpio_alternate_function_set(GPIO_TypeDef *port, uint8_t pin, uint8_t af);
void mcu_hw_gpio_open_drain_set(GPIO_TypeDef *port, uint8_t pin) ;
void mcu_hw_gpio_push_pull_set(GPIO_TypeDef *port, uint8_t pin);
void mcu_hw_gpio_pullup_pulldown_disable(GPIO_TypeDef *port, uint8_t pin);
void mcu_hw_gpio_drive_speed_set(GPIO_TypeDef *port, uint8_t pin, uint8_t drive_speed);

#endif /* MCU_HW_GPIO_H_ */
