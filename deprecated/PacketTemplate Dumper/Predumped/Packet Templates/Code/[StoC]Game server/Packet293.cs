using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 293)]
        public class Packet293 : IPacket
        {
                public class PacketSt293 : IPacketTemplate
                {
                        public UInt16 Header { get { return 293; } }
                        [PacketFieldType(ConstSize = false, MaxSize = 256)]
                        public string Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data2;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt293>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt293)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt293> pParser;

        }
}
