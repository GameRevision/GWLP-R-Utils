using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 179)]
        public class Packet179 : IPacket
        {
                public class PacketSt179 : IPacketTemplate
                {
                        public UInt16 Header { get { return 179; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt179>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt179> pParser;
        }
}
