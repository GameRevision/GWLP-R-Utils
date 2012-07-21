using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 133)]
        public class Packet133 : IPacket
        {
                public class PacketSt133 : IPacketTemplate
                {
                        public UInt16 Header { get { return 133; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt133>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt133> pParser;
        }
}
