/**
 * For copyright information see the LICENSE document.
 */

package packetcodegen;

import org.apache.commons.lang.WordUtils;
import com.sun.codemodel.*;
import gwlpr.protocol.serialization.GWMessage;
import gwlpr.protocol.util.*;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.util.Arrays;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicInteger;
import javax.xml.bind.JAXBException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.xml.sax.SAXException;
import packetcodegen.jaxb.CommunicationDirection;
import packetcodegen.jaxb.CommunicationDirectionTypes;
import packetcodegen.jaxb.FieldType;
import packetcodegen.jaxb.PacketSimpleTypes;
import static packetcodegen.jaxb.PacketSimpleTypes.AGENTID;
import static packetcodegen.jaxb.PacketSimpleTypes.UTF_16;
import static packetcodegen.jaxb.PacketSimpleTypes.VARINT;
import packetcodegen.jaxb.PacketType;
import packetcodegen.jaxb.Templates;


/**
 * Taken from iDemmel (with permission), and changed to our needs.
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
            generate(JaxbUtils.validateAndUnmarshal(Templates.class, packXML, schema));
        }
        catch (JAXBException | SAXException | IOException | JClassAlreadyExistsException ex)
        {
            LOGGER.error("Error in Code-Generation: ", ex);
        }
    }
    
    
    private static void generate(Templates packetNodes) throws IOException, JClassAlreadyExistsException
    {
        JCodeModel codeModel = new JCodeModel();
	
        // outer nodes
        for (CommunicationDirection direction : packetNodes.getProtocol()) 
        {
            // check if we process inbound or outbounf packets and the server type
            CommunicationDirectionTypes prot = direction.getType();
            
            // crappy enumeration types due to keeping backwards compatibility
            boolean fromClient = (prot == CommunicationDirectionTypes.CTOGS) || (prot == CommunicationDirectionTypes.CTO_GS) ||
                                 (prot == CommunicationDirectionTypes.CTOLS) || (prot == CommunicationDirectionTypes.CTO_LS);

            String serverName = (prot == CommunicationDirectionTypes.LSTOC) || (prot == CommunicationDirectionTypes.L_STO_C) ||
                                (prot == CommunicationDirectionTypes.CTOLS) || (prot == CommunicationDirectionTypes.CTO_LS)?
                "loginserver" : "gameserver";


            // set the new packet's package
            JPackage directionPackage = codeModel._package("gwlpr.protocol." + serverName + "." + (fromClient ? "inbound" : "outbound"));

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
        String packetDescription =  (packet.getInfo() == null || packet.getInfo().getDescription() == null || packet.getInfo().getDescription().isEmpty()) ? "" : "\n" + WordUtils.wrap(packet.getInfo().getDescription(), /* maximumLength */50);
        
        JDefinedClass packetClass = dirPackage._class(JMod.FINAL | JMod.PUBLIC, packetNamePrefixed)._extends(GWMessage.class);

        LOGGER.info("+-Processing packet: {}", packetNamePrefixed);
        LOGGER.debug("|+-Packet description: {}", packetDescription);

        StringBuilder packetJavadoc = 
                new StringBuilder("Auto-generated by PacketCodeGen.")
                        .append(packetDescription);
        
        JDocComment jDocComment = packetClass.javadoc();
        jDocComment.add(packetJavadoc.toString());

        AtomicInteger numberOfUnknowns = new AtomicInteger(); // unknown field number
        
        // get all fields in this packet
        for (FieldType field : packet.getField()) 
        {
            processField(field, packetClass, codeModel, numberOfUnknowns, fromClient);
        }
        
        // generate the header method
        JMethod headerMeth = packetClass.method(JMod.PUBLIC, short.class, "getHeader");
        headerMeth.annotate(Override.class);
        headerMeth
                .body()
                ._return(JExpr.lit(packet.getHeader().intValue()));
        
        // generate getters, setters
        for (JFieldVar fieldVar : packetClass.fields().values())
        {
            processAccessors(fieldVar, packetClass, fromClient);
        }
        
        // generate the toString method
        processToString(packetClass, codeModel);
    }

    
    private static void processField(FieldType field, JDefinedClass packetClass, JCodeModel codeModel, AtomicInteger numberOfUnknowns, boolean fromClient) 
            throws JClassAlreadyExistsException 
    {
        boolean isNested = 
                (field.getType() == PacketSimpleTypes.OPTIONAL) ||
                (field.getType() == PacketSimpleTypes.ARRAY_STATIC) ||
                (field.getType() == PacketSimpleTypes.ARRAY_VAR_SMALL) ||
                (field.getType() == PacketSimpleTypes.ARRAY_VAR_BIG);
        
        boolean isArray = 
                (field.getType() == PacketSimpleTypes.BUFFER_STATIC) ||
                (field.getType() == PacketSimpleTypes.BUFFER_VAR_SMALL) ||
                (field.getType() == PacketSimpleTypes.BUFFER_VAR_BIG) ||
                (isNested && (field.getType() != PacketSimpleTypes.OPTIONAL));
        
        boolean isStaticLength =
                (isArray) &&
                ((field.getType() == PacketSimpleTypes.ARRAY_STATIC) ||
                 (field.getType() == PacketSimpleTypes.BUFFER_STATIC));
        
        // length is either not set for non arrays and static arrays,
        // its 1byte for small stuff and 2bytes for big stuff.
        int prefixLength = 
                (!isArray || isStaticLength) ? -1 :
                (field.getType() == PacketSimpleTypes.ARRAY_VAR_SMALL) ||
                (field.getType() == PacketSimpleTypes.BUFFER_VAR_SMALL) ?
                1 : 2;

        String name = "";
        
        if (field.getInfo() == null || field.getInfo().getName() == null)
        {
            // check if we got special fields...
            if      (field.getType() == PacketSimpleTypes.AGENTID)  { name = "AgentID"; }
            else if (field.getType() == PacketSimpleTypes.OPTIONAL) { name = "Optional"; }
            else                                                    { name = "Unknown"; }
            
            name += numberOfUnknowns.incrementAndGet();
        }
        else
        {
            name = field.getInfo().getName();
        }
        
        String fieldName = WordUtils.uncapitalize(name);
        String fieldDescription =  (field.getInfo() == null || field.getInfo().getDescription() == null || field.getInfo().getDescription().isEmpty()) ? "" : "\n" + WordUtils.wrap(field.getInfo().getDescription(), /* maximumLength */50);
        
        JType fieldType;

        if (isNested) 
        {
            JDefinedClass nestedClass = packetClass._class(JMod.FINAL | JMod.STATIC | JMod.PUBLIC, "Nested" + name)._implements(NestedMarker.class);
            
            AtomicInteger numberOfUnknownsNested = new AtomicInteger();
            
            for (FieldType nested : field.getField()) 
            {
                processField(nested, nestedClass, codeModel, numberOfUnknownsNested, fromClient);
            }
            
            // generate getters, setters
            for (JFieldVar fieldVar : nestedClass.fields().values())
            {
                processAccessors(fieldVar, nestedClass, fromClient);
            }
            
            processToString(nestedClass, codeModel);
            
            // nested classes are either arrays or optional...
            // meaning we will later have to test if they are null before reading/writing
            fieldType = isArray? nestedClass.array() : nestedClass;
        } 
        else 
        {
            Class<?> fieldClass = convertFieldTypeToClass(field);
            fieldType = codeModel._ref(fieldClass);
        }

        LOGGER.debug("|+-Processing field: {}, of type: {}", fieldName, fieldType);
        
        // add the field
        JFieldVar packetField = packetClass.field(JMod.PRIVATE, fieldType, fieldName);
        
        if (fieldDescription != null && !fieldDescription.trim().equals(""))
        {
            JDocComment jDocComment = packetField.javadoc();
            jDocComment.add(fieldDescription);
        }
        
        // and dont forget array annotations if necessary
        if (isArray) 
        {
            int size = (int) (field.getElements() == null ? -1 : field.getElements());
            
            packetField.annotate(IsArray.class)
                    .param("constant", isStaticLength)
                    .param("size", size)
                    .param("prefixLength", prefixLength);
        }
        
        // or any special annotations
        if (field.getType() == PacketSimpleTypes.LONG)  { packetField.annotate(IsInt64.class); }
        if (field.getType() == PacketSimpleTypes.VARINT){ packetField.annotate(IsVarInt.class); }
        if (field.getType() == PacketSimpleTypes.ASCII) { packetField.annotate(IsASCII.class); }
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
    
    
    private static void processToString(JDefinedClass packetClass, JCodeModel codeModel)
    {
        JClass string = codeModel.ref(String.class);
        JClass stringBuilder = codeModel.ref(StringBuilder.class);
        JClass arrays = codeModel.ref(Arrays.class);
        
        JMethod toStringMeth = packetClass.method(JMod.PUBLIC, String.class, "toString");
        toStringMeth.annotate(Override.class);
        JBlock body = toStringMeth.body();
        
        JVar stringBuilderVar = body.decl(stringBuilder, "sb");
        stringBuilderVar = stringBuilderVar.init(JExpr._new(stringBuilder).arg(packetClass.name() + "["));
        
        JInvocation appendChain = null;
        
        for (JFieldVar fieldVar : packetClass.fields().values())
        {
            if (appendChain != null) 
            { 
                // a comma is needed
                appendChain = appendChain.invoke("append").arg("," + fieldVar.name() + "=");
            } 
            else 
            {
                appendChain = stringBuilderVar.invoke("append").arg(fieldVar.name() + "=");
            }
            
            // now add the field to the toString output
            JExpression expression = fieldVar.type().isArray() ? 
                    arrays.staticInvoke("toString").arg(JExpr._this().ref(fieldVar.name())) :
                        fieldVar.type().isReference() ?
                            JExpr._this().ref(fieldVar.name()).invoke("toString") :
                            JExpr._this().ref(fieldVar.name());
            
            appendChain = appendChain.invoke("append").arg(expression);
        }
        
        if (appendChain != null) 
        {
            appendChain = appendChain.invoke("append").arg("]");
        } 
        else 
        {
            appendChain = stringBuilderVar.invoke("append").arg("]");
        }

        body.add(appendChain);
        body._return(stringBuilderVar.invoke("toString"));
    }
    
    
    private static Class<?> convertFieldTypeToClass(FieldType field) 
    {
        PacketSimpleTypes type = field.getType();
        
        if (type == null) { throw new IllegalArgumentException("Type cannot be null");  }

        switch (type)
        {
            case BYTE:              return short.class;
            case SHORT:             return int.class;
            case INT:               return long.class;
            case AGENTID:           return long.class;
            case LONG:              return long.class;
            case FLOAT:             return float.class;
            case VEC_2:             return Vector2.class;
            case VEC_3:             return Vector3.class;
            case VEC_4:             return Vector4.class;
            case DW_3:              return WorldPosition.class;
            case VARINT:            return int.class;
            case ASCII:             return String.class;
            case UTF_16:            return String.class;
            case UID_16:            return UUID.class;
            case GUID_18:           return GUID18.class;
            case BUFFER_STATIC:     return byte[].class;
            case BUFFER_VAR_SMALL:  return byte[].class;
            case BUFFER_VAR_BIG:    return byte[].class;

            default: throw new UnsupportedOperationException("Unsupported field type because it was unused until now: " + type);
        }
    }
}
