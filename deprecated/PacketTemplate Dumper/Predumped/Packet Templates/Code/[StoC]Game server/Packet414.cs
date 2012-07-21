using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 414)]
        public class Packet414 : IPacket
        {
                public class PacketSt414 : IPacketTemplate
                {
                        public UInt16 Header { get { return 414; } }
                        public UInt16 Data1;
                        public UInt32 Data2;
                        public byte Data3;
                        public byte Data4;
                        public byte Data5;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt414>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt414)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt414> pParser;

        }
}
