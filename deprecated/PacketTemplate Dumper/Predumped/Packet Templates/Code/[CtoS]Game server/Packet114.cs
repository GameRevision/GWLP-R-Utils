using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 114)]
        public class Packet114 : IPacket
        {
                public class PacketSt114 : IPacketTemplate
                {
                        public UInt16 Header { get { return 114; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt114>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt114> pParser;
        }
}
