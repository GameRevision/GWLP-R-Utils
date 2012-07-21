using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 405)]
        public class Packet405 : IPacket
        {
                public class PacketSt405 : IPacketTemplate
                {
                        public UInt16 Header { get { return 405; } }
                        public UInt32 ID1;
                        public UInt16 Data1;
                        public byte Data2;
                        public UInt32 Data3;
                        public byte Data4;
                        public byte Data5;
                        public UInt32 Data6;
                        public Single Data7;
                        public Single Data8;
                        public UInt16 Data9;
                        public byte Data10;
                        public byte Data11;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data12;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt405>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt405)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt405> pParser;

        }
}
