using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 206)]
        public class Packet206 : IPacket
        {
                public class PacketSt206 : IPacketTemplate
                {
                        public UInt16 Header { get { return 206; } }
                        public UInt32 ID1;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public UInt32[] Data1;
                        public UInt16 ArraySize2;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public UInt32[] Data2;
                        public byte Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt206>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt206)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt206> pParser;

        }
}
