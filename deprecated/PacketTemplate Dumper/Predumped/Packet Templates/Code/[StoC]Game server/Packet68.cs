using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 68)]
        public class Packet68 : IPacket
        {
                public class PacketSt68 : IPacketTemplate
                {
                        public UInt16 Header { get { return 68; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public string Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public string Data4;
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public string Data5;
                        public UInt16 Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt68>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt68)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt68> pParser;

        }
}
