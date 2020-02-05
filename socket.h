#pragma once
#include "packets.hpp"

namespace network
{
//const uint16_t TESTPROTO_RAW = 0x88b5;
const uint16_t TESTPROTO_RAW = 0x6969;
const size_t MAXDATASIZE = 1486;

struct EthernetMsg;
struct NetworkHeader;

class socket
{
private:
    int fd_;
    
public:
    socket (int protocol);
    socket (const socket&) = delete;  
    void sendTo (void* message, const sockaddr_ll& address, size_t size) const;
    //void receiveFrom (EthernetMsg& message, sockaddr_ll& address, size_t size) const;
    ~socket (void);
    int getFd (void) const { return fd_; }
    
private:
    void bindSocket (const sockaddr_in& address);
};

void getInterfaceMac (const std::string& ifaceName);
int getInterfaceIndex(const std::string& ifacename, int sockfd);
Eth craftFrame (const std::string& dest, const std::string& source);
void addNetworkLayer (EthernetMsg& p, const std::string& dest, const std::string& src);
std::array <unsigned char, 6> transformMac (const std::string& mac);
sockaddr_ll makeAddress (const std::string& iface, const std::string& mac, int fd);
}
