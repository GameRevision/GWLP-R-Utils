using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 11)]
        public class Packet11 : IPacket
        {
                public class PacketSt11 : IPacketTemplate
                {
                        public UInt16 Header { get { return 11; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt11>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt11> pParser;
        }
}
