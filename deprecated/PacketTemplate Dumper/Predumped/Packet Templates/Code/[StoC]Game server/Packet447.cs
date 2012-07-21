using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 447)]
        public class Packet447 : IPacket
        {
                public class PacketSt447 : IPacketTemplate
                {
                        public UInt16 Header { get { return 447; } }
                        public UInt16 Data1;
                        public UInt16 Data2;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt447>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt447)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt447> pParser;

        }
}
