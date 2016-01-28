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
 * @file staticMapPublisher.C
 *    publishes a static map to merge it later on with a map aquired from an external sensor
 *
 * @author Dirk Steindorf
 * @date   2015/12/09
 */

#include <iostream>
#include <fw/Unit.h>
#include <fw/ChannelReadWrite.h>
#include <maps/OccupancyGrid.h>

using namespace mira;
using namespace std;

namespace MapMerging { 

  ///////////////////////////////////////////////////////////////////////////////

  /**
   * publishes a static map to merge it later on with a map aquired from an external sensor
   */
  class staticMapPublisher : public Unit
  {
    MIRA_OBJECT(staticMapPublisher)

    public:

      staticMapPublisher();

      template<typename Reflector>
        void reflect(Reflector& r)
        {
          Unit::reflect(r);

          // TODO: reflect all parameters (members and properties) that specify the persistent state of the unit
          //r.property("Param1", mParam1, "First parameter of this unit with default value", 123.4f);
          //r.member("Param2", mParam2, setter(&UnitName::setParam2,this), "Second parameter with setter");
          r.member("StaticMapFile"   , staticMapFile   , "Path to the image of the static map");
          r.member("StaticMapChannel", staticMapChannel, "Channel where the static map is published to");
        }

    protected:

      virtual void initialize();

      virtual void process(const Timer& timer);

    private:

      // void onPoseChanged(ChannelRead<Pose2> pose);

    private:

      Channel<mira::maps::OccupancyGrid> mChannel;
      mira::Path filenameStatic;
      mira::Point2i offset;
      float cellsize;
      mira::maps::OccupancyGrid staticMap;
      std::string staticMapFile;
      std::string staticMapChannel;
  };

  ///////////////////////////////////////////////////////////////////////////////

  staticMapPublisher::staticMapPublisher() : Unit(Duration::milliseconds(100))
  {
    // TODO: further initialization of members, etc.
    offset = mira::Point2i(0,0);
    cellsize = 0.05f;
  }

  void staticMapPublisher::initialize()
  {
    // TODO: subscribe and publish all required channels
    //subscribe<Pose2>("Pose", &UnitName::onPoseChanged);
    mChannel = publish<mira::maps::OccupancyGrid>(staticMapChannel);

    filenameStatic = mira::Path(staticMapFile);
    staticMap      = mira::maps::loadOccupancyGridFromFile(filenameStatic, cellsize, offset);
  }

  void staticMapPublisher::process(const Timer& timer)
  {
    // TODO: this method is called periodically with the specified cycle time, so you can perform your computation here.
    mira::ChannelWrite<mira::maps::OccupancyGrid> writeStaticMap = mChannel.write();

    writeStaticMap->timestamp = mira::Time::now();
    writeStaticMap->frameID = resolveName("MapFrame"); 
    //cout << writeStaticMap->frameID << endl;
    writeStaticMap->value() = staticMap;
  }

  ///////////////////////////////////////////////////////////////////////////////

}

MIRA_CLASS_SERIALIZATION(MapMerging::staticMapPublisher, mira::Unit );
