using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 281)]
        public class Packet281 : IPacket
        {
                public class PacketSt281 : IPacketTemplate
                {
                        public UInt16 Header { get { return 281; } }
                        [PacketFieldType(ConstSize = true, MaxSize = 16)]
                        public byte[] Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 6)]
                        public string Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public byte Data7;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt281>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt281)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt281> pParser;

        }
}
