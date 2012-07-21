using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 87)]
        public class Packet87 : IPacket
        {
                public class PacketSt87 : IPacketTemplate
                {
                        public UInt16 Header { get { return 87; } }
                        public UInt32 ID1;
                        public UInt32 ID2;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt87>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt87)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt87> pParser;

        }
}
