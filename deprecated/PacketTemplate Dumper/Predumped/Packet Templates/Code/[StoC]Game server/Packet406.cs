using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 406)]
        public class Packet406 : IPacket
        {
                public class PacketSt406 : IPacketTemplate
                {
                        public UInt16 Header { get { return 406; } }
                        [PacketFieldType(ConstSize = true, MaxSize = 24)]
                        public byte[] Data1;
                        public UInt32 Data2;
                        public byte Data3;
                        public UInt16 Data4;
                        public byte Data5;
                        public UInt32 Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt406>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt406)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt406> pParser;

        }
}
