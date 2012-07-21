using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 160)]
        public class Packet160 : IPacket
        {
                public class PacketSt160 : IPacketTemplate
                {
                        public UInt16 Header { get { return 160; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt160>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt160> pParser;
        }
}
