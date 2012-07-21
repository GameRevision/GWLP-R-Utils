using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 415)]
        public class Packet415 : IPacket
        {
                public class PacketSt415 : IPacketTemplate
                {
                        public UInt16 Header { get { return 415; } }
                        public UInt16 Data1;
                        public byte Data2;
                        public byte Data3;
                        public byte Data4;
                        public byte Data5;
                        public byte Data6;
                        public UInt16 Data7;
                        public byte Data8;
                        public UInt32 Data9;
                        public UInt32 Data10;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data11;
                        [PacketFieldType(ConstSize = false, MaxSize = 6)]
                        public string Data12;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt415>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt415)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt415> pParser;

        }
}
