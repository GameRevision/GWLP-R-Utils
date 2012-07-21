using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 391)]
        public class Packet391 : IPacket
        {
                public class PacketSt391 : IPacketTemplate
                {
                        public UInt16 Header { get { return 391; } }
                        public UInt32 Data1;
                        public Single Data2;
                        public Single Data3;
                        public UInt16 Data4;
                        public byte Data5;
                        public byte Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt391>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt391)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt391> pParser;

        }
}
