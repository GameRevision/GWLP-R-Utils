using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 449)]
        public class Packet449 : IPacket
        {
                public class PacketSt449 : IPacketTemplate
                {
                        public UInt16 Header { get { return 449; } }
                        public UInt16 Data1;
                        public UInt16 Data2;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt449>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt449)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt449> pParser;

        }
}
