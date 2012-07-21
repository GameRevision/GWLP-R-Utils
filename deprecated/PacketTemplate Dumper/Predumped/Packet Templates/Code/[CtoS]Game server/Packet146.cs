using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 146)]
        public class Packet146 : IPacket
        {
                public class PacketSt146 : IPacketTemplate
                {
                        public UInt16 Header { get { return 146; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt146>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt146> pParser;
        }
}
