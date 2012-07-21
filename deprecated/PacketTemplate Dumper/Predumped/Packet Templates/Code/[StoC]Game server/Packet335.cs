using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 335)]
        public class Packet335 : IPacket
        {
                public class PacketSt335 : IPacketTemplate
                {
                        public UInt16 Header { get { return 335; } }
                        public UInt32 Data1;
                        public UInt16 Data2;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt335>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt335)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt335> pParser;

        }
}
