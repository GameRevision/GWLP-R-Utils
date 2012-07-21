using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 435)]
        public class Packet435 : IPacket
        {
                public class PacketSt435 : IPacketTemplate
                {
                        public UInt16 Header { get { return 435; } }
                        public UInt16 Data1;
                        public UInt16 Data2;
                        public UInt16 Data3;
                        public byte Data4;
                        public byte Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt435>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt435)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt435> pParser;

        }
}
