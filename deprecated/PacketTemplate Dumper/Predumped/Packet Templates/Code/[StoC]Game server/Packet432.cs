using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 432)]
        public class Packet432 : IPacket
        {
                public class PacketSt432 : IPacketTemplate
                {
                        public UInt16 Header { get { return 432; } }
                        public UInt16 Data1;
                        public UInt16 Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data3;
                        public byte Data4;
                        public byte Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt432>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt432)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt432> pParser;

        }
}
