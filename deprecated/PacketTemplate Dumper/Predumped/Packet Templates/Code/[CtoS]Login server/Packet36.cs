using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 36)]
        public class Packet36 : IPacket
        {
                public class PacketSt36 : IPacketTemplate
                {
                        public UInt16 Header { get { return 36; } }
                        public UInt32 Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 16)]
                        public string Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt36>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt36> pParser;
        }
}
