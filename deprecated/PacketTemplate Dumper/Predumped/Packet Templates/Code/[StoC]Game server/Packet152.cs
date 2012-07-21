using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 152)]
        public class Packet152 : IPacket
        {
                public class PacketSt152 : IPacketTemplate
                {
                        public UInt16 Header { get { return 152; } }
                        public UInt32 ID1;
                        public Single Data1;
                        public Single Data2;
                        public UInt16 Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public byte Data7;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt152>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt152)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt152> pParser;

        }
}
