using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 113)]
        public class Packet113 : IPacket
        {
                public class PacketSt113 : IPacketTemplate
                {
                        public UInt16 Header { get { return 113; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt113>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt113> pParser;
        }
}
