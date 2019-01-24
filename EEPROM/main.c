/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"
#include "utils/uartstdio.h"

void UART0_Init(uint32_t Baud);
uint8_t EEPROM_init();

int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |  SYSCTL_XTAL_16MHZ);//System Frequency: 40M

    uint32_t pui32Data[2];
    uint32_t pui32Read[2];

    UART0_Init(115200); //init uart 0
    EEPROM_init();      //init eeprom

    EEPROMRead(pui32Read, 0x400, sizeof(pui32Read));    //read data from 0x400

    pui32Data[0] = pui32Read[0]+1;
    pui32Data[1] = pui32Read[1]+1;

    EEPROMProgram(pui32Data, 0x400, sizeof(pui32Data)); //write data to 0x400
    EEPROMRead(pui32Read, 0x400, sizeof(pui32Read));    //read data from 0x400

    UARTprintf("%d %d\n", pui32Read[0], pui32Read[1]);    //output

    while(1)
    {
    }
}

//
//function:Init EEPROM.
//sucess  :return 0
//fail    :return 1
uint8_t EEPROM_init()
{
    uint32_t ui32EEPROMInit;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0))
    {
    }

    ui32EEPROMInit = EEPROMInit();

    if(ui32EEPROMInit != EEPROM_INIT_OK)
    {
        UARTprintf("EEPROM Init Error!\n");
        return 1;
    }
    else
    {
        return EEPROM_INIT_OK;
    }
}

void UART0_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);//Clock 16MHz
    UARTStdioConfig(0, Baud, 16000000);
}
