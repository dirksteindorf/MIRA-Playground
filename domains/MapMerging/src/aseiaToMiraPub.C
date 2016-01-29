/*
 * Copyright (C) 2015 by
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
 * @file aseiaToMiraPub.C
 *    subscribes to GridMap from ASEIA and changes the frameID, so MIRA can merge the map with its own ones
 *
 * @author Dirk Steindorf
 * @date   2015/12/17
 */

#include <fw/Unit.h>
#include <fw/ChannelReadWrite.h>
#include <maps/GridMap.h>
#include <maps/OccupancyGrid.h>

using namespace mira;
using namespace maps;

namespace MapMerging { 

///////////////////////////////////////////////////////////////////////////////

/**
 * subscribes to GridMap from ASEIA and changes the frameID, so MIRA can merge the map with its own ones
 */
class aseiaToMiraPub : public Unit
{
MIRA_OBJECT(aseiaToMiraPub)

public:

    aseiaToMiraPub();

    template<typename Reflector>
    void reflect(Reflector& r)
    {
        Unit::reflect(r);

        // TODO: reflect all parameters (members and properties) that specify the persistent state of the unit
        //r.property("Param1", mParam1, "First parameter of this unit with default value", 123.4f);
        //r.member("Param2", mParam2, setter(&UnitName::setParam2,this), "Second parameter with setter");
        r.member("StaticMapChannel", staticMapChannel, "Channel where the static map can be found");
        r.member("AseiaMapChannel" , aseiaMapChannel , "Channel where the ASEIA map can be found");
        r.member("PublishTo"       , publishTo       , "Channel where this Unit publishes its maps");
    }

protected:

    virtual void initialize();
    virtual void process(const Timer& timer);

private:

    // void onPoseChanged(ChannelRead<Pose2> pose);
    void onNewStaticMap(ChannelRead<OccupancyGrid> map);
    void onNewAseiaMap(ChannelRead<OccupancyGrid> map);

private:

    //Channel<Img<>> mChannel;
    Channel<OccupancyGrid> sensorMapChannel;
    std::string staticMapChannel;
    std::string aseiaMapChannel;
    std::string publishTo;
};

///////////////////////////////////////////////////////////////////////////////

aseiaToMiraPub::aseiaToMiraPub() : Unit(Duration::milliseconds(100))
{
    // TODO: further initialization of members, etc.
}

void aseiaToMiraPub::initialize()
{
    // TODO: subscribe and publish all required channels
    //subscribe<Pose2>("Pose", &UnitName::onPoseChanged);
    //mChannel = publish<Img<>>("Image");
    sensorMapChannel = publish<OccupancyGrid>(publishTo);
}

void aseiaToMiraPub::process(const Timer& timer)
{
    auto staticChannel = subscribe<OccupancyGrid>(staticMapChannel);
    auto aseiaChannel  = subscribe<OccupancyGrid>(aseiaMapChannel);
    auto readStaticMap = staticChannel.read();
    auto readAseiaMap  = aseiaChannel.read();

    auto writeMap = sensorMapChannel.write();
    writeMap->timestamp = Time::now();
    writeMap->frameID   = readStaticMap->frameID;
    writeMap->value()   = readAseiaMap->value();
}

///////////////////////////////////////////////////////////////////////////////

}

MIRA_CLASS_SERIALIZATION(MapMerging::aseiaToMiraPub, mira::Unit );
