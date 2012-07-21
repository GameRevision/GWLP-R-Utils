using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 64)]
        public class Packet64 : IPacket
        {
                public class PacketSt64 : IPacketTemplate
                {
                        public UInt16 Header { get { return 64; } }
                        public UInt32 Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 128)]
                        public string Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 128)]
                        public string Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt64>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt64)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt64> pParser;

        }
}
