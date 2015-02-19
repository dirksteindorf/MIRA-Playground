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
 * @file remoteOrders.C
 *    waits for strings from remote frameworks and reacts to them
 *
 * @author Dirk Steindorf
 * @date   2015/02/19
 */

#include <fw/Unit.h>
#include <transform/Pose.h>
#include <fw/ChannelReadWrite.h>

using namespace mira;

namespace sandbox { 

///////////////////////////////////////////////////////////////////////////////

/**
 * waits for strings from remote frameworks and reacts to them
 */
class remoteOrders : public Unit
{
MIRA_OBJECT(remoteOrders)

public:

	remoteOrders();

	template<typename Reflector>
	void reflect(Reflector& r)
	{
		Unit::reflect(r);

		//reflect all parameters (members and properties) that specify the persistent state of the unit
		//r.property("Param1", mParam1, "First parameter of this unit with default value", 123.4f);
		//r.member("Param2", mParam2, setter(&UnitName::setParam2,this), "Second parameter with setter");
	}

protected:

	virtual void initialize();
	virtual void process(const Timer& timer);

private:

    void onPause(ChannelRead<std::string> data);
    void onKuka1(ChannelRead<std::string> data);
    void onKuka2(ChannelRead<std::string> data);
    void onPilotEvent(ChannelRead<std::string> data);
    
    void handleKuka1(Pose2 start, Pose2 end, Pose2 waitPosition);
    void handleKuka2(Pose2 start, Pose2 end, Pose2 waitPosition);
    
    void resetKuka1();
    void resetKuka2();
    void waitFor(bool expression);
    void waitForPilot();
    
    void log(std::string msg);

private:

	//Channel<Img<>> mChannel;
    Channel<std::string> scitosToKuka1;
    Channel<std::string> scitosToKuka2;
    bool moveHeadUp;
    
    bool paused;
    bool stopCommand;
    bool isAside;

	bool kuka1online;
	bool kuka2online;
	bool connected;

	bool kuka1searching;
	bool kuka2searching;

	bool kuka1follows;
	bool kuka2follows;

	bool kuka1stopped;
	bool kuka2stopped;

	bool kuka1finished;
	bool kuka2finished;
	
	// TODO: replace with real values
	float DUMMY;
	
	Pose2 workstation1;
	Pose2 workstation2;
	Pose2 workstation3;
	Pose2 workstation4;
	
	Pose2 sideline1;
	Pose2 sideline2;
	
	std::string PilotEvent;
	std::string followMe;
	std::string stopFollowing;
	std::string scitosHere;
};

///////////////////////////////////////////////////////////////////////////////

remoteOrders::remoteOrders() : Unit(Duration::milliseconds(100))
{
    moveHeadUp = true;
    
    paused = false;
    stopCommand = false;
    isAside = false;

	kuka1online = false;
	kuka2online = false;
	connected = true;

	kuka1searching = false;
	kuka2searching = false;

	kuka1follows = false;
	kuka2follows = false;

	kuka1stopped = false;
	kuka2stopped = false;

	kuka1finished = false;
	kuka2finished = false;
	
	DUMMY = 0.0f;
	
	// TODO: these are still dummy values
	workstation1 = Pose2(2.0f, 0.0f, 90.0f);
	workstation2 = Pose2(3.0f, 0.0f, 90.0f);
	workstation3 = Pose2(2.0f, -8.0f, 270.0f);
	workstation4 = Pose2(3.0f, -8.0f, 270.0f);
	
	// TODO: these are still dummy values
	sideline1 = Pose2(1.0f, 0.0f, 180.0f);
	sideline1 = Pose2(1.0f, -8.0f, 180.0f);
	
	followMe = "follow me";
	stopFollowing = "stop following";
	scitosHere = "scitos here";
}

void remoteOrders::initialize()
{
	subscribe<std::string>("kuka1ToScitos", &remoteOrders::onKuka1);
	subscribe<std::string>("kuka2ToScitos", &remoteOrders::onKuka2);
	subscribe<std::string>("/navigation/PilotEvent", &remoteOrders::onPilotEvent);

    scitosToKuka1 = publish<std::string>("scitosToKuka1");
    scitosToKuka2 = publish<std::string>("scitosToKuka2");
    
    waitForService("/navigation/Pilot");
}

//------------------------------------------------------------------------------
// easier string output
void remoteOrders::log(std::string msg)
{
    std::cout<<msg<<std::endl;
}

//------------------------------------------------------------------------------
// react to pause button
// TODO: implement pause button
void remoteOrders::onPause(ChannelRead<std::string> data)
{
    if(data->value() == "pause")
    {
        log("pausing");
        paused = true;
        callService<void>("/robot/Robot", "emergencyStop");
    }

    if(data->value() == "continue")
    {
        log("continuing");
        paused = false;
        callService<void>("/robot/Robot", "emergencyStop");
    }
}

//------------------------------------------------------------------------------
// callback for Kuka 1
void remoteOrders::onKuka1(ChannelRead<std::string> data)
{
    if(data->value() == "heard you")
    {   
        log("kuka 1 heard me");
        kuka1online = true;
    }

    if(data->value() == "searching")
    {
        log("kuka 1 is searching");
        kuka1searching = true;
    }

    if(data->value() == "marker found")
    {
        log("kuka 1 found the marker");
        kuka1follows = true;
    }

    if(data->value() == "okay")
    {
        log("kuka 1 stopped following");
        kuka1stopped = true;
    }

    if(data->value() == "done")
    {
        log("kuka 1 is done");
        kuka1finished = true;
    }
}

//------------------------------------------------------------------------------
// callback for Kuka 2
void remoteOrders::onKuka2(ChannelRead<std::string> data)
{
    if(data->value() == "heard you")
    {
        log("kuka 2 heard me");
        kuka2online = true;
    }

    if(data->value() == "searching")
    {
        log("kuka 2 is searching");
        kuka2searching = true;
    }

    if(data->value() == "marker found")
    {
        log("kuka 2 found the marker");
        kuka2follows = true;
    }

    if(data->value() == "okay")
    {
        log("kuka 2 stopped following");
        kuka2stopped = true;
    }

    if(data->value() == "done")
    {
        log("kuka 2 is done");
        kuka2finished = true;
    }
}

//------------------------------------------------------------------------------
// store value of /navigation/PilotEvent in a variable
void remoteOrders::onPilotEvent(ChannelRead<std::string> data)
{
    PilotEvent = data->value();
}

//------------------------------------------------------------------------------
// bring Kuka 1 from its current workstation to its new one
void remoteOrders::handleKuka1(Pose2 start, Pose2 end, Pose2 waitPosition)
{
    // drive to first workstation
    RPCFuture<void> result1 = callService<void>("/navigation/Pilot", "setGoal", 
                                Pose2(DUMMY, DUMMY, DUMMY), 0.05f, 0.05f);
    result1.wait();
    log("driving to workstation to get kuka 1");
    waitForPilot();
    log("arrived at workstation");
    
    // turn marker to Kuka
    RPCFuture<void> result2 = callService<void>("/navigation/Pilot", "setGoal", 
                               Pose2(DUMMY, DUMMY, DUMMY), DUMMY, DUMMY);
    result2.wait();
    log("turning marker to kuka 1");
    waitForPilot();
    
    log("telling kuka 1 to follow me");
    scitosToKuka1.post(followMe);

    waitFor(kuka1searching);
    log("kuka 1 is searching");
    waitFor(kuka1follows);
    log("kuka 1 follows me");

    // drive to third workstation
    RPCFuture<void> result3 = callService<void>("/navigation/Pilot", "setGoal", 
                                Pose2(DUMMY, DUMMY, DUMMY), DUMMY, DUMMY);
    result3.wait();
    log("driving to workstation with kuka 1");
    //waitForPilot(); // does not work
    if(PilotEvent == "GoalReached")
    
        log("arrived at workstation");
    
        log("telling kuka 1 to stop following");
        scitosToKuka1.post(stopFollowing);
        
        //waitFor(kuka1stopped);
        if(kuka1stopped)
            log("kuka 1 stopped following, moving aside");

            // drive to the side
            if(!isAside)
                RPCFuture<void> result4 = callService<void>("/navigation/Pilot", "setGoal", 
                                Pose2(DUMMY, DUMMY, DUMMY), DUMMY, DUMMY);
                isAside = true;

            //waitFor(kuka1finished);
            if(kuka1finished)
                log("kuka 1 is done, getting kuka 2");
                log("====================================================================");
                log(" ");
}

//------------------------------------------------------------------------------
// bring Kuka 2 from its current workstation to its new one
void remoteOrders::handleKuka2(Pose2 start, Pose2 end, Pose2 waitPosition)
{
    // drive to first workstation
    RPCFuture<void> result1 = callService<void>("/navigation/Pilot", "setGoal", 
                                Pose2(DUMMY, DUMMY, DUMMY), DUMMY, DUMMY);
    result1.wait();
    log("driving to workstation to get kuka 2");
    waitForPilot();
    log("arrived at workstation");
    
    // turn marker to Kuka
    RPCFuture<void> result2 = callService<void>("/navigation/Pilot", "setGoal", 
                               Pose2(DUMMY, DUMMY, DUMMY), DUMMY, DUMMY);
    result2.wait();
    log("turning marker to kuka 2");
    waitForPilot();
    
    log("telling kuka 2 to follow me");
    scitosToKuka2.post(followMe);

    waitFor(kuka2searching);
    log("kuka 2 is searching");
    waitFor(kuka2follows);
    log("kuka 2 follows me");

    // drive to third workstation
    RPCFuture<void> result3 = callService<void>("/navigation/Pilot", "setGoal", 
                                Pose2(DUMMY, DUMMY, DUMMY), DUMMY, DUMMY);
    result3.wait();
    log("driving to workstation with kuka 2");
    waitForPilot();
    log("arrived at workstation");
    
    log("telling kuka 2 to stop following");
    scitosToKuka2.post(stopFollowing);
    waitFor(kuka2stopped);
    log("kuka 2 stopped following, moving aside");

    // drive to the side
    RPCFuture<void> result4 = callService<void>("/navigation/Pilot", "setGoal", 
                                Pose2(DUMMY, DUMMY, DUMMY), DUMMY, DUMMY);

    //waitFor(kuka2stopped);
    waitFor(kuka2finished);
    log("kuka 2 is done, getting kuka 1");
    log("====================================================================");
    log(" ");
}

//------------------------------------------------------------------------------
// reset variables for Kuka 1
void remoteOrders::resetKuka1()
{
    bool kuka1searching = false;
    bool kuka1follows = false;
    bool kuka1stopped = false;
    bool kuka1finished = false;
}

//------------------------------------------------------------------------------
// reset variables for Kuka 2
void remoteOrders::resetKuka2()
{
    bool kuka2searching = false;
    bool kuka2follows = false;
    bool kuka2stopped = false;
    bool kuka2finished = false;
}

//------------------------------------------------------------------------------
// helper function to wait for certain values
void remoteOrders::waitFor(bool expression)
{
    while(!expression){}
}

//------------------------------------------------------------------------------
// helper function to wait for the PilotEvent
void remoteOrders::waitForPilot()
{
    while(PilotEvent != "GoalReached")
    {
        if(PilotEvent != "Idle") log(PilotEvent);
    }
}

//------------------------------------------------------------------------------
// main loop
void remoteOrders::process(const Timer& timer)
{
    if(!kuka1online)
    {
        scitosToKuka1.post(scitosHere);
    }

    if(!kuka2online)
    {
        scitosToKuka2.post(scitosHere);
    }

    connected = kuka1online && kuka2online;

    if(connected)
    {
        while(!stopCommand) // TODO: define stop command
        {
            handleKuka1(workstation1, workstation3, sideline2);
            handleKuka2(workstation2, workstation4, sideline2);

            handleKuka1(workstation3, workstation1, sideline1);
            handleKuka2(workstation4, workstation2, sideline1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

}

MIRA_CLASS_SERIALIZATION(sandbox::remoteOrders, mira::Unit );
