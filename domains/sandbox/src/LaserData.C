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
 * @file LaserData.C
 *    get the RangeScan data
 *
 * @author Dirk Steindorf
 * @date   2015/02/19
 */

#include <fw/MicroUnit.h>
#include <robot/RangeScan.h>
#include <transform/Velocity.h>

#include <iostream>
#include <fstream>

using namespace std;

using namespace mira;

namespace sandbox { 

///////////////////////////////////////////////////////////////////////////////

/**
 * get the RangeScan data
 */
class LaserData : public MicroUnit
{
MIRA_OBJECT(LaserData)

public:

	LaserData();

	template<typename Reflector>
	void reflect(Reflector& r)
	{
		MicroUnit::reflect(r);

		// TODO: reflect all parameters (members and properties) that specify the persistent state of the unit
		//r.property("Param1", mParam1, "First parameter of this unit with default value", 123.4f);
		//r.member("Param2", mParam2, setter(&UnitName::setParam2,this), "Second parameter with setter");
        r.property("minDistance", minDistance, " ", 0.2f);
        r.property("maxDistance", maxDistance, " ", 0.4f);
	}

protected:

	virtual void initialize();

private:

	// void onPoseChanged(ChannelRead<Pose2> pose);
    void onScan(ChannelRead<robot::RangeScan> data);

private:

	//Channel<Img<>> mChannel;
    ofstream laserFile;
    bool canWrite;
    float minDistance;
    float maxDistance;
};

///////////////////////////////////////////////////////////////////////////////

LaserData::LaserData()
{
	// TODO: further initialization of members, etc.
    canWrite = true;
}

void LaserData::initialize()
{
	// TODO: subscribe and publish all required channels
	//subscribe<Pose2>("Pose", &UnitName::onPoseChanged);
	//mChannel = publish<Img<>>("Image");
    subscribe<robot::RangeScan>("/robot/frontLaser/Laser", &LaserData::onScan);
}

void LaserData::onScan(ChannelRead<robot::RangeScan> data)
{
    float angle;
    float distance;

    waitForService("/robot/Robot");

    for(unsigned int i=0; i<data->range.size(); i++)
    {
        angle = i * 0.5f - 135;
        distance = data->range[i];
        
        if( distance > minDistance && 
            distance < maxDistance && 
            angle > -125.0f && 
            angle < 125.0f)
        {
            // angles greater than 89° are only reachable if the RoboHead turns the other way around
            // 90° is not reachable, so there's a gap between 89° and 91° that the RoboHead can not reach
            if(angle > 89.0f)
            {
                angle = 89.0f;
            }

            if(angle >= -135.0f && angle < 0.0f)
            {
                angle = angle + 360.0f;
                //cout<<"object at "<<angle<<" degree and "<<distance<<" meters"<<endl;
            }
            
            callService<void>("/robot/Robot", "moveHeadLeftRight", angle);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

}

MIRA_CLASS_SERIALIZATION(sandbox::LaserData, mira::MicroUnit );
