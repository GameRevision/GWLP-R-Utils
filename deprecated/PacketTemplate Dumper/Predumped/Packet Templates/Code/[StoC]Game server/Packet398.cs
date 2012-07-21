using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 398)]
        public class Packet398 : IPacket
        {
                public class PacketSt398 : IPacketTemplate
                {
                        public UInt16 Header { get { return 398; } }
                        public UInt32 Data1;
                        public UInt16 Data2;
                        public UInt16 Data3;
                        public byte Data4;
                        public byte Data5;
                        public byte Data6;
                        public byte ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 2)]
                        public byte[] Data7;
                        public byte Data9;
                        public byte Data10;
                        public byte Data11;
                        public byte Data12;
                        public byte Data13;
                        public byte Data14;
                        public UInt16 Data15;
                        public byte Data16;
                        [PacketFieldType(ConstSize = false, MaxSize = 48)]
                        public string Data17;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt398>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt398)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt398> pParser;

        }
}
