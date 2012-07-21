using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 103)]
        public class Packet103 : IPacket
        {
                public class PacketSt103 : IPacketTemplate
                {
                        public UInt16 Header { get { return 103; } }
                        public UInt16 Data1;
                        public byte Data2;
                        public byte Data3;
                        public byte Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public byte Data7;
                        public byte Data8;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt103>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt103)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt103> pParser;

        }
}
