using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 146)]
        public class Packet146 : IPacket
        {
                public class PacketSt146 : IPacketTemplate
                {
                        public UInt16 Header { get { return 146; } }
                        public UInt32 ID1;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data1;
                        public byte Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 122)]
                        public string Data3;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt146>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt146)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt146> pParser;

        }
}
