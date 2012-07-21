using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 61)]
        public class Packet61 : IPacket
        {
                public class PacketSt61 : IPacketTemplate
                {
                        public UInt16 Header { get { return 61; } }
                        public UInt32 Data1;
                        public Single Data2;
                        public Single Data3;
                        public UInt16 Data4;
                        public UInt16 Data5;
                        public UInt32 Data6;
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public string Data7;
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public string Data8;
                        [PacketFieldType(ConstSize = false, MaxSize = 8)]
                        public string Data9;
                        public UInt16 Data10;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt61>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt61)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt61> pParser;

        }
}
