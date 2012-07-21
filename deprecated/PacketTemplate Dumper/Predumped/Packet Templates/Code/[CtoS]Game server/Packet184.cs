using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 184)]
        public class Packet184 : IPacket
        {
                public class PacketSt184 : IPacketTemplate
                {
                        public UInt16 Header { get { return 184; } }
                        public UInt32 Data1;
                        [PacketFieldType(ConstSize = true, MaxSize = 16)]
                        public byte[] Data2;
                        public byte Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt184>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt184> pParser;
        }
}
