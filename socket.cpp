#include <array>
#include <cstdio>
#include <vector>
#include <netpacket/packet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <system_error>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> //close()
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <algorithm>
#include <linux/if_ether.h>

#include "socket.h"

namespace network
{
socket::socket (int protocol)
{
    int fd = ::socket(AF_PACKET, SOCK_RAW, htons(protocol));

    if (fd < 0)
        throw std::system_error(errno, std::system_category(), "No se pudo crear el socket");
    fd_ = fd;
}

void socket::sendTo(void* message, const sockaddr_ll &address, size_t size) const
{
    //int result = sendto(fd_, &message, sizeof(message), 0, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
    int result = sendto(fd_, message, size, 0, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
    if (result < 0)
        throw std::system_error(errno, std::system_category(), "Fallo durante el envio");

}
/*
void socket::receiveFrom (EthernetMsg &message, sockaddr_ll &address) const
{
    socklen_t srcLen = sizeof (address);
    int result = recvfrom(fd_, &message, sizeof(message), 0, reinterpret_cast<sockaddr*>(&address), &srcLen);
    if (result < 0)
        throw std::system_error(errno, std::system_category(), "Fallo durante la recepcion");
}*/

socket::~socket(void)
{
	close (fd_);
}

auto getInterfaceMac (const std::string& ifaceName, int sockfd)
{
    ::ifreq ifMac;
    std::memset(&ifMac, 0, sizeof(ifreq));
    std::strncpy(ifMac.ifr_name, ifaceName.c_str(), ifaceName.size());
    std::cout << ifMac.ifr_name << std::endl;
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifMac) < 0)
        perror("SIOCGIFHWADDR");
    /*
    for (int i=0; i<6; ++i)
        std::cout << std::hex << static_cast<unsigned short>((unsigned char)ifMac.ifr_hwaddr.sa_data[i]) << ((i<5)?':':'\n');
    */
    ifMac.ifr_hwaddr.sa_data;
}

int getInterfaceIndex(const std::string& ifacename, int sockfd)
{
        ifreq ifIdx;
        memset(&ifIdx, 0, sizeof(struct ifreq));
        strncpy(ifIdx.ifr_name, ifacename.c_str(), ifacename.size());
        if (ioctl(sockfd, SIOCGIFINDEX, &ifIdx) < 0)
                perror("SIOCGIFINDEX");

        return ifIdx.ifr_ifindex;
}

Eth craftFrame (const std::string& dest, const std::string& source)
{   
    Eth m{};

    auto macDest = transformMac(dest);
    auto macSrc = transformMac(source);
    std::copy (macDest.begin(), macDest.end(), m.destMac.begin());
    std::copy (macSrc.begin(), macSrc.end(), m.sourceMac.begin());
    
    m.protocol = {((uint8_t*)(&TESTPROTO_RAW))[1], ((uint8_t*)(&TESTPROTO_RAW))[0]};
    return m;
}
/*
void addNetworkLayer (EthernetMsg& p, const std::string& dest, const std::string& src, const std::string data)
{
    if (data.size() > MAXDATASIZE-1)
        throw std::length_error ("Mensaje demasiado largo");
    NetworkHeader& nh = (NetworkHeader&)p.data;
    memcpy (nh.dest, dest.c_str(), 6);
    memcpy (nh.src, src.c_str(), 6);
    nh.size = data.size()+1;
    strncpy (nh.msg, data.c_str(), data.size());
}

std::vector <unsigned char> transformMac (const std::string& mac)
{
    auto octets = parse (mac, ":");
    
    std::vector <unsigned char> tmp (6);
    
    std::transform (octets.begin(), octets.end(), tmp.begin(), [](const std::string& str) { return std::stoul (str, nullptr, 16); });
    
    return tmp;
}*/

std::array<unsigned char, 6> transformMac (const std::string& mac)
{
    std::array <unsigned char, 6> octets;
    sscanf (mac.c_str(), "%x:%x:%x:%x:%x:%x", &octets[0], &octets[1], &octets[2], &octets[3], &octets[4], &octets[5]);
    return octets;
}


sockaddr_ll makeAddress (const std::string& iface, const std::string& mac, int fd)
{
    sockaddr_ll socket_address;
    socket_address.sll_ifindex = network::getInterfaceIndex (iface, fd);
    socket_address.sll_halen = ETH_ALEN;
    auto macAd = transformMac(mac);
    std::copy(macAd.begin(), macAd.end(), socket_address.sll_addr);
    
    return socket_address;
}

}
