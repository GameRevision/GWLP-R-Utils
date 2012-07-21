using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 340)]
        public class Packet340 : IPacket
        {
                public class PacketSt340 : IPacketTemplate
                {
                        public UInt16 Header { get { return 340; } }
                        public UInt32 Data1;
                        public UInt32 Data2;
                        public byte Data3;
                        public byte Data4;
                        public UInt16 Data5;
                        public UInt16 Data6;
                        public byte Data7;
                        public UInt32 Data8;
                        public UInt32 Data9;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt340>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt340)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt340> pParser;

        }
}
