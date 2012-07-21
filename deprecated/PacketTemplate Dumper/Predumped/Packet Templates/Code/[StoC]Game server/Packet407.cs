using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 407)]
        public class Packet407 : IPacket
        {
                public class PacketSt407 : IPacketTemplate
                {
                        public UInt16 Header { get { return 407; } }
                        public byte Data1;
                        public byte Data2;
                        public UInt32 Data3;
                        [PacketFieldType(ConstSize = false, MaxSize = 122)]
                        public string Data4;
                        public UInt32 Data5;
                        [PacketFieldType(ConstSize = false, MaxSize = 122)]
                        public string Data6;
                        public UInt32 Data7;
                        public byte Data8;
                        public byte Data9;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt407>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt407)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt407> pParser;

        }
}
