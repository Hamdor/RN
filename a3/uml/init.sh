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
iptables -F

case $name in
  r1)
    ifconfig eth0 172.16.11.1/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add         default      gw 172.16.11.2
  ;;
  r2)
    ifconfig eth0 172.16.11.2/24 up
    ifconfig eth1 172.16.12.1/24 up
    ifconfig eth2 172.16.15.1/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add         default      gw 172.16.12.3
    route add -net 172.16.14.0/24  gw 172.16.15.2
    route add -net 172.16.103.0/24 gw 172.16.12.3
    route add -net 172.16.106.0/24 gw 172.16.12.3

    ## Default Policy DROP
    iptables -P INPUT   DROP
    iptables -P OUTPUT  DROP
    iptables -P FORWARD DROP
    #################################################################
    ## Ping Pakete nicht dropen, wenn die Pakete hier angekommen   ##
    ## sind, dann kamen diese nicht aus dem Internet               ##
    #################################################################
    ## Ping senden
    iptables -A INPUT   -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A OUTPUT  -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A FORWARD -p icmp --icmp-type echo-request -j ACCEPT
    ## Ping antwort
    iptables -A INPUT   -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A OUTPUT  -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A FORWARD -p icmp --icmp-type echo-reply -j ACCEPT
    #################################################################
    ## SSH von R1 sowie R7 auf R3 (DMZ) erlauben                   ##
    #################################################################
    ## R1
    iptables -A FORWARD -p tcp -s 172.16.11.1 -d 172.16.12.2 --sport 513:65535 --dport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
    iptables -A FORWARD -p tcp -s 172.16.12.2 -d 172.16.11.1 --sport 22 --dport 513:65535 -m state --state ESTABLISHED -j ACCEPT
    ## R7
    iptables -A FORWARD -p tcp -s 172.16.14.2 -d 172.16.12.2 --sport 513:65535 --dport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
    iptables -A FORWARD -p tcp -s 172.16.12.2 -d 172.16.14.2 --sport 22 --dport 513:65535 -m state --state ESTABLISHED -j ACCEPT
  ;;
  r3)
    ifconfig eth0 172.16.12.2/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.11.0/24  gw 172.16.12.1
    route add -net 172.16.14.0/24  gw 172.16.12.1
    route add -net 172.16.15.0/24  gw 172.16.12.1
    route add -net 172.16.103.0/24 gw 172.16.12.3
    route add -net 172.16.106.0/24 gw 172.16.12.3
    route add         default      gw 172.16.12.3
  ;;
  r4)
    ifconfig eth0 172.16.12.3/24 up
    ifconfig eth1 172.16.103.2/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add         default      gw 172.16.103.1
    route add -net 172.16.11.0/24  gw 172.16.12.1
    route add -net 172.16.14.0/24  gw 172.16.12.1
    route add -net 172.16.15.0/24  gw 172.16.12.1
    route add -net 172.16.106.0/24 gw 172.16.103.1
    ## Default Policy DROP
    iptables -P INPUT   DROP
    iptables -P OUTPUT  DROP
    iptables -P FORWARD DROP
    #################################################################
    ## Ping Pakete nicht dropen, wenn die Pakete hier angekommen   ##
    ## sind, dann kamen diese nicht aus dem Internet               ##
    #################################################################
    ## Ping senden
    iptables -A INPUT   -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A OUTPUT  -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A FORWARD -p icmp --icmp-type echo-request -j ACCEPT
    ## Ping antwort
    iptables -A INPUT   -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A OUTPUT  -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A FORWARD -p icmp --icmp-type echo-reply -j ACCEPT
  ;;
  r5)
    ifconfig eth0 172.16.103.1/24 up
    ifconfig eth1 172.16.106.1/24 up
    ifconfig eth2 172.16.207.1/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.11.0/24 gw 172.16.103.2
    route add -net 172.16.12.0/24 gw 172.16.103.2
    route add -net 172.16.14.0/24 gw 172.16.106.2
    route add -net 172.16.15.0/24 gw 172.16.106.2
    ## Default Policy DROP
    iptables -P INPUT   DROP
    iptables -P OUTPUT  DROP
    iptables -P FORWARD DROP
    #################################################################
    ## Pings von internen Netzwerken erlauben                      ##
    ## Eingehende Pings werden geblockt                            ##
    #################################################################
    ## Ping senden
    iptables -A INPUT   -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A OUTPUT  -p icmp --icmp-type echo-request -j ACCEPT
    ## Ping antwort
    iptables -A INPUT   -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A OUTPUT  -p icmp --icmp-type echo-reply -j ACCEPT
    ## Filter fuer Internes Netz / Internet Pings
    iptables -A FORWARD -i eth0 -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A FORWARD -i eth0 -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A FORWARD -i eth1 -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A FORWARD -i eth1 -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A FORWARD -i eth2 -p icmp --icmp-type echo-reply -j ACCEPT
  ;;
  r6)
    ifconfig eth0 172.16.106.2/24 up
    ifconfig eth1 172.16.15.2/24 up
    ifconfig eth2 172.16.14.1/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add -net 172.16.11.0/24 gw 172.16.15.1
    route add -net 172.16.12.0/24 gw 172.16.15.1
    route add -net 172.16.103.0/24 gw 172.16.106.1
    route add         default      gw 172.16.106.1
    ## Default Policy DROP
    iptables -P INPUT   DROP
    iptables -P OUTPUT  DROP
    iptables -P FORWARD DROP
    #################################################################
    ## Ping Pakete nicht dropen, wenn die Pakete hier angekommen   ##
    ## sind, dann kamen diese nicht aus dem Internet               ##
    #################################################################
    ## Ping senden
    iptables -A INPUT   -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A OUTPUT  -p icmp --icmp-type echo-request -j ACCEPT
    iptables -A FORWARD -p icmp --icmp-type echo-request -j ACCEPT
    ## Ping antwort
    iptables -A INPUT   -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A OUTPUT  -p icmp --icmp-type echo-reply -j ACCEPT
    iptables -A FORWARD -p icmp --icmp-type echo-reply -j ACCEPT
    #################################################################
    ## SSH von R1 sowie R7 auf R3 (DMZ) erlauben                   ##
    #################################################################
    ## R7
    iptables -A FORWARD -p tcp -s 172.16.14.2 -d 172.16.12.2 --sport 513:65535 --dport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
    iptables -A FORWARD -p tcp -s 172.16.12.2 -d 172.16.14.2 --sport 22 --dport 513:65535 -m state --state ESTABLISHED -j ACCEPT
    #################################################################
    ## NAT                                                         ##
    #################################################################
    iptables -t nat -A POSTROUTING -o eth0 -i eth2 -j MASQUERADE
  ;;
  r7)
    ifconfig eth0 172.16.14.2/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add         default      gw 172.16.14.1
  ;;
  r8)
    ifconfig eth0 172.16.207.2/24 up
    # Routing   |     Zielnetz     |   Gateway
    #-----------+------------------+-------------
    route add         default      gw 172.16.207.1
  ;;
esac
#################################################################
## Verbindungen die schon aufgebaut sind akzeptieren           ##
#################################################################
iptables -A INPUT   -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A OUTPUT  -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A FORWARD -m state --state ESTABLISHED,RELATED -j ACCEPT

#################################################################
## Ausgehende HTTP/FTP Verbindungen                            ##
#################################################################
iptables -A OUTPUT -m state --state NEW -p tcp --syn --dport 20 -j ACCEPT
iptables -A OUTPUT -m state --state NEW -p tcp --syn --dport 21 -j ACCEPT
iptables -A OUTPUT -m state --state NEW -p tcp --syn --dport 80 -j ACCEPT

#################################################################
## HTTP/FTP Forwarding                                         ##
#################################################################
iptables -A FORWARD -m state --state NEW -p tcp --syn --dport 20 -j ACCEPT
iptables -A FORWARD -m state --state NEW -p tcp --syn --dport 21 -j ACCEPT
iptables -A FORWARD -m state --state NEW -p tcp --syn --dport 80 -j ACCEPT
