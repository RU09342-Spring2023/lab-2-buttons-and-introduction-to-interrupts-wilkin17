/*
 * OccupancyDetector.c
 *
 *  Created on: Jan 31, 2023
 *      Author: Luke Wilkins
 */
#include <msp430.h>
void gpioInit();

char armed = 0;                             // Global Variable to check if there is movement
int x = 0;                               // Arbitrary Counting Variable

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    gpioInit();                             // Configure GPIO

    P2IFG &= ~BIT3;                         // P2.3 IFG cleared
    __bis_SR_register(GIE);                 // Enter LPM3 w/interrupt




    while(1){
        switch (armed){
        case (1):                           // Warning state, if the sensor detects movement
            P6OUT &= ~BIT6;                 // Disable green LED
            P1OUT ^= BIT0;                  // Toggle P1.0
            __delay_cycles(500000);         // Delay for 500000*(1/MCLK)=0.5s
            P1OUT ^= BIT0;                  // Toggle P1.0
            __delay_cycles(500000);         // Delay for 500000*(1/MCLK)=0.5s
            if (x > 8){                     // If there's no movement for 10 blinks (~10 seconds), the sensor will return to the armed state
                armed = 0;
                P2IFG &= ~BIT3;             // Clear P2.3 IFG
            }
            else{
                x++;
            }
            break;

        case(0):                            // If the sensor does not detect any motion (blinks green LED every 3 seconds)
            P1OUT &= ~BIT0;                 // Disable red LED
            P6OUT ^= BIT6;                  // Toggle P6.6
            __delay_cycles(100000);         // Delay for 100000*(1/MCLK)=0.1s
            P6OUT ^= BIT6;                  // Toggle P6.6
            __delay_cycles(3000000);        // Delay for 3000000*(1/MCLK)=3s
            break;
        }

    }
    return 0;
}

void gpioInit(){
    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    P4DIR &= ~BIT1;                         // Set P4.1 to input direction
    P4REN |= BIT1;                          // Enable Resistor on P4.1
    P4OUT |= BIT1;                          // Configure Resistor on P4.1 to be Pullup

    //TEMP "movement" button
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    armed = 1;                   // Enable if the toggle should be active
    x = 0;                       // Reset movement timer
    P2IFG &= ~BIT3;              // Clear P2.3 IFG
}
