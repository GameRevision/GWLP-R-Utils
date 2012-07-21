using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 130)]
        public class Packet130 : IPacket
        {
                public class PacketSt130 : IPacketTemplate
                {
                        public UInt16 Header { get { return 130; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt130>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt130> pParser;
        }
}
