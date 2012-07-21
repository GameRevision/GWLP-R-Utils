using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 366)]
        public class Packet366 : IPacket
        {
                public class PacketSt366 : IPacketTemplate
                {
                        public UInt16 Header { get { return 366; } }
                        public UInt16 Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 6)]
                        public string Data3;
                        public UInt16 Data4;
                        public byte Data5;
                        public byte Data6;
                        public byte Data7;
                        public byte Data8;
                        public UInt16 Data9;
                        public UInt16 Data10;
                        public byte Data11;
                        public byte Data12;
                        public byte Data13;
                        public byte Data14;
                        public UInt16 Data15;
                        public UInt16 Data16;
                        public UInt32 Data17;
                        public UInt32 Data18;
                        public UInt32 Data19;
                        public byte Data20;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt366>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt366)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt366> pParser;

        }
}
