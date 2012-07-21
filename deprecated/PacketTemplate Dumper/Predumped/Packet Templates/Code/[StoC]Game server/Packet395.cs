using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 395)]
        public class Packet395 : IPacket
        {
                public class PacketSt395 : IPacketTemplate
                {
                        public UInt16 Header { get { return 395; } }
                        public UInt32 ID1;
                        public UInt16 Data1;
                        public byte Data2;
                        public UInt32 Data3;
                        public byte Data4;
                        public byte Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt395>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt395)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt395> pParser;

        }
}
