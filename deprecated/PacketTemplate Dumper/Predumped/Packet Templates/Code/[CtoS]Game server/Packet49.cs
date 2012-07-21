using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 49)]
        public class Packet49 : IPacket
        {
                public class PacketSt49 : IPacketTemplate
                {
                        public UInt16 Header { get { return 49; } }
                        public UInt32 Data1;
                        public byte Data2;
                        public byte Data3;
                        public byte Data4;
                        public byte Data5;
                        public byte Data6;
                        public UInt16 Data7;
                        public byte Data8;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt49>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt49> pParser;
        }
}
