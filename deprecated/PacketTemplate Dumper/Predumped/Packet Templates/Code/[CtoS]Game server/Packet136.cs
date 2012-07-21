using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 136)]
        public class Packet136 : IPacket
        {
                public class PacketSt136 : IPacketTemplate
                {
                        public UInt16 Header { get { return 136; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt136>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt136> pParser;
        }
}
