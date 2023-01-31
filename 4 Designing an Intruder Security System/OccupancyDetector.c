/*
 * OccupancyDetector.c
 *
 *  Created on: Jan 31, 2023
 *      Author: Luke Wilkins
 */
#include <msp430.h>
void gpioInit();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    gpioInit();                             // Configure GPIO


    int armed = 0;

    while(1){
        if (armed == 0){                    // The green LED will blink once every 3s while the system does not detect anyone
            P6OUT ^= BIT6;                  // Toggle P6.6
            __delay_cycles(100000);         // Delay for 100000*(1/MCLK)=0.1s
            P6OUT ^= BIT6;                  // Toggle P6.6
            __delay_cycles(3000000);        // Delay for 3000000*(1/MCLK)=3s
        }

    }
    return 0;
}

void gpioInit(){
    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction
    P4DIR &= ~BIT1;                         // Set P4.1 to input direction

    P4REN |= BIT1;                          // Enable Resistor on P4.1
    P4OUT |= BIT1;                          // Configure Resistor on P4.1 to be Pullup
}

