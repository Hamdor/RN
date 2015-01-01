#!/bin/bash

name=$(/usr/bin/awk 'BEGIN {RS=" ";FS="="}$1=="name"{print $2}' /proc/cmdline )

echo "Init script on router $name started."

hostname $name

echo "1" > /proc/sys/net/ipv4/ip_forward

echo "#no nameserver configured" >/etc/resolv.conf

echo  '127.0.0.1 localhost
172.16.11.1  r1_0
172.16.11.2  r2_0
172.16.12.1  r2_1
172.16.15.1  r2_2
172.16.12.2  r3_0
172.16.12.3  r4_0
172.16.103.2  r4_1
172.16.103.1  r5_0
172.16.106.1  r5_1
172.16.207.1  r5_2
172.16.106.2  r6_0
172.16.15.2  r6_1
172.16.14.1  r6_2
172.16.14.2  r7_0
172.16.207.2  r8_0' >/etc/hosts

echo ' '>/root/.bash_history


ifconfig lo 127.0.0.1/24 up


case $name in
  r1)
    ifconfig eth0 172.16.11.1/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.12.0/24 gw 172.16.11.2
    route add -net 172.16.15.0/24 gw 172.16.11.2
    route add -net 172.16.14.0/24 gw 172.16.11.2
    route add -net 172.16.103.0/24 gw 172.16.11.2
    route add -net 172.16.106.0/24 gw 172.16.11.2

	# Loesche alle vorhandenen Firewall-Eintraege
    iptables -F
    # hier die iptables-Befehle eintragen:
  ;;
  r2)
    ifconfig eth0 172.16.11.2/24 up
    ifconfig eth1 172.16.12.1/24 up
    ifconfig eth2 172.16.15.1/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.14.0/24 gw 172.16.15.2
    route add -net 172.16.103.0/24 gw 172.16.12.3
    route add -net 172.16.106.0/24 gw 172.16.12.3

    # Loesche alle vorhandenen Firewall-Eintraege
    iptables -F
    # hier die iptables-Befehle eintragen:
  ;;
  r3)
    ifconfig eth0 172.16.12.2/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.11.0/24 gw 172.16.12.1
    route add -net 172.16.106.0/24 gw 172.16.12.3
    route add -net 172.16.15.0/24 gw 172.16.12.1
    route add -net 172.16.103.0/24 gw 172.16.12.3
    route add -net 172.16.14.0/24 gw 172.16.12.3

    # Loesche alle vorhandenen Firewall-Eintraege
    iptables -F
    # hier die iptables-Befehle eintragen:
  ;;
  r4)
    ifconfig eth0 172.16.12.3/24 up
    ifconfig eth1 172.16.103.2/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.11.0/24 gw 172.16.12.1
    route add -net 172.16.106.0/24 gw 172.16.103.1
    route add -net 172.16.14.0/24 gw 172.16.103.1

    # Loesche alle vorhandenen Firewall-Eintraege
    iptables -F
    # hier die iptables-Befehle eintragen:
  ;;
  r5)
    ifconfig eth0 172.16.103.1/24 up
    ifconfig eth1 172.16.106.1/24 up
    ifconfig eth2 172.16.207.1/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.12.0/24 gw 172.16.103.2
    route add -net 172.16.11.0/24 gw 172.16.103.2
    route add -net 172.16.14.0/24 gw 172.16.106.2

    # Loesche alle vorhandenen Firewall-Eintraege
    iptables -F
    # hier die iptables-Befehle eintragen:
  ;;
  r6)
    ifconfig eth0 172.16.106.2/24 up
    ifconfig eth1 172.16.15.2/24 up
    ifconfig eth2 172.16.14.1/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.11.0/24 gw 172.16.15.1
    route add -net 172.16.103.0/24 gw 172.16.106.1
    route add -net 172.16.12.0/24 gw 172.16.106.1

    # Loesche alle vorhandenen Firewall-Eintraege
    iptables -F
    # hier die iptables-Befehle eintragen:
  ;;
  r7)
    ifconfig eth0 172.16.14.2/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.15.0/24 gw 172.16.14.1
    route add -net 172.16.11.0/24 gw 172.16.14.1
    route add -net 172.16.12.0/24 gw 172.16.14.1

    # Loesche alle vorhandenen Firewall-Eintraege
    iptables -F
    # hier die iptables-Befehle eintragen:
  ;;
  r8)
    ifconfig eth0 172.16.207.2/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------

    # Loesche alle vorhandenen Firewall-Eintraege
    iptables -F
    # hier die iptables-Befehle eintragen:
  ;;
esac

