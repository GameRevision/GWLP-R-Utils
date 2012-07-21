using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 24)]
        public class Packet24 : IPacket
        {
                public class PacketSt24 : IPacketTemplate
                {
                        public UInt16 Header { get { return 24; } }
                        public UInt32 Data1;
                        [PacketFieldType(ConstSize = true, MaxSize = 8)]
                        public byte[] Data2;
                        [PacketFieldType(ConstSize = true, MaxSize = 8)]
                        public byte[] Data3;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 200)]
                        public byte[] Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt24>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt24)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt24> pParser;

        }
}
