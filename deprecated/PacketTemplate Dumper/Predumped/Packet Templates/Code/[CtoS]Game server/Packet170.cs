using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 170)]
        public class Packet170 : IPacket
        {
                public class PacketSt170 : IPacketTemplate
                {
                        public UInt16 Header { get { return 170; } }
                        public UInt16 Data1;
                        public byte Data2;
                        public UInt16 Data3;
                        public byte Data4;
                        public byte Data5;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt170>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt170> pParser;
        }
}
