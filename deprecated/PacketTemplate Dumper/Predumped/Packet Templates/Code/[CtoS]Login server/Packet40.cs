using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 40)]
        public class Packet40 : IPacket
        {
                public class PacketSt40 : IPacketTemplate
                {
                        public UInt16 Header { get { return 40; } }
                        public UInt32 Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 28)]
                        public string Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 28)]
                        public string Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data4;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data5;
                        [PacketFieldType(ConstSize = false, MaxSize = 12)]
                        public string Data6;
                        [PacketFieldType(ConstSize = false, MaxSize = 28)]
                        public string Data7;
                        [PacketFieldType(ConstSize = false, MaxSize = 28)]
                        public string Data8;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt40>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt40> pParser;
        }
}
