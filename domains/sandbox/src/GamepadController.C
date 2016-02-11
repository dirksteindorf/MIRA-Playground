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
 * @file GamepadController.C
 *    This Unit lets you drive the robot using a Gamepad.
 *
 * @author Dirk Steindorf
 * @date   2015/02/19
 */

#include <fw/Unit.h>
#include <transform/Velocity.h>
#include <robot/RangeScan.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include "gamepad.h"
#include "gamepadEvent.h"

#define SPEED_STEP 0.1f
#define MIN_SPEED 0.1f

using namespace mira;

namespace sandbox { 

///////////////////////////////////////////////////////////////////////////////

/**
 * This Unit lets you drive the robot using a Gamepad.
 */
class GamepadController : public Unit
{
MIRA_OBJECT(GamepadController)

public:

    GamepadController();

    template<typename Reflector>
    void reflect(Reflector& r)
    {
        Unit::reflect(r);

        r.property("maxSpeed", maxSpeed, "Maximum speed of the robot in meters per second", 1.0f);
        r.property("stopDistance", stopDistance, "distance to an obstacle at which the robot stops driving", 0.0f);
    }

protected:

    virtual void initialize();

    virtual void process(const Timer& timer);

private:

    void onScan(ChannelRead<robot::RangeScan> data);

private:

    Gamepad gamepad;
    GamepadEvent gamepadEvent;
    bool isDrivingAllowed;
    float transformSpeed;
    float rotationSpeed;
    float maxSpeed;
    float stopDistance;
};

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
GamepadController::GamepadController() : Unit(Duration::milliseconds(100))
{
    gamepad = Gamepad(0);
    isDrivingAllowed= true;
    transformSpeed = 0.0f;
    rotationSpeed = 0.0f;
}

//-----------------------------------------------------------------------------
void GamepadController::initialize()
{
    waitForService("/robot/Robot");
    subscribe<robot::RangeScan>("/robot/frontLaser/Laser", &GamepadController::onScan);

}

//-----------------------------------------------------------------------------
void GamepadController::process(const Timer& timer)
{
	
    if(gamepad.isDataAvailable(&gamepadEvent))
    {
        if(gamepadEvent.isButtonReleased(BUTTON_BACK))
        {
            isDrivingAllowed = false;
            callService<void>("/robot/Robot", 
                              "emergencyStop");
        }
        if(gamepadEvent.isButtonPressed(BUTTON_START))
        {
            std::cout << "pressed start" << std::endl;
            isDrivingAllowed = !isDrivingAllowed;
            transformSpeed = 0.0f;
            rotationSpeed = 0.0f;
            
            callService<void>("/robot/Robot", 
                              "resetMotorStop");

            callService<void>("/robot/Robot", 
                              "setVelocity", 
                              Velocity2(transformSpeed, 0.0f, rotationSpeed));
        }

        //---------------------------------------------------------------------
        // DPad 
        if(gamepadEvent.isDPadUp())
        {
            std::cout << "pressed up" << std::endl;
            transformSpeed += SPEED_STEP;
            rotationSpeed = 0.0f;
        }
        if(gamepadEvent.isDPadDown())
        {
            std::cout << "pressed down" << std::endl;
            transformSpeed -= SPEED_STEP;
            rotationSpeed = 0.0f;
        }
    
        if(gamepadEvent.isDPadRight())
        {
            rotationSpeed -= SPEED_STEP;
        }
        if(gamepadEvent.isDPadLeft())
        {
            rotationSpeed += SPEED_STEP;
        }
    
        //---------------------------------------------------------------------
        // color buttons 
        if(gamepadEvent.isButtonPressed(BUTTON_A) ||
           gamepadEvent.isButtonPressed(BUTTON_B)) 
        {
            transformSpeed = 0.0f;
            rotationSpeed = 0.0f;
        }

        if(gamepadEvent.isButtonPressed(BUTTON_X))
        {
            //system("festival --tts /localhome/demo/Documents/test.txt");
            callService<void>("/RPC", "sleep"); 
        }

        if(gamepadEvent.isButtonPressed(BUTTON_Y))
        {
            //system("festival --tts /localhome/demo/Documents/test.txt");
            callService<void>("/RPC", "wakeUp"); 
        }

        //---------------------------------------------------------------------
        // left stick
        if(gamepadEvent.isLeftStickUp())
        {
            callService<void>("/robot/Robot", 
                              "moveHeadLeftRight", 
                              0.0f); 
        }

        if(gamepadEvent.isLeftStickDown())
        {
            callService<void>("/robot/Robot", 
                              "moveHeadLeftRight", 
                              180.0f); 
        }

        if(gamepadEvent.isLeftStickLeft())
        {
            callService<void>("/robot/Robot", 
                              "moveHeadLeftRight", 
                              90.0f); 
        }
        if(gamepadEvent.isLeftStickRight())
        {
            callService<void>("/robot/Robot", 
                              "moveHeadLeftRight", 
                              270.0f); 
        }

        //---------------------------------------------------------------------
        // bumper and trigger 
        if(gamepadEvent.isRightTriggerPressed())
        {
            transformSpeed = maxSpeed;
        }

        if(gamepadEvent.isLeftTriggerPressed())
        {
            transformSpeed = MIN_SPEED;
        }

        // twinkle with left eye
        if(gamepadEvent.isLeftBumperPressed())
        {
            callService<void>("/robot/Robot", 
                              "twinkle", 
                              (unsigned char) 0, 
                              (unsigned int) 500);
        }
        // twinkle with right eye
        if(gamepadEvent.isRightBumperPressed())
        {
            callService<void>("/robot/Robot", 
                              "twinkle", 
                              (unsigned char) 1, 
                              (unsigned int) 500);
        }
    }

    if(isDrivingAllowed)
    {
        if(transformSpeed > maxSpeed)
        {
            transformSpeed = maxSpeed;
        }
        callService<void>("/robot/Robot", 
                          "setVelocity", 
                          Velocity2(transformSpeed, 0.0f, rotationSpeed));
    }
}

//-----------------------------------------------------------------------------
void GamepadController::onScan(ChannelRead<robot::RangeScan> data)
{
    float angle;
    float distance;

    for(unsigned int i=0; i<data->range.size(); i++)
    {
        angle = i * 0.5f - 135;
        distance = data->range[i];

        if(i > 10 && i < 530 && distance < stopDistance)
        {
            transformSpeed = 0.0f;
            rotationSpeed = 0.0f;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////

}

MIRA_CLASS_SERIALIZATION(sandbox::GamepadController, mira::Unit );
