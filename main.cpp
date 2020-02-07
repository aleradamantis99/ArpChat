#include <iostream>
#include <sys/ioctl.h>
#include <net/if.h>
#include <cstring>
#include <unistd.h>
#include <netpacket/packet.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <iomanip>
#include <cstddef>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "socket.h"
#include "packets.hpp"

int main ()
{
    using namespace network;  

    std::string iface("wlan0");
    network::socket s(TESTPROTO_RAW);
    std::string destMac("74:d4:35:ea:53:1d"), srcMac("3c:a0:67:69:42:c9"), msg("Hola tio");
    network::packet<Eth, Data<13>> p{network::craftFrame(destMac, srcMac)};
    
    auto& data = get<Data<13>>(p).payload;
    std::copy (msg.begin(), msg.end(), data.begin());
    
    
    auto socketAddress = network::makeAddress(iface, destMac, s.getFd());
    
    s.sendTo(&p, socketAddress, 14+9);
}
