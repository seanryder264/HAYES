#include "GPIO.h"
#include "ADC.h"

int main(void)
{
    HAL_Init();

    GPIO_TX_Init();
    GPIO_MUX_Init();
}