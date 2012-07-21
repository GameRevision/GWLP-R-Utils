using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 104)]
        public class Packet104 : IPacket
        {
                public class PacketSt104 : IPacketTemplate
                {
                        public UInt16 Header { get { return 104; } }
                        public UInt16 Data1;
                        public byte Data2;
                        public byte Data3;
                        public byte Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public byte Data7;
                        public byte Data8;
                        public UInt32 Data9;
                        public UInt32 Data10;
                        public UInt32 Data11;
                        public UInt32 Data12;
                        public UInt32 Data13;
                        public UInt32 Data14;
                        public UInt32 Data15;
                        public UInt32 Data16;
                        public UInt32 Data17;
                        public UInt32 Data18;
                        public UInt32 Data19;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data20;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt104>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt104)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt104> pParser;

        }
}
