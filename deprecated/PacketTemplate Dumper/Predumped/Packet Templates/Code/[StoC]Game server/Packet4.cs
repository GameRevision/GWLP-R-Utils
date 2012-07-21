using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 4)]
        public class Packet4 : IPacket
        {
                public class PacketSt4 : IPacketTemplate
                {
                        public UInt16 Header { get { return 4; } }
                        public UInt16 Data1;
                        public UInt16 Data2;
                        public UInt16 Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt4>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt4)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt4> pParser;

        }
}
