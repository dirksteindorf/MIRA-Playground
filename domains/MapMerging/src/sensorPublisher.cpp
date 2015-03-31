#include <iostream>
#include <fw/Framework.h>
#include <fw/ChannelReadWrite.h>
#include <maps/OccupancyGrid.h>

using namespace mira;
using namespace std;

Authority authority;

mira::Channel<mira::maps::OccupancyGrid> mapChannel;
mira::maps::OccupancyGrid openMap;
mira::maps::OccupancyGrid closedMap;


void checkInput(const Timer& timer)
{
    std::string input;
    cout<<"Send message: ";
    cin>>input;

    // close client if "exit" was submitted
    if(input=="exit")
        exit(-1);

    mira::ChannelWrite<mira::maps::OccupancyGrid> writeMap = mapChannel.write();

    if(input=="open")
        writeMap->value() = openMap;

    // publish map with closed door
    else if(input=="closed")
       writeMap->value() = closedMap;
}


int main(int argc, char** argv)
{
    Framework framework(argc, argv, true);

    // prerequisites for map publishing
    mira::Path filenameOpen("/home/dirk/MIRA/maps/map-open.png");
    mira::Path filenameClosed("/home/dirk/MIRA/maps/map-closed.png");

    mira::Point2i offset(206, 126);
    float cellsize = 0.05f;

    openMap = mira::maps::loadOccupancyGridFromFile(filenameOpen, cellsize, offset);
    closedMap = mira::maps::loadOccupancyGridFromFile(filenameClosed, cellsize, offset);


    // creating and starting the authority
    authority.checkin("/", "sensorAuthority");
    authority.createTimer(Duration::milliseconds(50), &checkInput);

    // MIRA channels
    mapChannel = authority.publish<mira::maps::OccupancyGrid>("sensorMap");

    authority.start();

    return framework.run();
}
