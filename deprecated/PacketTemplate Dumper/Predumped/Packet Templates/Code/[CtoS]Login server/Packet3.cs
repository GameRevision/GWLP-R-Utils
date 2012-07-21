using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 3)]
        public class Packet3 : IPacket
        {
                public class PacketSt3 : IPacketTemplate
                {
                        public UInt16 Header { get { return 3; } }
                        public UInt32 Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data2;
                        [PacketFieldType(ConstSize = true, MaxSize = 20)]
                        public byte[] Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 26)]
                        public string Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt3>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt3> pParser;
        }
}
