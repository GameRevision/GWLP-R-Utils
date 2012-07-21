using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 401)]
        public class Packet401 : IPacket
        {
                public class PacketSt401 : IPacketTemplate
                {
                        public UInt16 Header { get { return 401; } }
                        public byte Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 122)]
                        public string Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 122)]
                        public string Data3;
                        public byte Data4;
                        public UInt32 Data5;
                        public UInt32 Data6;
                        public UInt32 Data7;
                        public UInt32 Data8;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt401>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt401)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt401> pParser;

        }
}
