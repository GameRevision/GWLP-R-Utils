using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace LoginServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 46)]
        public class Packet46 : IPacket
        {
                public class PacketSt46 : IPacketTemplate
                {
                        public UInt16 Header { get { return 46; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt46>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt46> pParser;
        }
}
