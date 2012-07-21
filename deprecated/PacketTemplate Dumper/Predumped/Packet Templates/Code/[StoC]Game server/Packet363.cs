using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 363)]
        public class Packet363 : IPacket
        {
                public class PacketSt363 : IPacketTemplate
                {
                        public UInt16 Header { get { return 363; } }
                        public UInt32 Data1;
                        public UInt16 Data2;
                        public byte Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt363>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt363)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt363> pParser;

        }
}
