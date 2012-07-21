using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 59)]
        public class Packet59 : IPacket
        {
                public class PacketSt59 : IPacketTemplate
                {
                        public UInt16 Header { get { return 59; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public Single Data3;
                        public Single Data4;
                        public UInt16 Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt59>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt59)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt59> pParser;

        }
}
