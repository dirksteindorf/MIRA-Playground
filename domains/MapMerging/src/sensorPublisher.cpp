#include <iostream>
#include <fw/Framework.h>
#include <fw/ChannelReadWrite.h>
#include <maps/OccupancyGrid.h>

using namespace mira;
using namespace std;

Authority authority;

mira::Channel<mira::maps::OccupancyGrid> sensorMapChannel;
mira::Channel<mira::maps::OccupancyGrid> staticMapChannel;

mira::maps::OccupancyGrid openMap;
mira::maps::OccupancyGrid closedMap;
mira::maps::OccupancyGrid staticMap;


void checkInput(const Timer& timer)
{
    std::string input;
    cout<<"Send message: ";
    cin>>input;

    // close client if "exit" was submitted
    if(input=="exit")
        exit(-1);

    mira::ChannelWrite<mira::maps::OccupancyGrid> writeSensorMap = sensorMapChannel.write();

    // publish sensor map
    writeSensorMap->timestamp = mira::Time::now();
    writeSensorMap->frameID = authority.resolveName("GlobalFrame"); 

    if(input=="open")
        writeSensorMap->value() = openMap;
    else if(input=="closed")
        writeSensorMap->value() = closedMap;
}

void publishStaticMap(const Timer& timer)
{
    mira::ChannelWrite<mira::maps::OccupancyGrid> writeStaticMap = staticMapChannel.write();
    
    writeStaticMap->timestamp = mira::Time::now();
    writeStaticMap->frameID = authority.resolveName("GlobalFrame"); 
    writeStaticMap->value() = staticMap;
}


int main(int argc, char** argv)
{
    Framework framework(argc, argv, true);

    // prerequisites for map publishing
    mira::Path filenameOpen("/home/dirk/MIRA/maps/map-open.png");
    mira::Path filenameClosed("/home/dirk/MIRA/maps/map-closed.png");
    mira::Path filenameStatic("/home/dirk/MIRA/maps/map-static.png");

    mira::Point2i staticOffset(172, 274);
    mira::Point2i offset(206, 283);
    float cellsize = 0.05f;

    openMap = mira::maps::loadOccupancyGridFromFile(filenameOpen, cellsize, offset);
    closedMap = mira::maps::loadOccupancyGridFromFile(filenameClosed, cellsize, offset);
    staticMap = mira::maps::loadOccupancyGridFromFile(filenameStatic, cellsize, staticOffset);


    // creating and starting the authority
    authority.checkin("/", "sensorAuthority");
    authority.createTimer(Duration::milliseconds(50), &checkInput);
    authority.createTimer(Duration::milliseconds(50), &publishStaticMap);

    // MIRA channels
    sensorMapChannel = authority.publish<mira::maps::OccupancyGrid>("sensorMap");
    staticMapChannel = authority.publish<mira::maps::OccupancyGrid>("staticMap");

    authority.start();

    return framework.run();
}
