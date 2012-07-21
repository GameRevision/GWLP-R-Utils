using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 43)]
        public class Packet43 : IPacket
        {
                public class PacketSt43 : IPacketTemplate
                {
                        public UInt16 Header { get { return 43; } }
                        public UInt32 Data1;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 512)]
                        public UInt32[] Data2;
                        public UInt16 ArraySize2;
                        [PacketFieldType(ConstSize = false, MaxSize = 512)]
                        public UInt32[] Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt43>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt43> pParser;
        }
}
