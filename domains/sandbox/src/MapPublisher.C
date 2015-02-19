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
 * @file MapPublisher.C
 *    This unit loads a map from a specified file and publishes it to a channel.
 *
 * @author Dirk Steindorf
 * @date   2015/02/19
 */

#include <fw/Unit.h>
#include <fw/ChannelReadWrite.h>
#include <maps/OccupancyGrid.h>

using namespace mira;

namespace sandbox { 

///////////////////////////////////////////////////////////////////////////////

/**
 * This unit loads a map from a specified file and publishes it to a channel.
 */
class MapPublisher : public Unit
{
MIRA_OBJECT(MapPublisher)

public:

	MapPublisher();

	template<typename Reflector>
	void reflect(Reflector& r)
	{
		Unit::reflect(r);

		// TODO: reflect all parameters (members and properties) that specify the persistent state of the unit
		//r.property("Param1", mParam1, "First parameter of this unit with default value", 123.4f);
		//r.member("Param2", mParam2, setter(&UnitName::setParam2,this), "Second parameter with setter");
	}

protected:

	virtual void initialize();

	virtual void process(const Timer& timer);

private:

    void saveOneMap(ChannelRead<mira::maps::OccupancyGrid> mergedMap);

private:

    Channel<mira::maps::OccupancyGrid> mapChannel;
    Channel<mira::maps::OccupancyGrid> mapChannel2;

    mira::maps::OccupancyGrid map1;
    mira::maps::OccupancyGrid map2;

    mira::Path filename1;
    mira::Path filename2;
    mira::Path mergePath;
    
    mira::Point2i offset1;
    mira::Point2i offset2;

    float cellsize;

    bool saveMap;
};

///////////////////////////////////////////////////////////////////////////////

MapPublisher::MapPublisher() : Unit(Duration::milliseconds(100))
{
    filename1 = mira::Path("/localhome/demo/map/office3/map-smile.png");
    filename2 = mira::Path("/localhome/demo/map/office3/map-static.png");
    mergePath = mira::Path("/localhome/demo/map/office3/map-merged.png");

    offset1 = mira::Point2i(154, 166);
    offset2 = mira::Point2i(0, 0);

    cellsize = 0.05f;

    saveMap = true;
}

void MapPublisher::initialize()
{
    mapChannel = publish<mira::maps::OccupancyGrid>("ownMap");
    mapChannel2 = publish<mira::maps::OccupancyGrid>("ownMap2");

    subscribe<mira::maps::OccupancyGrid>("MergedMap", &MapPublisher::saveOneMap);
}

void MapPublisher::process(const Timer& timer)
{
    map1 = mira::maps::loadOccupancyGridFromFile(filename1, cellsize, offset1);
    map2 = mira::maps::loadOccupancyGridFromFile(filename2, cellsize, offset2);

    //-----------------------------------------------------
    // publish first map
    mira::ChannelWrite<mira::maps::OccupancyGrid> writeMap1 = mapChannel.write();
    writeMap1->timestamp = mira::Time::now();
    writeMap1->value() = map1;

    // SCITOSDriver has to be included for this
    writeMap1->frameID = resolveName("GlobalFrame"); 
    writeMap1.finish();

    //-----------------------------------------------------
    // publish second map
    mira::ChannelWrite<mira::maps::OccupancyGrid> writeMap2 = mapChannel2.write();
    writeMap2->timestamp = mira::Time::now();
    writeMap2->value() = map2;

    // SCITOSDriver has to be included for this
    writeMap2->frameID = resolveName("GlobalFrame"); 
    writeMap2.finish();
}

void MapPublisher::saveOneMap(ChannelRead<mira::maps::OccupancyGrid> mergedMap)
{
    if(saveMap)
    {
        saveOccupancyGridToFile(mergePath, mergedMap);
        saveMap = false;
    }
}
///////////////////////////////////////////////////////////////////////////////

}

MIRA_CLASS_SERIALIZATION(sandbox::MapPublisher, mira::Unit );
