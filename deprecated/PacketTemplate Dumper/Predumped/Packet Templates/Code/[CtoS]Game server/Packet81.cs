using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 81)]
        public class Packet81 : IPacket
        {
                public class PacketSt81 : IPacketTemplate
                {
                        public UInt16 Header { get { return 81; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt81>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt81> pParser;
        }
}
