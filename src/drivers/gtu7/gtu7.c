#include <stdint.h>
#include "src/headers/stm32f412zx.h"
#include "src/drivers/gtu7/gtu7.h"
#include "src/main.h"

int8_t Gtu7_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    GPIOB->MODER |= (0b10 << (GTU7_PIN_RX * 2)) |
                    (0b10 << (GTU7_PIN_TX * 2));

    GPIOB->AFR[1] |= (0b0111 << ((GTU7_PIN_RX - 8) * 4)) |
                     (0b0111 << ((GTU7_PIN_TX - 8) * 4));

    USART3->BRR = (CORE_CLOCK / GTU7_BAUD_RATE);

    USART3->CR1 |= USART_CR1_RE |
                   USART_CR1_TE |
                   USART_CR1_UE;

    return 0;
}