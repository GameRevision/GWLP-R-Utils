using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 16)]
        public class Packet16 : IPacket
        {
                public class PacketSt16 : IPacketTemplate
                {
                        public UInt16 Header { get { return 16; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        [PacketFieldType(ConstSize = true, MaxSize = 16)]
                        public byte[] Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data4;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data5;
                        [PacketFieldType(ConstSize = false, MaxSize = 256)]
                        public string Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt16>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt16)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt16> pParser;

        }
}
