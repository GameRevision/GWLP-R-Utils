using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 279)]
        public class Packet279 : IPacket
        {
                public class PacketSt279 : IPacketTemplate
                {
                        public UInt16 Header { get { return 279; } }
                        public UInt16 Data1;
                        [PacketFieldType(ConstSize = true, MaxSize = 16)]
                        public byte[] Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 6)]
                        public string Data4;
                        public byte Data5;
                        public byte Data6;
                        public byte Data7;
                        public byte Data8;
                        public byte Data9;
                        public byte Data10;
                        public byte Data11;
                        public UInt16 Data12;
                        public byte Data13;
                        public byte Data14;
                        public UInt32 Data15;
                        public UInt32 Data16;
                        public UInt32 Data17;
                        public UInt32 Data18;
                        public byte Data19;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt279>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt279)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt279> pParser;

        }
}
