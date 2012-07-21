using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 365)]
        public class Packet365 : IPacket
        {
                public class PacketSt365 : IPacketTemplate
                {
                        public UInt16 Header { get { return 365; } }
                        public UInt32 Data1;
                        public UInt16 Data2;
                        public byte Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                        public string Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt365>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt365)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt365> pParser;

        }
}
