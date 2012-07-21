using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 31)]
        public class Packet31 : IPacket
        {
                public class PacketSt31 : IPacketTemplate
                {
                        public UInt16 Header { get { return 31; } }
                        public UInt32 Data1;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data2;
                        public UInt16 ArraySize2;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public UInt32[] Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt31>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt31)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt31> pParser;

        }
}
