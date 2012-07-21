using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 127)]
        public class Packet127 : IPacket
        {
                public class PacketSt127 : IPacketTemplate
                {
                        public UInt16 Header { get { return 127; } }
                        public UInt16 Data1;
                        public UInt16 Data2;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public UInt16[] Data3;
                        public byte Data4;
                        public byte Data5;
                        public byte Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt127>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt127> pParser;
        }
}
