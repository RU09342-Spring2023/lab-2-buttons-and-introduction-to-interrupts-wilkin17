# Occupancy Detection and Alert System
### Designed by Luke Wilkins

## What is it?
This program is a detection and alert system that has three different states.
1. The default armed state - denoted by a green blinking LED.
2. The warning state       - intended as a transition between the armed and alert states. Denoted by a red blinking LED.
3. The alert state         - for when the system detects prolonged motion. Denoted by a solid red LED.

## How does it work?
The sensor detects "movement" based off of when SW2 (P2.3) is pressed. Every time it is pressed, it's registered as a "movement."
- While the system is in its armed state, the green LED (P6.6) will flash every 3 seconds. 
	- If the system detects "movement" in the form of the button press, it will enter the warning state.
- The warning state occurs whenever the system detects "movement." The green LED will turn off and the red LED (P1.0) will begin flashing very quickly to show it is in the warning state. The moment the movement is gone, the system will return to the armed state.
	- However, if there is repeated movement for 10+ seconds, the system will see that as a threat and enter the alert state.
- The alert state is a fixed state, and can only be disabled by resetting the system by pressing SW1 (P4.1). The red LED will turn solid to show that the device is in its warning state.