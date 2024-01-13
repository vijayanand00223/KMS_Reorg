/**
 * @file net_config.h
 * @brief CycloneTCP configuration file
 *
 * @section License
 *
 * Copyright (C) 2010-2015 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.6.1
 **/

#ifndef _NET_CONFIG_H
#define _NET_CONFIG_H

//Trace level for TCP/IP stack debugging
#define MEM_TRACE_LEVEL          2
#define NIC_TRACE_LEVEL          2
#define ETH_TRACE_LEVEL          2
#define ARP_TRACE_LEVEL          2
#define IP_TRACE_LEVEL           2
#define IPV4_TRACE_LEVEL         2
#define IPV6_TRACE_LEVEL         2
#define ICMP_TRACE_LEVEL         2
#define IGMP_TRACE_LEVEL         2
#define ICMPV6_TRACE_LEVEL       2
#define MLD_TRACE_LEVEL          2
#define NDP_TRACE_LEVEL          2
#define UDP_TRACE_LEVEL          2
#define TCP_TRACE_LEVEL          2
#define SOCKET_TRACE_LEVEL       2
#define RAW_SOCKET_TRACE_LEVEL   2
#define BSD_SOCKET_TRACE_LEVEL   2
#define SLAAC_TRACE_LEVEL        2
#define DHCP_TRACE_LEVEL         2
#define DHCPV6_TRACE_LEVEL       2
#define DNS_TRACE_LEVEL          2
#define MDNS_TRACE_LEVEL         2
#define NBNS_TRACE_LEVEL         0
#define LLMNR_TRACE_LEVEL        2
#define FTP_TRACE_LEVEL          2
#define HTTP_TRACE_LEVEL         2
#define SMTP_TRACE_LEVEL         2
#define SNMP_TRACE_LEVEL         2
#define SNTP_TRACE_LEVEL         2
#define STD_SERVICES_TRACE_LEVEL 2/*
//Trace level for TCP/IP stack debugging
#define MEM_TRACE_LEVEL          2
#define NIC_TRACE_LEVEL          2
#define ETH_TRACE_LEVEL          2
#define ARP_TRACE_LEVEL          2
#define IP_TRACE_LEVEL           2
#define IPV4_TRACE_LEVEL         2
#define IPV6_TRACE_LEVEL         4
#define ICMP_TRACE_LEVEL         2
#define IGMP_TRACE_LEVEL         2
#define ICMPV6_TRACE_LEVEL       2
#define MLD_TRACE_LEVEL          2
#define NDP_TRACE_LEVEL          2
#define UDP_TRACE_LEVEL          2
#define TCP_TRACE_LEVEL          2
#define SOCKET_TRACE_LEVEL       2
#define RAW_SOCKET_TRACE_LEVEL   2
#define BSD_SOCKET_TRACE_LEVEL   2
#define SLAAC_TRACE_LEVEL        2
#define DHCP_TRACE_LEVEL         2
#define DHCPV6_TRACE_LEVEL       4
#define DNS_TRACE_LEVEL          2
#define MDNS_TRACE_LEVEL         2
#define NBNS_TRACE_LEVEL         0
#define LLMNR_TRACE_LEVEL        2
#define FTP_TRACE_LEVEL          2
#define HTTP_TRACE_LEVEL         4
#define SMTP_TRACE_LEVEL         2
#define SNMP_TRACE_LEVEL         2
#define SNTP_TRACE_LEVEL         2
#define STD_SERVICES_TRACE_LEVEL 2*/

//RTOS support
#define NET_RTOS_SUPPORT DISABLED

//Number of network adapters
#define NET_INTERFACE_COUNT 1

//Maximum size of the MAC filter table
#define MAC_FILTER_MAX_SIZE 8

//IPv4 support
#define IPV4_SUPPORT ENABLED
#define MDNS_RESPONDER_SUPPORT ENABLED
//Maximum size of the IPv4 filter table
#define IPV4_FILTER_MAX_SIZE 4

//IPv4 fragmentation support
#define IPV4_FRAG_SUPPORT ENABLED
//Maximum number of fragmented packets the host will accept
//and hold in the reassembly queue simultaneously
#define IPV4_MAX_FRAG_DATAGRAMS 2
//Maximum datagram size the host will accept when reassembling fragments
#define IPV4_MAX_FRAG_DATAGRAM_SIZE 2048

//Size of ARP cache
#define ARP_CACHE_SIZE 8
//Maximum number of packets waiting for address resolution to complete
#define ARP_MAX_PENDING_PACKETS 2

//IGMP support
#define IGMP_SUPPORT DISABLED

//IPv6 support
#define IPV6_SUPPORT DISABLED
//Maximum size of the IPv6 filter table
#define IPV6_FILTER_MAX_SIZE 8

//IPv6 fragmentation support
#define IPV6_FRAG_SUPPORT DISABLED
//Maximum number of fragmented packets the host will accept
//and hold in the reassembly queue simultaneously
#define IPV6_MAX_FRAG_DATAGRAMS 2
//Maximum datagram size the host will accept when reassembling fragments
#define IPV6_MAX_FRAG_DATAGRAM_SIZE 2048

//MLD support
#define MLD_SUPPORT DISABLED

//Neighbor cache size
#define NDP_CACHE_SIZE 8
//Maximum number of packets waiting for address resolution to complete
#define NDP_MAX_PENDING_PACKETS 2

//TCP support
#define TCP_SUPPORT ENABLED
//Default buffer size for transmission
#define TCP_DEFAULT_TX_BUFFER_SIZE (2048)
//Default buffer size for reception
#define TCP_DEFAULT_RX_BUFFER_SIZE (2048)
//Default SYN queue size for listening sockets
#define TCP_DEFAULT_SYN_QUEUE_SIZE 4
//Maximum number of retransmissions
#define TCP_MAX_RETRIES 5
//Selective acknowledgment support
#define TCP_SACK_SUPPORT DISABLED

//UDP support
#define UDP_SUPPORT ENABLED
//Receive queue depth for connectionless sockets
#define UDP_RX_QUEUE_SIZE 4

//Raw socket support
#define RAW_SOCKET_SUPPORT DISABLED
//Receive queue depth for raw sockets
#define RAW_SOCKET_RX_QUEUE_SIZE 4

//Number of sockets that can be opened simultaneously
#define SOCKET_MAX_COUNT 4

//Size of DNS cache
#define DNS_CACHE_SIZE 4

//SNMP agent support
#define SNMP_AGENT_SUPPORT DISABLED
//MIB-II module support
#define MIB2_SUPPORT DISABLED

//DHCPv6 client support
#define DHCPV6_CLIENT_SUPPORT DISABLED

#endif
