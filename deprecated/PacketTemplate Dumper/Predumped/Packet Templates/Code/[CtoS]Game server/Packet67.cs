using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 67)]
        public class Packet67 : IPacket
        {
                public class PacketSt67 : IPacketTemplate
                {
                        public UInt16 Header { get { return 67; } }
                        public byte Data1;
                        public UInt32 Data2;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data3;
                        public UInt32 Data4;
                        public UInt16 ArraySize2;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt67>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt67> pParser;
        }
}
