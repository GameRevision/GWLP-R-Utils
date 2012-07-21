using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 49)]
        public class Packet49 : IPacket
        {
                public class PacketSt49 : IPacketTemplate
                {
                        public UInt16 Header { get { return 49; } }
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public string Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 128)]
                        public string Data2;
                        public byte Data3;
                        public UInt32 Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt49>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt49)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt49> pParser;

        }
}
