using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 422)]
        public class Packet422 : IPacket
        {
                public class PacketSt422 : IPacketTemplate
                {
                        public UInt16 Header { get { return 422; } }
                        public byte Data1;
                        public byte Data2;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data3;
                        public UInt32 Data4;
                        public byte Data5;
                        public UInt32 Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt422>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt422)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt422> pParser;

        }
}
