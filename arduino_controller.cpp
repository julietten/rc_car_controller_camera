#include <SPI.h>
#include <RH_ASK.h>

//defines digital input pins
#define jb 17 //joystick button
#define b1 5 //button 1
#define b2 6 //button 2
#define s1 3 //switch 1
#define s2 4 //switch 2

RH_ASK driver(2000,A3,11,A1,false); //initializes transmitter and receiver pair
uint8_t jpotx = 0; //stores analog input x value of joystick
uint8_t jpoty = 0; //stores analog input y value of joystick
uint8_t jbutton = 0; //stores digital input of joystick button
uint8_t button1 = 0; //stores digital input of button 1
uint8_t button2 = 0; //stores digital input of button 2
uint8_t switch1 = 0; //stores digital input of switch 1
uint8_t switch2 = 0; //stores digital input of switch 2

/**
*resets inputs to neutral values
*these inputs are the 7 inputs sent to the car from the controller
*/
void defaultData(){ 
	jpotx = 127; //sets x movement of joystick to neutral, values range from 0 to 255
	jpoty = 127; //sets y movement of joystick to neutral, values range from 0 to 255
	jbutton = 1; //sets joystick button to neutral, 1 because it has pullup resistors set in controller
	button1 = 1; //sets button 1 to neutral, 1 because it has pullup resistors set in controller
	button2 = 1; //sets button 2 to neutral, 1 because it has pullup resistors set in controller
	switch1 = 1; //sets switch 1 to neutral, 1 because it has pullup resistors set in controller
	switch2 = 1; //sets switch 2 to neutral, 1 because it has pullup resistors set in controller
}

/**
*sets up controller inputs
*initializes transmitter
*sets all inputs to their default values
*/
void setup(){
	pinMode(jb, INPUT_PULLUP); //sets arduino internal pull-up resistor for joystick button
	pinMode(b1, INPUT_PULLUP); //sets arduino internal pull-up resistor for button 1
	pinMode(b2, INPUT_PULLUP); //sets arduino internal pull-up resistor for button 2
	pinMode(s1, INPUT_PULLUP); //sets arduino internal pull-up resistor for switch 1
	pinMode(s2, INPUT_PULLUP); //sets arduino internal pull-up resistor for switch 2
	driver.init(); //initializes transmitter
	defaultData(); //resets all input button values to default
	Serial.begin(9600);
	 if (!driver.init()) //checks if the initialization of the transmitter failed
         Serial.println("init failed"); //print out in case the transmitter intialization failed
}

/**
*reads analog input values from the joystick and converts them to 8 bit values
*reads digital input values
*constructs a signal consisting of 7 bytes, 1 for each input, and sends the signal to the car
*continuously loops through this function for constant transmission
*/
void loop(){
	jpotx = map(analogRead(A0), 0, 1023, 0, 255); //stores x value of joystick as 8 bits (mapped from 10)
	jpoty = map(analogRead(A2), 0, 1023, 0, 255); //stores y value of joystick as 8 bits (mapped from 10)
	
	jbutton = digitalRead(jb); //stores digital value of the joystick button
	button1 = digitalRead(b1); //stores digital value of button 1
	button2 = digitalRead(b2); //stores digital value of button 2
	switch1 = digitalRead(s1); //stores digital value of switch 1
	switch2 = digitalRead(s2); //stores digital value of switch 2

	uint8_t dataArray[] = {jpotx, jpoty, jbutton, button1, button2, switch1, switch2}; //stores all buttons and analog inputs on controller in a 7 byte array
	const uint8_t *data = dataArray; //stores address of dataArray
	driver.send((uint8_t *)data, 7); //sends signals containing 7 byte array to receiver
	driver.waitPacketSent(); //sends signal
}
