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
 * @file MapMerger.C
 *    The MapMerger combines multiple maps of different areas into one global map.
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
 * The MapMerger combines multiple maps of different areas into one global map.
 */
class MapMerger : public Unit
{
MIRA_OBJECT(MapMerger)

public:

	MapMerger();

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

    // for maps published under /ownMap
    //void onNewMap(ChannelRead<mira::maps::OccupancyGrid> data);

    // for maps published under /maps/static/Map
    //void onNewStaticMap(ChannelRead<mira::maps::OccupancyGrid> data);

private:
    /*
    Channel<mira::maps::OccupancyGrid> mergedMapChannel;

    std::vector<mira::maps::OccupancyGrid> requiredMaps;
    std::vector<mira::maps::OccupancyGrid> optionalMaps;

    mira::maps::OccupancyGrid staticMap;
    mira::maps::OccupancyGrid ownMap;

    mira::Path staticMapPath;
    mira::Path ownMapPath;
    mira::Point2i offset;
    float cellsize;
    */
};

///////////////////////////////////////////////////////////////////////////////

MapMerger::MapMerger() : Unit(Duration::milliseconds(100))
{
	// TODO: further initialization of members, etc.
   /*
    staticMapPath = mira::Path("/localhome/demo/map/office3/map-static.png");
    ownMapPath = mira::Path("/localhome/demo/map/office3/map-nogo.png");

    offset = mira::Point2i(0, 0);
    cellsize = 0.05f;
    */
}

void MapMerger::initialize()
{
    //subscribe<mira::maps::OccupancyGrid>("maps/static/Map", &MapMerger::onNewStaticMap);
    //subscribe<mira::maps::OccupancyGrid>("ownMap", &MapMerger::onNewMap);

    //mergedMapChannel = publish<mira::maps::OccupancyGrid>("ownMergedMap");
}

void MapMerger::process(const Timer& timer)
{
	// TODO: this method is called periodically with the specified cycle time, so you can perform your computation here.
    
    //staticMap = mira::maps::loadOccupancyGridFromFile(staticMapPath, cellsize, offset);
    //ownMap = mira::maps::loadOccupancyGridFromFile(ownMapPath, cellsize, offset);

    //requiredMaps.push_back(staticMap);
    //optionalMaps.push_back(ownMap);
    
}

/*
void MapMerger::onNewMap()
{
}

void MapMerger::onNewStaticMap()
{
}
*/

///////////////////////////////////////////////////////////////////////////////

}

MIRA_CLASS_SERIALIZATION(sandbox::MapMerger, mira::Unit );
