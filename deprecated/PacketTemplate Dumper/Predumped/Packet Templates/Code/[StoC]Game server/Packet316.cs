using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 316)]
        public class Packet316 : IPacket
        {
                public class PacketSt316 : IPacketTemplate
                {
                        public UInt16 Header { get { return 316; } }
                        public UInt16 Data1;
                        public byte Data2;
                        public UInt16 Data3;
                        public byte Data4;
                        public UInt32 Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt316>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt316)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt316> pParser;

        }
}
