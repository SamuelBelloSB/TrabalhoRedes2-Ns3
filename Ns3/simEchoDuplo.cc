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
#include "ns3/ipv4-global-routing-helper.h"

// Essa é uma simulação criada pelo grupo ET-NET através da modificação da simulação first.cc
// Topologia:
//
//               10.1.1.0            10.2.1.0
// n0(servidor)---------------- n1--n2(cliente)---n3(cliente)
//    Topologia ponta-a-ponta   / / / / / / / / / / / / / / / 
//                              ==============================
//                                     Topolgia CSMA


/*O que acontece nessa simulação?
 * São criadas duas topologias, a primeira é uma rede ponta-a-ponta entre os nós n0 e n1,
 * a segunda é uma rede CSMA englobando os nós n1, n2 e n3.
 * Os clientes n2(10.2.1.2) e n3(10.2.1.3) enviarão pacotes Echo para o servidor n0(10.1.1.1);
 * o nó n1 fará apenas o papel de um roteador/switch, intermediando a conexão entre os demais 
 * dispositivos.
 * Ao receber cada pacote UDP, o servidor n0, no extremo da rede ponta-a-ponta, irá responder
 * ecoando o pacote de volta para cada transmissor. A simulação termina quando cada emissor tiver
 * recebido seu pacote Echo de volta.
 * O dispositivo CSMA do nó n1 ficará em modo promiscuo, captando todos os pacotes que passarem
 * por ele, para gerar uma leitura pcap contendo toda a movimentação que ocorreu na rede CSMA.
 * Também serão geradas leituras pcap para os dois nós da rede ponta-a-ponta 
 * (n0 e n1 em seu dispositivo point-to-point) permitindo uma leitura completa do tráfego
 * nessa simulação.
*/  
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  

  //Nodes da topologia point-to-point(n0 e n1)
  NodeContainer nodes;
  nodes.Create (2);

  //Nodes da topologia CSMA(n1, n2 e n3)
  NodeContainer csmaNodes;
  csmaNodes.Add(nodes.Get(1));
  csmaNodes.Create(2);

  //Ligando Nodes point-to-point
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  //Ligando Nodes CSMA
  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
  csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

  //Instalando devices nos nós point-to-point
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  //Instalando devices nos nós CSMA
  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install(csmaNodes);

  //Instalando pilha de protocolos nos nós das duas topologias
  InternetStackHelper stack;
  stack.Install (nodes.Get(0));
  stack.Install (csmaNodes);

  //Setando os endereços IP para os nós de ambas as subredes
  Ipv4AddressHelper address;

  //Setando endereços IP para a subrede ponta a ponta
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces;
  interfaces = address.Assign (devices);

  //Setando endereços IP para a subrede CSMA

  address.SetBase("10.2.1.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign(csmaDevices);


  //Instalando a aplicação servidor
  //Ela será instalada no nó mais a esquerda da topologia point-to-point(n0)
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //Declarando e setando atributos para a aplicação cliente
  //Ela será instalada em dois nós(n2 e n3)
  UdpEchoClientHelper echoClient (interfaces.GetAddress(0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  //Instalando a aplicação cliente em n2
  ApplicationContainer clientApps = echoClient.Install (csmaNodes.Get(1));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  //Instalando a aplicação cliente em n3
  ApplicationContainer clientApps2 = echoClient.Install(csmaNodes.Get(2));
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (10.0));

  /* Usamos esse Helper para fazer toda a configuração de roteamento sem ter que configurar
   * nenhum roteador.
  */
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  //Habilitando a geração de arquivos pcap;
  //Formato em que os arquivos pcap saem: 
  //<<nome_do_arquivo>>-<<nó_da_leitura>>-<<dispositivo_da_leitura>>.pcap

  pointToPoint.EnablePcapAll("simEchoDuplo");
  csma.EnablePcap("simEchoDuplo", csmaDevices.Get(0), true);

  //Iniciando o simulador e o destruindo quando a simulação termina
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
