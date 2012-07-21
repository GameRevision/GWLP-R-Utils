using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 106)]
        public class Packet106 : IPacket
        {
                public class PacketSt106 : IPacketTemplate
                {
                        public UInt16 Header { get { return 106; } }
                        public byte Data1;
                        public byte ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 6)]
                        public byte[] Data2;
                        public UInt32 Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt106>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt106> pParser;
        }
}
