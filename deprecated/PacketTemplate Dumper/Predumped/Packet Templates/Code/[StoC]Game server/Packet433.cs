using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 433)]
        public class Packet433 : IPacket
        {
                public class PacketSt433 : IPacketTemplate
                {
                        public UInt16 Header { get { return 433; } }
                        public UInt16 Data1;
                        public UInt16 Data2;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt433>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt433)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt433> pParser;

        }
}
