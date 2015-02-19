#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include "gamepad.h"

Gamepad::Gamepad()
{
	openDevice("/dev/input/js0");
}

Gamepad::Gamepad(int deviceNumber)
{
	std::stringstream s;
	s<<"/dev/input/js"<<deviceNumber;
	openDevice(s.str());
}

Gamepad::Gamepad(std::string devicePath)
{
	openDevice(devicePath);
}

void Gamepad::openDevice(std::string devicePath)
{
	_fileDescriptor = open(devicePath.c_str(), O_RDONLY | O_NONBLOCK);
}

bool Gamepad::isDataAvailable(GamepadEvent* event)
{
	int data = read(_fileDescriptor, event, sizeof(*event));
	if(data == -1)
	{
		return false;
	}
	else
	{
		// ensure that we are still in sync
		return data == sizeof(*event);
	}
}
