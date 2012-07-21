using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 256)]
        public class Packet256 : IPacket
        {
                public class PacketSt256 : IPacketTemplate
                {
                        public UInt16 Header { get { return 256; } }
                        public byte Data1;
                        [PacketFieldType(ConstSize = false, MaxSize = 122)]
                        public string Data2;
                        public UInt32 Data3;
                        public UInt16 Data4;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt256>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt256)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt256> pParser;

        }
}
