using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 115)]
        public class Packet115 : IPacket
        {
                public class PacketSt115 : IPacketTemplate
                {
                        public UInt16 Header { get { return 115; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt115>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt115> pParser;
        }
}
