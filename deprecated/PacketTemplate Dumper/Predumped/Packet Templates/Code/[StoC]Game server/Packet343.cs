using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 343)]
        public class Packet343 : IPacket
        {
                public class PacketSt343 : IPacketTemplate
                {
                        public UInt16 Header { get { return 343; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public byte Data3;
                        public byte Data4;
                        public UInt16 Data5;
                        public UInt16 Data6;
                        public byte Data7;
                        public UInt32 Data8;
                        public UInt32 Data9;
                        public UInt32 Data10;
                        public UInt32 Data11;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data12;
                        public byte ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 256)]
                        public UInt32[] Data13;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt343>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt343)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt343> pParser;

        }
}
