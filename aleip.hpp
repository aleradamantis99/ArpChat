#pragma once

namespace network
{

struct AleIp
{
    uint8_t dest;
    uint8_t src;
};
/*
A   L   E   I P
l   i   l
e   g   e
I   h   g
p   t   a
    e   n
    n   t
    s   l
        y
*/
/*
A   L   E   P
l   o   t   r
e   v   h   o
P   e   e   t
    s   r   o
        n   c
        e   o
        t   l
        
*/
struct AleTP
{
    uint8_t dest;
    uint8_t src;
    uint16_t seq;
};

}//NAMESPACE network
