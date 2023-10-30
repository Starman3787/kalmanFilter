#include <stdint.h>
#include "src/headers/stm32f412zx.h"
#include "src/drivers/gtu7/gtu7.h"
#include "src/main.h"

int8_t Gtu7_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    GPIOB->MODER |= (0b10 << (__DRIVERS_GTU7_PIN_RX__ * 2)) |
                    (0b10 << (__DRIVERS_GTU7_PIN_TX__ * 2));

    GPIOB->AFR[1] |= (0b0111 << ((__DRIVERS_GTU7_PIN_RX__ - 8) * 4)) |
                     (0b0111 << ((__DRIVERS_GTU7_PIN_TX__ - 8) * 4));

    USART3->BRR = (__MAIN_H_CORE_CLOCK__ / __DRIVERS_GTU7_BAUD_RATE__);

    USART3->CR1 |= USART_CR1_RE |
                   USART_CR1_TE |
                   USART_CR1_UE;

    return 0;
}