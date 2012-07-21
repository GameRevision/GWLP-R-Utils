using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 388)]
        public class Packet388 : IPacket
        {
                public class PacketSt388 : IPacketTemplate
                {
                        public UInt16 Header { get { return 388; } }
                        public UInt16 Data1;
                        public byte Data2;
                        public byte Data3;
                        public byte Data4;
                        public UInt32 Data5;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data6;
                        public byte ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data7;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt388>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt388)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt388> pParser;

        }
}
