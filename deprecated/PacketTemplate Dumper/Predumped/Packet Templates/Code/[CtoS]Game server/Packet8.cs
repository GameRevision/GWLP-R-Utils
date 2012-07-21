using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 8)]
        public class Packet8 : IPacket
        {
                public class PacketSt8 : IPacketTemplate
                {
                        public UInt16 Header { get { return 8; } }
                        public UInt32 ID1;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data1;
                        public UInt16 ArraySize2;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data2;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt8>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt8> pParser;
        }
}
