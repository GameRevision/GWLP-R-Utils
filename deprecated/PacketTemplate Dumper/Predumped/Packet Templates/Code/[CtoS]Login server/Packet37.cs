using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 37)]
        public class Packet37 : IPacket
        {
                public class PacketSt37 : IPacketTemplate
                {
                        public UInt16 Header { get { return 37; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public UInt32 Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        [PacketFieldType(ConstSize = true, MaxSize = 16)]
                        public byte[] Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt37>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt37> pParser;
        }
}
