using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 456)]
        public class Packet456 : IPacket
        {
                public class PacketSt456 : IPacketTemplate
                {
                        public UInt16 Header { get { return 456; } }
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public UInt16[] Data1;
                        public byte ArraySize2;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data2;
                        public UInt32 Data4;
                        public UInt32 Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt456>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt456)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt456> pParser;

        }
}
