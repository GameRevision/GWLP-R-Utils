using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 132)]
        public class Packet132 : IPacket
        {
                public class PacketSt132 : IPacketTemplate
                {
                        public UInt16 Header { get { return 132; } }
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data1;
                        [PacketFieldType(ConstSize = true, MaxSize = 8)]
                        public byte[] Data2;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt132>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt132> pParser;
        }
}
