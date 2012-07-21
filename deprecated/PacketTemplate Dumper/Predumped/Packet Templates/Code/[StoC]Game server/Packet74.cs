using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 74)]
        public class Packet74 : IPacket
        {
                public class PacketSt74 : IPacketTemplate
                {
                        public UInt16 Header { get { return 74; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public UInt32 Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public byte Data7;
                        public byte Data8;
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public string Data9;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt74>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt74)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt74> pParser;

        }
}
