using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 37)]
        public class Packet37 : IPacket
        {
                public class PacketSt37 : IPacketTemplate
                {
                        public UInt16 Header { get { return 37; } }
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data1;
                        public UInt32 Data2;
                        public UInt32 Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public UInt32 Data7;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt37>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt37)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt37> pParser;

        }
}
