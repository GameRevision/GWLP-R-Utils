using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 47)]
        public class Packet47 : IPacket
        {
                public class PacketSt47 : IPacketTemplate
                {
                        public UInt16 Header { get { return 47; } }
                        public UInt32 Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 6)]
                        public string Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt47>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt47> pParser;
        }
}
