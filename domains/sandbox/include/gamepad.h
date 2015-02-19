#ifndef GAMEPAD_HH
#define GAMEPAD_HH

#include <iostream>
#include "gamepadEvent.h"

class Gamepad
{
	private:
		void openDevice(std::string path);
		int _fileDescriptor;
	public:
		Gamepad();
		Gamepad(int deviceNumber);
		Gamepad(std::string path);
		bool isDataAvailable(GamepadEvent* event);

		bool isAvailable()
		{
			return _fileDescriptor != -1;
		}
};
#endif
