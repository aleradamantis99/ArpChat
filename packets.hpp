#pragma once
#include <type_traits>
#include <tuple>
#include <cstddef>
#pragma pack(push, 1)


namespace network
{

const size_t HW_LENGTH = 10;
const size_t PROT_LENGTH = 10;


template <size_t S>
struct Data
{
    const static uint8_t layer = -1;
};

template <typename T, typename... Args>
struct packet
{
    static_assert (T::layer <= std::tuple_element<0, std::tuple<Args...>>::type::layer);
    
    T header;
    packet<Args...> payload;
};

template <typename T>
struct packet<T>
{
    T header;
};

template <size_t S>
struct packet<Data<S>>
{
    std::array<uint8_t, S> payload;
};

template <typename T2, typename T, typename... Args, std::enable_if_t<std::is_same_v<T,T2>>* = nullptr>
auto& get (const packet<T, Args...>& p)
{
    return p;
}

template <typename T2, typename T, typename... Args, std::enable_if_t<!std::is_same_v<T,T2>>* = nullptr>
auto& get (const packet<T, Args...>& p)
{
    return get<T2>(p.payload);
}

template <typename T2, typename T, typename... Args, std::enable_if_t<std::is_same_v<T,T2>>* = nullptr>
auto& get (packet<T, Args...>& p)
{
    return p;
}

template <typename T2, typename T, typename... Args, std::enable_if_t<!std::is_same_v<T,T2>>* = nullptr>
auto& get (packet<T, Args...>& p)
{
    return get<T2>(p.payload);
}
/*
template <typename T2, typename T>
auto& get (const packet<T>& p)
{
    static_assert(std::is_same_v<T,T2>, "No se encontro el protocolo");
    return p;
}
*/
struct Eth
{
    const static uint8_t layer = 2;
    std::array<uint8_t, 6> destMac;
    std::array<uint8_t, 6> sourceMac;
    std::array<uint8_t, 2> protocol;
};

struct Arp
{
    const static uint8_t layer = 2;
    uint16_t hardwareType;
    uint16_t protocolType;
    uint8_t hardwareLen;
    uint8_t protocolLen;
    uint16_t opcode;
    std::array<uint8_t, HW_LENGTH> senderHwAddress;
    std::array<uint8_t, PROT_LENGTH> senderProtAddress;
    std::array<uint8_t, HW_LENGTH> targerHwAddress;
    std::array<uint8_t, PROT_LENGTH> targetProtAddress;
};

} //NAMESPACE network
