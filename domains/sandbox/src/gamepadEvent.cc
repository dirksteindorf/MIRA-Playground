#include "gamepadEvent.h"

//-----------------------------------------------------------------------------
// events
bool GamepadEvent::isButtonEvent()
{
	return (type & JS_EVENT_BUTTON) != 0;
}

bool GamepadEvent::isAxisEvent()
{
	return (type & JS_EVENT_AXIS) != 0;
}

bool GamepadEvent::isInitialState()
{
	return (type & JS_EVENT_INIT) != 0;
}

//-----------------------------------------------------------------------------
// Button state
bool GamepadEvent::isButtonPressed(short buttonNumber)
{
	return (isButtonEvent() && 
			number == buttonNumber &&
			value == 1);
}

bool GamepadEvent::isButtonReleased(short buttonNumber)
{
	return (isButtonEvent() &&
			number == buttonNumber &&
			value == 0);
}

//-----------------------------------------------------------------------------
// Axis directions 
bool GamepadEvent::isAxisLeft(short axisNumber)
{
	return (isAxisEvent() && 
			number == axisNumber &&
			value < -DEADZONE);
}

bool GamepadEvent::isAxisRight(short axisNumber)
{
	return (isAxisEvent() && 
			number == axisNumber &&
			value > DEADZONE);
}

bool GamepadEvent::isAxisUp(short axisNumber)
{
	return (isAxisEvent() && 
			number == axisNumber &&
			value < -DEADZONE);
}

bool GamepadEvent::isAxisDown(short axisNumber)
{
	return (isAxisEvent() && 
			number == axisNumber &&
			value > DEADZONE);
}

//-----------------------------------------------------------------------------
// directions of left stick
bool GamepadEvent::isLeftStickLeft()
{
	return isAxisLeft(6);
}

bool GamepadEvent::isLeftStickRight()
{
	return isAxisRight(6);
}

bool GamepadEvent::isLeftStickUp()
{
	return isAxisUp(7);
}

bool GamepadEvent::isLeftStickDown()
{
	return isAxisDown(7);
}

//-----------------------------------------------------------------------------
// directions of right stick
bool GamepadEvent::isRightStickLeft()
{
	return isAxisLeft(3);
}

bool GamepadEvent::isRightStickRight()
{
	return isAxisRight(3);
}

bool GamepadEvent::isRightStickUp()
{
	return isAxisUp(4);
}

bool GamepadEvent::isRightStickDown()
{
	return isAxisDown(4);
}

//-----------------------------------------------------------------------------
// Trigger states
bool GamepadEvent::isRightTriggerPressed()
{
	return isAxisDown(5);
}
bool GamepadEvent::isRightTriggerReleased()
{
	return isAxisUp(5);
}

bool GamepadEvent::isLeftTriggerPressed()
{
	return isAxisDown(2);
}
bool GamepadEvent::isLeftTriggerReleased()
{
	return isAxisUp(2);
}

//-----------------------------------------------------------------------------
// Bumper states
bool GamepadEvent::isRightBumperPressed()
{
	return isButtonPressed(5);
}

bool GamepadEvent::isRightBumperReleased()
{
	return isButtonReleased(5);
}

bool GamepadEvent::isLeftBumperPressed()
{
	return isButtonPressed(4);
}

bool GamepadEvent::isLeftBumperReleased()
{
	return isButtonReleased(4);
}

//-----------------------------------------------------------------------------
// DPad directions
bool GamepadEvent::isDPadLeft()
{
	return isAxisLeft(0);
}
bool GamepadEvent::isDPadRight()
{
	return isAxisRight(0);
}
bool GamepadEvent::isDPadUp()
{
	return isAxisUp(1);
}
bool GamepadEvent::isDPadDown()
{
	return isAxisDown(1);
}
