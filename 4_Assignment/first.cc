/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
 #include "ns3/csma-module.h"
 #include "ns3/applications-module.h"
 #include "ns3/internet-apps-module.h"
 #include "ns3/internet-module.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

 static void SinkRx (Ptr<const Packet> p, const Address &ad)
 {
   //std::cout << *p << std::endl;
 }

 static void PingRtt (std::string context, Time rtt)
 {
   //std::cout << context << " " << rtt << std::endl;
 }


int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);


  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (1);
	
  NS_LOG_INFO("TEST");
  /* Point to Point */
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("13Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("30ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  /* CSMA */
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("90Mbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  /* Internet Stack */

  InternetStackHelper stack;
  stack.Install (p2pNodes.Get (0));
  stack.Install (csmaNodes);

  /* Set IP Addresses for P2P */
  Ipv4AddressHelper address;
  address.SetBase ("10.0.20.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  /* Set IP Addresses for CSMA */
  address.SetBase ("10.0.148.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  /* Application Test Setup for PING */
  NS_LOG_INFO ("Create Source");
  Config::SetDefault ("ns3::Ipv4RawSocketImpl::Protocol", StringValue ("1"));
  InetSocketAddress dst = InetSocketAddress (p2pInterfaces.GetAddress (1));
  OnOffHelper onoff = OnOffHelper ("ns3::Ipv4RawSocketFactory", dst);
  onoff.SetConstantRate (DataRate (15000));
  onoff.SetAttribute ("PacketSize", UintegerValue (1200));
 
 
  ApplicationContainer apps = onoff.Install (p2pNodes.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));
 
  NS_LOG_INFO ("Create Sink.");
  PacketSinkHelper sink = PacketSinkHelper ("ns3::Ipv4RawSocketFactory", dst);
  apps = sink.Install (csmaNodes.Get (1));
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (11.0));
 
  NS_LOG_INFO ("Create pinger");
  V4PingHelper ping = V4PingHelper (p2pInterfaces.GetAddress (0));
  NodeContainer pingers;
  pingers.Add (p2pNodes.Get (0));
  //pingers.Add (p2pNodes.Get (1));
  //pingers.Add (csmaNodes.Get (0));
  //pingers.Add (p2pNodes.Get (2));
  apps = ping.Install (pingers);
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (5.0));
 
  NS_LOG_INFO ("Configure Tracing.");
  // first, pcap tracing in non-promiscuous mode
  csma.EnablePcapAll ("csma-ping", false);
 
  // then, print what the packet sink receives.
  Config::ConnectWithoutContext ("/NodeList/3/ApplicationList/0/$ns3::PacketSink/Rx", 
                                 MakeCallback (&SinkRx));
  // finally, print the ping rtts.
  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::V4Ping/Rtt",
                   MakeCallback (&PingRtt));
 
  Packet::EnablePrinting ();
 
 
  NS_LOG_INFO ("Run Simulation.");

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
