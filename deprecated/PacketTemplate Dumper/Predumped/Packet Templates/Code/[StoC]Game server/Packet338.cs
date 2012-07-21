using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 338)]
        public class Packet338 : IPacket
        {
                public class PacketSt338 : IPacketTemplate
                {
                        public UInt16 Header { get { return 338; } }
                        public UInt16 Data1;
                        public byte ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 7)]
                        public byte[] Data2;
                        public UInt16 Data4;
                        public byte Data5;
                        public UInt16 Data6;
                        public UInt16 Data7;
                        public UInt16 Data8;
                        public UInt16 Data9;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt338>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt338)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt338> pParser;

        }
}
