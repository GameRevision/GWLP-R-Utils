using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 21)]
        public class Packet21 : IPacket
        {
                public class PacketSt21 : IPacketTemplate
                {
                        public UInt16 Header { get { return 21; } }
                        public UInt32 Data1;
                        [PacketFieldType(ConstSize = true, MaxSize = 8)]
                        public byte[] Data2;
                        [PacketFieldType(ConstSize = true, MaxSize = 8)]
                        public byte[] Data3;
                        public UInt32 Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt21>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt21)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt21> pParser;

        }
}
