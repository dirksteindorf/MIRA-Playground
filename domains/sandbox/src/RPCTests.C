/*
 * Copyright (C) 2012 by
 *   MetraLabs GmbH (MLAB), GERMANY
 * and
 *   Neuroinformatics and Cognitive Robotics Labs (NICR) at TU Ilmenau, GERMANY
 * All rights reserved.
 *
 * Contact: info@mira-project.org
 *
 * Commercial Usage:
 *   Licensees holding valid commercial licenses may use this file in
 *   accordance with the commercial license agreement provided with the
 *   software or, alternatively, in accordance with the terms contained in
 *   a written agreement between you and MLAB or NICR.
 *
 * GNU General Public License Usage:
 *   Alternatively, this file may be used under the terms of the GNU
 *   General Public License version 3.0 as published by the Free Software
 *   Foundation and appearing in the file LICENSE.GPL3 included in the
 *   packaging of this file. Please review the following information to
 *   ensure the GNU General Public License version 3.0 requirements will be
 *   met: http://www.gnu.org/copyleft/gpl.html.
 *   Alternatively you may (at your option) use any later version of the GNU
 *   General Public License if such license has been publicly approved by
 *   MLAB and NICR (or its successors, if any).
 *
 * IN NO EVENT SHALL "MLAB" OR "NICR" BE LIABLE TO ANY PARTY FOR DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF
 * THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF "MLAB" OR
 * "NICR" HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * "MLAB" AND "NICR" SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND "MLAB" AND "NICR" HAVE NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS OR MODIFICATIONS.
 */

/**
 * @file RPCTests.C
 *    This Unit uses different RPCs to combine multiple movements of the RoboHead.
 *
 * @author Dirk Steindorf
 * @date   2015/02/19
 */

#include <fw/Unit.h>
#include <transform/Velocity.h>
#include <transform/RigidTransform.h>
#include <transform/Pose.h>
#include <iostream>
#include <cstdlib>
#include <string>

using namespace mira;
using namespace std;

namespace sandbox { 

///////////////////////////////////////////////////////////////////////////////

/**
 * This Unit uses different RPCs to combine multiple movements of the RoboHead.
 */
class RPCTests : public Unit
{
MIRA_OBJECT(RPCTests)

public:

	RPCTests();

	template<typename Reflector>
	void reflect(Reflector& r)
	{
		Unit::reflect(r);

		// TODO: reflect all parameters (members and properties) that specify the persistent state of the unit
		//r.property("Param1", mParam1, "First parameter of this unit with default value", 123.4f);
		//r.member("Param2", mParam2, setter(&UnitName::setParam2,this), "Second parameter with setter");

		// methods can be called in the RPC console in miracenter
		// for example: /RPCtests.nod()
		r.method("nod", &RPCTests::nod, this, "This method lets the RoboHead nod.");
		r.method("nod2", &RPCTests::nod2, this, "This method lets the RoboHead nod.");
		r.method("sleep", &RPCTests::sleep, this, 
				"This method lets the RoboHead look down and close its eyes.");
		r.method("wakeUp", &RPCTests::wakeUp, this, 
				"This method lets the RoboHead look straight ahead and open its eyes.");
		r.method("LEDoff", &RPCTests::LEDoff, this, "This method turns off all LEDs.");
		r.method("LEDon", &RPCTests::LEDon, this, "This method turns on all LEDs.");
		r.method("driveToGoal", &RPCTests::driveToGoal, this, "lets the robot drive to a predefined position");

		// properties can be changed in the PropertyView in miracenter
		// RPCTests has to be selected in the AuthorityView to see these properties
		r.property("HeadUpPosition", headUp, "Upward facing head position in degree", 5.0f);
		r.property("HeadDownPosition", headDown, "Downward facing head position in degree", 0.0f);
        // enumeration does not work without giving a default value
        //r.property("IntProp2", mInt2, "comment", 333,  
        //           PropertyHints::enumeration("-1000=Blink;333=Blarg;2000=Huiiii"));
    }
	

protected:

	virtual void initialize();

	virtual void process(const Timer& timer);

private:

	// void onPoseChanged(ChannelRead<Pose2> pose);
	void nod();
	void nod2();
	void sleep();
	void wakeUp();
	void LEDoff();
	void LEDon();
    void driveToGoal();

private:

	//Channel<Img<>> mChannel;
	float headUp;
	float headDown;
    int mInt2;
};

///////////////////////////////////////////////////////////////////////////////

RPCTests::RPCTests() : Unit(Duration::milliseconds(100))
{
	// TODO: further initialization of members, etc.
}

void RPCTests::initialize()
{
	// TODO: subscribe and publish all required channels
	//subscribe<Pose2>("Pose", &UnitName::onPoseChanged);
	//mChannel = publish<Img<>>("Image");

	publishService(*this);
}

void RPCTests::process(const Timer& timer)
{
	// TODO: this method is called periodically with the specified cycle time, so you can perform your computation here.
}

//-----------------------------------------------------------------------------
// head moves up and down again
// angles can be set in the PropertyView in miracenter
void RPCTests::nod()
{
	std::string headProperty = "Head.HeadTilt";
	std::string value;
	RPCFuture<std::string> result;

	waitForService("/robot/Robot");

	// TODO: remove this
	callService<void>("/robot/Robot", "setVelocity", mira::Velocity2(0.5f, 0.0f, 0.0f));
	// remove until here

	callService<void>("/robot/Robot", "moveHeadUpDown", headUp);

	// wait for the first movement to be completed
	do
	{
		result = callService<std::string>("/robot/Robot#builtin", "getProperty", headProperty);
		result.wait();
		value = result.get();
	}
	while(atof(value.c_str()) < (headUp - 0.3f));

	callService<void>("/robot/Robot", "moveHeadUpDown", headDown);	
}

//-----------------------------------------------------------------------------
// nodding - starting with a downward movement
void RPCTests::nod2()
{
	std::string headProperty = "Head.HeadTilt";
	std::string value;
	RPCFuture<std::string> result;

	waitForService("/robot/Robot");
	callService<void>("/robot/Robot", "moveHeadUpDown", headDown);

	// wait for the first movement to be completed
	do
	{
		result = callService<std::string>("/robot/Robot#builtin", "getProperty", headProperty);
		result.wait();
		value = result.get();
	}
	while(atof(value.c_str()) > (headDown + 0.5));

	callService<void>("/robot/Robot", "moveHeadUpDown", headUp);	
}

//-----------------------------------------------------------------------------
// turn all LEDs off
void RPCTests::LEDoff()
{
	std::string led = "Head.LEDPhase";
	
	callService<void>("/robot/Robot#builtin", "setProperty", 
						std::string("Head.HeadLightInterval"), std::string("400"));

	for(int i=0; i<8; i++)
	{
		callService<void>("/robot/Robot#builtin", "setProperty", led+to_string(i), to_string(0));
	}
}

//-----------------------------------------------------------------------------
// turn all LEDs on
void RPCTests::LEDon()
{
	std::string led = "Head.LEDPhase";

	callService<void>("/robot/Robot#builtin", "setProperty", 
						std::string("Head.HeadLightInterval"), std::string("80"));

	for(int i=0; i<8; i++)
	{
		callService<void>("/robot/Robot#builtin", "setProperty", led+to_string(i), to_string(i*10));
	}
}

//-----------------------------------------------------------------------------
// move head down, close eyes
void RPCTests::sleep()
{
	unsigned char eyes = 2;

	waitForService("/robot/Robot");
	callService<void>("/robot/Robot", "moveHeadUpDown", -7.0f);
	callService<void>("/robot/Robot", "moveEyeLidUpDown", eyes, 0.0f);
	LEDoff();
	//system("festival --tts /localhome/demo/Documents/test.txt");
}

//-----------------------------------------------------------------------------
// head faces straight forward, open eyes
void RPCTests::wakeUp()
{
	unsigned char eyes = 2;

	waitForService("/robot/Robot");
	callService<void>("/robot/Robot", "moveHeadUpDown", 0.0f);
	callService<void>("/robot/Robot", "moveEyeLidUpDown", eyes, 100.0f);
	LEDon();
}


//-----------------------------------------------------------------------------
// lets the robot drive to a predefined goal
void RPCTests::driveToGoal()
{
	waitForService("/robot/Robot");
	waitForService("/navigation/Pilot");
    callService<void>("/navigation/Pilot", "setGoal", Pose2(2.45f, 0.0f, 270.0f), 0.1f, 0.1f);
}

///////////////////////////////////////////////////////////////////////////////

}


MIRA_CLASS_SERIALIZATION(sandbox::RPCTests, mira::Unit );
