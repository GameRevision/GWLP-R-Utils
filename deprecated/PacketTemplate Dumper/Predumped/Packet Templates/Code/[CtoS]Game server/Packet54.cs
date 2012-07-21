using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 54)]
        public class Packet54 : IPacket
        {
                public class PacketSt54 : IPacketTemplate
                {
                        public UInt16 Header { get { return 54; } }
                        public Single Data1;
                        public Single Data2;
                        public UInt32 Data3;
                        public Single Data4;
                        public Single Data5;
                        public UInt32 Data6;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt54>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt54> pParser;
        }
}
