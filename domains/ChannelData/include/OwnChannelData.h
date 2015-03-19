#ifndef OWNCHANNELDATA_H
#define OWNCHANNELDATA_H

class OwnChannelData
{
public:
	template<typename Reflector>
    void reflect(Reflector& r)
    {
        r.member("ObjectID", id, "id of the object");
        r.member("ObjectValue", value, "stored value");
    }

    OwnChannelData();
    
    int id;
    int value;
};

#endif