using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 33)]
        public class Packet33 : IPacket
        {
                public class PacketSt33 : IPacketTemplate
                {
                        public UInt16 Header { get { return 33; } }
                        public UInt32 Data1;
                        public UInt16 Data2;
                        public byte Data3;
                        public byte Data4;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt33>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt33)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt33> pParser;

        }
}
