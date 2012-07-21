using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 31)]
        public class Packet31 : IPacket
        {
                public class PacketSt31 : IPacketTemplate
                {
                        public UInt16 Header { get { return 31; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public UInt32 Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data7;
                        [PacketFieldType(ConstSize = false, MaxSize = 16)]
                        public string Data8;
                        [PacketFieldType(ConstSize = false, MaxSize = 256)]
                        public string Data9;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data10;
                        [PacketFieldType(ConstSize = false, MaxSize = 28)]
                        public string Data11;
                        [PacketFieldType(ConstSize = false, MaxSize = 28)]
                        public string Data12;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data13;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data14;
                        [PacketFieldType(ConstSize = false, MaxSize = 28)]
                        public string Data15;
                        [PacketFieldType(ConstSize = false, MaxSize = 4)]
                        public string Data16;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data17;
                        [PacketFieldType(ConstSize = false, MaxSize = 4)]
                        public string Data18;
                        [PacketFieldType(ConstSize = false, MaxSize = 28)]
                        public string Data19;
                        [PacketFieldType(ConstSize = false, MaxSize = 12)]
                        public string Data20;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data21;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data22;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt31>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt31> pParser;
        }
}
