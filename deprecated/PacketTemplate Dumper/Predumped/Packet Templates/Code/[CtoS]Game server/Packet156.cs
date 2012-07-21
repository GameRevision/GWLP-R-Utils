using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.FromClient
{
        [PacketAttributes(IsIncoming = true, Header = 156)]
        public class Packet156 : IPacket
        {
                public class PacketSt156 : IPacketTemplate
                {
                        public UInt16 Header { get { return 156; } }
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt156>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        throw new NotImplementedException();
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt156> pParser;
        }
}
