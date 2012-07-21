using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 123)]
        public class Packet123 : IPacket
        {
                public class PacketSt123 : IPacketTemplate
                {
                        public UInt16 Header { get { return 123; } }
                        public byte Data1;
                        public UInt32 Data2;
                        public UInt32 Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public UInt32 Data7;
                        public UInt32 Data8;
                        public UInt16 Data9;
                        public UInt16 Data10;
                        public UInt16 Data11;
                        public UInt16 Data12;
                        public UInt16 Data13;
                        public byte Data14;
                        public byte Data15;
                        public byte Data16;
                        public byte Data17;
                        public byte Data18;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt123>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt123)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt123> pParser;

        }
}
