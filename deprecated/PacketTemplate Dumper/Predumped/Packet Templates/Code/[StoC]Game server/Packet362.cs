using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 362)]
        public class Packet362 : IPacket
        {
                public class PacketSt362 : IPacketTemplate
                {
                        public UInt16 Header { get { return 362; } }
                        public UInt32 Data1;
                        public byte Data2;
                        public UInt32 Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public UInt32 Data7;
                        public UInt32 Data8;
                        public byte Data9;
                        public byte Data10;
                        public byte Data11;
                        public byte Data12;
                        public byte Data13;
                        public UInt16 Data14;
                        public UInt16 Data15;
                        public UInt16 Data16;
                        public UInt16 Data17;
                        public UInt16 Data18;
                        public UInt32 Data19;
                        public byte Data20;
                        public byte Data21;
                        public UInt32 Data22;
                        public byte Data23;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt362>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt362)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt362> pParser;

        }
}
