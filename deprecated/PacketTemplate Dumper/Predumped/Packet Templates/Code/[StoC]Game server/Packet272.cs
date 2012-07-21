using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 272)]
        public class Packet272 : IPacket
        {
                public class PacketSt272 : IPacketTemplate
                {
                        public UInt16 Header { get { return 272; } }
                        public UInt16 Data1;
                        public byte Data2;
                        public byte Data3;
                        public UInt32 Data4;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data5;
                        [PacketFieldType(ConstSize = false, MaxSize = 6)]
                        public string Data6;
                        public byte Data7;
                        public byte Data8;
                        public byte Data9;
                        public byte Data10;
                        public byte Data11;
                        public UInt16 Data12;
                        public byte Data13;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt272>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt272)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt272> pParser;

        }
}
