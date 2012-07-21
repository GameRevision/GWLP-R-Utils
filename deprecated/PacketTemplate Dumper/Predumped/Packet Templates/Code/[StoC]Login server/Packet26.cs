using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 26)]
        public class Packet26 : IPacket
        {
                public class PacketSt26 : IPacketTemplate
                {
                        public UInt16 Header { get { return 26; } }
                        public UInt32 Data1;
                        public byte ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 11)]
                        public byte[] Data2;
                        public byte Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt26>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt26)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt26> pParser;

        }
}
