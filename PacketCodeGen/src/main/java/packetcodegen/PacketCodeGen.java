/**
 * For copyright information see the LICENSE document.
 */

package packetcodegen;

import realityshard.shardlet.utils.GenericAction;
import org.apache.commons.lang.WordUtils;
import com.sun.codemodel.JBlock;
import com.sun.codemodel.JClass;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JDocComment;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JExpression;
import com.sun.codemodel.JFieldVar;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JPackage;
import com.sun.codemodel.JTryBlock;
import com.sun.codemodel.JType;
import com.sun.codemodel.JVar;
import gwlpr.actions.Vector2;
import gwlpr.actions.Vector3;
import gwlpr.actions.Vector4;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.ByteBuffer;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Level;
import javax.xml.bind.JAXBException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.xml.sax.SAXException;
import packetcodegen.jaxb.CommunicationDirection;
import packetcodegen.jaxb.PacketFieldType;
import packetcodegen.jaxb.PacketSimpleTypes;
import packetcodegen.jaxb.PacketType;
import packetcodegen.jaxb.Packets;


/**
 * Taken from iDemmel, and changed to our needs.
 * 
 * @author _rusty
 */
public final class PacketCodeGen 
{
    
    private final static Logger LOGGER = LoggerFactory.getLogger(PacketCodeGen.class);
    
    
    /**
     * Application entry point.
     *
     * @param       args                    Command line args
     *                                      (See code for more information)
     */
    public static void main(String[] args)
    {
        // try load the schema
        File schema = new File("input" + File.separator + "gw-packet-schema.xsd");
        if (!schema.exists()) { LOGGER.error("Schema file not found: {}", schema.getAbsolutePath()); return; }

        // try load the templates
        File packXML = new File("input" + File.separator + "PacketTemplates.xml");
        if (!packXML.exists()) { LOGGER.error("Packet templates file not found: {}", packXML.getAbsolutePath()); return; }


        try
        {
            // generate from out of the schema
            generate(JaxbUtils.validateAndUnmarshal(Packets.class, packXML, schema));
        }
        catch (JAXBException | SAXException | IOException | JClassAlreadyExistsException ex)
        {
            LOGGER.error("Error in Code-Generation: ", ex);
        }
    }
    
    
    private static void generate(Packets packetNodes) throws IOException, JClassAlreadyExistsException
    {
        JCodeModel codeModel = new JCodeModel();
	
        // outer nodes
        for (CommunicationDirection direction : packetNodes.getDirection()) 
        {
            // check if we process inbound or outbounf packets and the server type
            boolean fromClient = direction.getAbbr().startsWith("C");

            String serverName = ((direction.getAbbr().charAt(0) == 'L') || (direction.getAbbr().charAt(3) == 'L')) ?
                "loginserver" : "gameserver";

            // set the new packet's package
            JPackage directionPackage = codeModel._package("gwlpr.actions." + serverName + "." + (fromClient ? "inbound" : "outbound"));

            LOGGER.info("Processing packets of package: {}", directionPackage);

            for (PacketType packet : direction.getPacket()) 
            {
                processPacket(packet, directionPackage, codeModel, fromClient);
            }
        }

        // finally take the output and generate the files
        codeModel.build(new File("output"), new PrintStream(new ByteArrayOutputStream()) 
            {
                @Override
                public void print(String s) { LOGGER.debug("CodeModel: {}", s); }
            });
    }
    
    
    private static void processPacket(PacketType packet, JPackage dirPackage, JCodeModel codeModel, boolean fromClient) 
            throws JClassAlreadyExistsException
    {
        // get the packet info
        String packetName =         (packet.getInfo() == null || packet.getInfo().getName() == null) ? "Unknown" : packet.getInfo().getName();
        String packetNamePrefixed = "P" + String.format("%03d", packet.getHeader()) + "_" + packetName;
        String packetDescription =  (packet.getInfo() == null || packet.getInfo().getDescription() == null || packet.getInfo().getDescription().isEmpty()) ? "" : WordUtils.wrap(packet.getInfo().getDescription(), /* maximumLength */50);
        String packetAuthor =       (packet.getInfo() == null || packet.getInfo().getAuthor() == null || packet.getInfo().getAuthor().isEmpty()) ? "nobody" : packet.getInfo().getAuthor();
        
        JDefinedClass packetClass = dirPackage._class(JMod.FINAL | JMod.PUBLIC, packetNamePrefixed)._implements(GenericAction.class);

        LOGGER.info("+-Processing packet: {}", packetNamePrefixed);
        LOGGER.debug("|+-Packet description: {}", packetDescription);
        LOGGER.debug("|+-Packet author: {}", packetAuthor);

        StringBuilder packetJavadoc = 
                new StringBuilder("Auto-generated by \"Packet generator\".")
                        .append("\n").append(packetDescription)
                        .append("\n\n@author ").append(packetAuthor);
        
        JDocComment jDocComment = packetClass.javadoc();
        jDocComment.add(packetJavadoc.toString());

        AtomicInteger numberOfUnknowns = new AtomicInteger(); // unknown field number
        
        // get all fields in this packet
        for (PacketFieldType field : packet.getField()) 
        {
            processField(field, packetClass, codeModel, numberOfUnknowns, fromClient);
        }
        
        // generate the header method
        packetClass
                .method(JMod.PUBLIC, short.class, "getHeader")
                .body()
                ._return(JExpr.lit(packet.getHeader().intValue()));
        
        // generate getters, setters
        for (JFieldVar fieldVar : packetClass.fields().values())
        {
            processAccessors(fieldVar, packetClass, fromClient);
        }
    }

    
    private static void processField(PacketFieldType field, JDefinedClass packetClass, JCodeModel codeModel, AtomicInteger numberOfUnknowns, boolean fromClient) 
            throws JClassAlreadyExistsException 
    {
        boolean isNested = field.getType().equals(PacketSimpleTypes.NESTED);

        // TODO: make the fields mandatory
        String name = field.getInfo() == null || field.getInfo().getName() == null ? "Unknown" + numberOfUnknowns.incrementAndGet() : field.getInfo().getName();
        String fieldName = WordUtils.uncapitalize(name);

        JType fieldType = null;

        if (isNested) 
        {
            JDefinedClass nestedClass = packetClass._class(JMod.FINAL | JMod.PUBLIC, "Nested" + name);
            
            AtomicInteger numberOfUnknownsNested = new AtomicInteger();
            
            for (PacketFieldType nested : field.getField()) 
            {
                processField(nested, nestedClass, codeModel, numberOfUnknownsNested, fromClient);
            }
            
            fieldType = field.getOccurs() != null ? nestedClass.array() : nestedClass;
        } 
        else 
        {
            Class<?> fieldClass = convertFieldTypeToClass(field);
            fieldType = codeModel._ref(fieldClass);
        }

        LOGGER.debug("|+-Processing field: {}, of type: {}", fieldName, fieldType);
        packetClass.field(JMod.PRIVATE, fieldType, fieldName);
    }
    
    
    private static void processAccessors(JFieldVar fieldVar, JDefinedClass packetClass, boolean fromClient)
    {
        String name = WordUtils.capitalize(fieldVar.name());
        
        if (fromClient) 
        {
            String methodName = "get" + name;
            JMethod getter = packetClass.method(JMod.PUBLIC, fieldVar.type(), methodName);
            getter.body()._return(fieldVar);
        } 
        else 
        {
            String methodName = "set" + name;
            JMethod setter = packetClass.method(JMod.PUBLIC, Void.TYPE, methodName);
            setter.param(fieldVar.type(), fieldVar.name());
            setter.body().assign(JExpr._this().ref(fieldVar.name()), JExpr.ref(fieldVar.name()));
        }
    }
    
    
    private static Class<?> convertFieldTypeToClass(PacketFieldType field) 
    {
        PacketSimpleTypes type = field.getType();
        boolean isArray = field.getOccurs() != null;
        
        if (type == null) { throw new IllegalArgumentException("Type cannot be null");  }

        switch (type)
        {
                case AGENTID: return isArray ? long[].class     : long.class;
                case FLOAT:   return isArray ? float[].class    : float.class;
                case INT_16:  return isArray ? int[].class      : int.class;
                case INT_32:  return isArray ? long[].class     : long.class;
                case INT_8:   return isArray ? short[].class    : short.class;
                case UTF_16:  return isArray ? String.class     : char.class;
                case VEC_2:   return isArray ? Vector2[].class  : Vector2.class;
                case VEC_3:   return isArray ? Vector3[].class  : Vector3.class;
                case VEC_4:   return isArray ? Vector4[].class  : Vector4.class;
                    
                default: throw new UnsupportedOperationException("Unsupported field type because it was unused until now: " + type);
        }
    }
}
