using System;
using ServerEngine.ProcessorQueues;
using ServerEngine.PacketManagement.CustomAttributes;
using ServerEngine.PacketManagement.Definitions;

namespace GameServer.Packets.ToClient
{
        [PacketAttributes(IsIncoming = false, Header = 462)]
        public class Packet462 : IPacket
        {
                public class PacketSt462 : IPacketTemplate
                {
                        public UInt16 Header { get { return 462; } }
                        public UInt16 Data1;
                        public UInt16 Data2;
                        public byte Data3;
                        public byte Data4;
                        public byte Data5;
                        public byte Data6;
                        public byte Data7;
                        [PacketFieldType(ConstSize = false, MaxSize = 32)]
                        public string Data8;
                        [PacketFieldType(ConstSize = false, MaxSize = 20)]
                        public string Data9;
                        public byte Data10;
                        public byte Data11;
                        public byte Data12;
                        public UInt32 Data13;
                }

                public void InitPacket(object parser)
                {
                        pParser = (PacketParser<PacketSt462>)parser;
                        IsInitialized = true;
                        IsInUse = false;
                }

                public bool Handler(ref NetworkMessage message)
                {
                        pParser((PacketSt462)message.PacketTemplate, message.PacketData);
                        QueuingService.NetOutQueue.Enqueue(message);
                        return true;
                }

                public bool IsInitialized { get; set; }

                public bool IsInUse { get; set; }

                private PacketParser<PacketSt462> pParser;

        }
}
