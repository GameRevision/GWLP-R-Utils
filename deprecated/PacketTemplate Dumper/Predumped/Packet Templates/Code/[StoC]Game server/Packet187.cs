using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 187)]
        public class Packet187 : IPacket
        {
                public class PacketSt187 : IPacketTemplate
                {
                        public UInt16 Header { get { return 187; } }
                        public byte Data1;
                        public UInt32 Data2;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt187>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt187)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt187> pParser;

        }
}
