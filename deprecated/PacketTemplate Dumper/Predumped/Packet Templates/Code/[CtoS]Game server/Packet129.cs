using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 129)]
        public class Packet129 : IPacket
        {
                public class PacketSt129 : IPacketTemplate
                {
                        public UInt16 Header { get { return 129; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt129>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt129> pParser;
        }
}
