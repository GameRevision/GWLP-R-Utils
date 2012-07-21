using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 253)]
        public class Packet253 : IPacket
        {
                public class PacketSt253 : IPacketTemplate
                {
                        public UInt16 Header { get { return 253; } }
                        public UInt32 ID1;
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public byte Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt253>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt253)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt253> pParser;

        }
}
