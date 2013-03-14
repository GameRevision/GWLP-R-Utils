/**
 * For copyright information see the LICENSE document.
 */

package com.packetcodegen;

import com.packetcodegen.jaxb.CommunicationDirection;
import com.packetcodegen.jaxb.PacketFieldType;
import com.packetcodegen.jaxb.PacketType;
import com.packetcodegen.jaxb.Packets;
import java.io.File;
import java.io.FileWriter;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * This class is used to create packet actions (ShardletActions) out of a
 * packet-xml file.
 *
 * @author miracle444
 */
public final class CodeGenExec
{

    private final static Logger LOGGER = LoggerFactory.getLogger(CodeGenExec.class);


    /**
     * Application entry point.
     *
     * @param       args                    Command line args
     *                                      (See code for more information)
     */
    public static void main(String[] args)
    {
        // the schema file to be used in the codegenerator
        File schema = new File("input" + File.separator + "gw-packet-schema.xsd");

        // check if the schema file exists
        if (!schema.exists())
        {
            LOGGER.error("Schema file not found.");
            return;
        }

        // the templates file to be used in the codegenerator
        File packXML = new File("input" + File.separator + "PacketTemplates.xml");

        // check if the templates file exists
        if (!packXML.exists())
        {
            LOGGER.error("Packet templates file not found.");
            return;
        }


        try
        {
            generate(schema, packXML);
        }
        catch (Exception ex)
        {
            LOGGER.error("Error in Code-Generation: ", ex);
        }
    }


    /**
     * Generates the network packet actions.
     *
     * @param       schema
     * @param       packXML
     * @throws      Exception               If an error occured during parsing of the
     *                                      templates or during code generation.
     */
    private static void generate(File schema, File packXML) throws Exception
    {
        // this constant describes what indention spacing is used in the codegenerator
        final String indention = "    ";

        // parse templates file
        Packets packConfig = JaxbUtils.validateAndUnmarshal(Packets.class, packXML, schema);

        // get all packet directions
        List<CommunicationDirection> directions = packConfig.getDirection();

        // for each direction in the templates
        for (CommunicationDirection direction : directions)
        {
            // get all packets for this direction
            List<PacketType> packets = direction.getPacket();

            // create a converter for direction properties to get application specific properties
            DirectionConverter directionConverter = new DirectionConverter(direction);

            // writer to output the header->action mapping
            FileWriter mappingWriter = new FileWriter("output" + File.separator +
                                            directionConverter.getServerName() + "-" +
                                            directionConverter.getDirectionString() + "-" +
                                            "mapping.txt");

            // for each packet in this direction
            for (PacketType packet : packets)
            {
                // create a converter to get application specific properties of this packet
                PacketConverter packetConverter = new PacketConverter(packet, directionConverter);

                // create the output file object
                File actionFile = new File("output" + File.separator +
                                           "actions" + File.separator +
                                           directionConverter.getServerName() + File.separator +
                                           directionConverter.getDirectionString() + File.separator +
                                           packetConverter.getActionName() + ".java");

                // create directories if they dont exist
                actionFile.getParentFile().mkdirs();

                // create the actual action java file...
                try (FileWriter actionWriter = new FileWriter(actionFile))
                {
                    ClassHelper classHelper = new ClassHelper(indention, packetConverter);

                    // add mapping to output file
                    mappingWriter.append(classHelper.getMapping());

                    // get all fields in this packet
                    List<PacketFieldType> fields = packet.getField();

                    // for each field in this packet
                    for (PacketFieldType field : fields)
                    {
                        // create a converter to get application specific information of this field
                        FieldConverter fieldConverter = new FieldConverter(field, packetConverter);

                        // add the field to this action
                        classHelper.addField(fieldConverter);

                        // if this field's type is nested add the type-definition
                        if (fieldConverter.isNested())
                        {
                            classHelper.addNestedType(fieldConverter, field.getField());
                        }
                    }

                    // output the generated code
                    actionWriter.write(classHelper.toString());
                }
            }

            // close the writer object
            mappingWriter.close();
        }
    }
}
