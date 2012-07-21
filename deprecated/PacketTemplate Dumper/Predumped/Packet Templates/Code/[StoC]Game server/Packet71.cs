using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 71)]
        public class Packet71 : IPacket
        {
                public class PacketSt71 : IPacketTemplate
                {
                        public UInt16 Header { get { return 71; } }
                        public UInt32 Data1;
                        public Single Data2;
                        public Single Data3;
                        public UInt16 Data4;
                        public UInt16 Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt71>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt71)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt71> pParser;

        }
}
