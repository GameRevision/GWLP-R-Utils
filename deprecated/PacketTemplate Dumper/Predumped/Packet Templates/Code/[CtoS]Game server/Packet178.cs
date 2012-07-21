using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 178)]
        public class Packet178 : IPacket
        {
                public class PacketSt178 : IPacketTemplate
                {
                        public UInt16 Header { get { return 178; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt178>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt178> pParser;
        }
}
