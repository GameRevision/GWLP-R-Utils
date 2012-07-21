using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 108)]
        public class Packet108 : IPacket
        {
                public class PacketSt108 : IPacketTemplate
                {
                        public UInt16 Header { get { return 108; } }
                        public byte Data1;
                        public byte ArraySize1;
                        [PacketFieldType(ConstSize = false, MaxSize = 64)]
                public UInt16[] Data2;
                        public byte Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public byte Data7;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt108>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt108)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt108> pParser;

        }
}
