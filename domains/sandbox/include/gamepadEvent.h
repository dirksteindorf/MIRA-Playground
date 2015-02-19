#ifndef GAMEPAD_EVENT_HH
#define GAMEPAD_EVENT_HH

#define JS_EVENT_BUTTON 0x01 // button pressed/released
#define JS_EVENT_AXIS 	0x02 // joystick moved
#define JS_EVENT_INIT	0x80 // initial state of device

#define DEADZONE		5	// deadzone for axis values

#define BUTTON_A		0
#define BUTTON_B		1
#define BUTTON_X		2
#define BUTTON_Y		3
#define BUTTON_BACK		6
#define BUTTON_START 	7
#define BUTTON_LOGITECH	8

class GamepadEvent
{
	public:
		unsigned int time; // event timestamp		
		short value;
		unsigned char type;
		unsigned char number; // axis/button number
		
		bool isButtonEvent();
		bool isAxisEvent();
		bool isInitialState();

		bool isButtonPressed(short buttonNumber);
		bool isButtonReleased(short buttonNumber);

		bool isAxisLeft(short axisNumber);
		bool isAxisRight(short axisNumber);
		bool isAxisUp(short axisNumber);
		bool isAxisDown(short axisNumber);

		bool isLeftStickLeft();
		bool isLeftStickRight();
		bool isLeftStickUp();
		bool isLeftStickDown();

		bool isRightStickLeft();
		bool isRightStickRight();
		bool isRightStickUp();
		bool isRightStickDown();

		bool isRightTriggerPressed();
		bool isRightTriggerReleased();

		bool isLeftTriggerPressed();
		bool isLeftTriggerReleased();

		bool isRightBumperPressed();
		bool isRightBumperReleased();

		bool isLeftBumperPressed();
		bool isLeftBumperReleased();

		bool isDPadLeft();
		bool isDPadRight();
		bool isDPadUp();
		bool isDPadDown();
};
#endif
