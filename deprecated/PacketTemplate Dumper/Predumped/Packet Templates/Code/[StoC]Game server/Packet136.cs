using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 136)]
        public class Packet136 : IPacket
        {
                public class PacketSt136 : IPacketTemplate
                {
                        public UInt16 Header { get { return 136; } }
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 128)]
                        public UInt32[] Data1;
                        public UInt16 ArraySize2;
                        [PacketFieldType(ConstSize = false, MaxSize = 128)]
                        public UInt32[] Data2;
                        public UInt16 ArraySize3;
                        [PacketFieldType(ConstSize = false, MaxSize = 128)]
                        public UInt32[] Data3;
                        public UInt16 ArraySize4;
                        [PacketFieldType(ConstSize = false, MaxSize = 128)]
                        public UInt32[] Data4;
                        public UInt16 ArraySize5;
                        [PacketFieldType(ConstSize = false, MaxSize = 128)]
                        public UInt32[] Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt136>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt136)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt136> pParser;

        }
}
