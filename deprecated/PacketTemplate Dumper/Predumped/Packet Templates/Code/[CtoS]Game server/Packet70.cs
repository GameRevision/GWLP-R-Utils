using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 70)]
        public class Packet70 : IPacket
        {
                public class PacketSt70 : IPacketTemplate
                {
                        public UInt16 Header { get { return 70; } }
                        public byte Data1;
                        public UInt32 Data2;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data3;
                        public UInt16 ArraySize2;
                        [PacketFieldType(ConstSize = false, MaxSize = 16)]
                        public byte[] Data4;
                        public UInt32 Data5;
                        public UInt16 ArraySize3;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data6;
                        public UInt16 ArraySize4;
                        [PacketFieldType(ConstSize = false, MaxSize = 16)]
                        public byte[] Data7;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt70>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt70> pParser;
        }
}
