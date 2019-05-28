/*
 * @author: Andreas Hirtenlehner, Constantin Schieber
 * @brief: NS3 Simulation of CN2 Assignment 4
 * @run: copy to `ns3-3.29/scratch/ass4.cc`, `cd ns3-3.29` and `./waf --run ass4`
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"

#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Assignment4");

int 
main (int argc, char *argv[])
{

  bool showPings = true;

  // Allow the user to override any of the defaults and the above
  // Bind ()s at run-time, via command-line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (3);
	
  /*
   * 10.0.20.2        10.0.20.1 <> 10.0.148.1       10.0.148.2
   * +--------+     P2P     +--------+      CSMA    +--------+
   * | Node 1 | ----------- | Node 2 | ------------ | Node 3 |
   * +--------+             +--------+              +--------+
   *            13Mbps                 90Mbs
   * 	        30ms                   2.0ms
   */
  NodeContainer n0n1 = NodeContainer (c.Get (1), c.Get (0));
  NodeContainer n1n2 = NodeContainer (c.Get (1), c.Get (2));

  InternetStackHelper internet;
  internet.Install (c);

  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("13Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("30ms"));
  NetDeviceContainer d0d1 = p2p.Install (n0n1);
  

  // We create the channels first without any IP addressing information
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("90Mbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d1d2 = csma.Install (n1n2);

  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
	
  ipv4.SetBase ("10.0.20.0", "255.255.255.0");
  Ipv4InterfaceContainer iic0 = ipv4.Assign (d0d1);

  ipv4.SetBase ("10.0.148.0", "255.255.255.0");
  Ipv4InterfaceContainer iic1 = ipv4.Assign (d1d2);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  NS_LOG_INFO ("Create Applications.");
  uint32_t packetSize = 1024;
  Time interPacketInterval = Seconds (0.2);
  V4PingHelper ping ("10.0.148.2");

  /* Start and Stop get overwritten in apps.start / apps.stop anyways */
  ping.SetAttribute("StartTime", TimeValue (Seconds (1)));
  ping.SetAttribute("StopTime", TimeValue (Seconds (10)));
  /* Interval of 0.2 seconds */
  ping.SetAttribute ("Interval", TimeValue (interPacketInterval));
  /* Packet Size of 1024 + 28 byte overhead = 1052 bytes total */
  ping.SetAttribute ("Size", UintegerValue (packetSize));

  /* Hide Pings if they are not needed */
  if (showPings)
  {
	  ping.SetAttribute ("Verbose", BooleanValue (true));
  }

  /* Configures who is pinging and in which time frame */
  ApplicationContainer apps = ping.Install (c.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  AsciiTraceHelper ascii;

  /* This prints the routing tables of the nodes */
  Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream ("routingTables.txt");
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  ipv4RoutingHelper.PrintRoutingTableAllAt(Seconds(3), stream);

  /* This prints captured traffic from node 0 to 1 and node 1 to 2 */
  csma.EnableAsciiAll (ascii.CreateFileStream ("ass4-test.tr"));
  csma.EnablePcapAll ("assignment4", true);
  p2p.EnablePcapAll ("assignment4", true);
  

  /* Allows us to draw the network with NetAnim */
  //std::string anim_name ("n-node-ppp.anim.xml");
  //AnimationInterface anim(anim_name.c_str ());
  //anim.EnableIpv4RouteTracking("n-route-tracking.anim.xml", Seconds(1), Seconds(10), Seconds(1));

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();

  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}

