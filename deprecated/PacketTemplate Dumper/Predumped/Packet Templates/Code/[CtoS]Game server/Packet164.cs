using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 164)]
        public class Packet164 : IPacket
        {
                public class PacketSt164 : IPacketTemplate
                {
                        public UInt16 Header { get { return 164; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt164>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt164> pParser;
        }
}
