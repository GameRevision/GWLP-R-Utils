using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 17)]
        public class Packet17 : IPacket
        {
                public class PacketSt17 : IPacketTemplate
                {
                        public UInt16 Header { get { return 17; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public UInt32 Data3;
                        [PacketFieldType(ConstSize = true, MaxSize = 8)]
                        public byte[] Data4;
                        [PacketFieldType(ConstSize = true, MaxSize = 8)]
                        public byte[] Data5;
                        [PacketFieldType(ConstSize = true, MaxSize = 16)]
                        public byte[] Data6;
                        [PacketFieldType(ConstSize = true, MaxSize = 16)]
                        public byte[] Data7;
                        public UInt32 Data8;
                        public UInt16 ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 200)]
                        public byte[] Data9;
                        public byte Data10;
                        public UInt32 Data11;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt17>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt17)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt17> pParser;

        }
}
