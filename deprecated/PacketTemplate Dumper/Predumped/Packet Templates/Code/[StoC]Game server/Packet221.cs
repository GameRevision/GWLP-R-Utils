using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 221)]
        public class Packet221 : IPacket
        {
                public class PacketSt221 : IPacketTemplate
                {
                        public UInt16 Header { get { return 221; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public UInt32 Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public UInt32 Data7;
                        public UInt32 Data8;
                        public UInt32 Data9;
                        public UInt32 Data10;
                        public UInt32 Data11;
                        public UInt32 Data12;
                        public UInt32 Data13;
                        public UInt32 Data14;
                        public UInt32 Data15;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt221>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt221)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt221> pParser;

        }
}
