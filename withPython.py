#!/usr/bin/python3.7m
from scapy.all import sendp, Ether, Padding

sendp (Ether(dst="6c:c7:ec:75:82:b9", src="3c:a0:67:69:42:c9", type=0x6969)/Padding(load="Paquete"), verbose=0)
