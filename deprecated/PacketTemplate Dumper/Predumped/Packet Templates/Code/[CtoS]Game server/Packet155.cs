using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 155)]
        public class Packet155 : IPacket
        {
                public class PacketSt155 : IPacketTemplate
                {
                        public UInt16 Header { get { return 155; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt155>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt155> pParser;
        }
}
