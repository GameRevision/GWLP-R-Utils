//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.5-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2013.08.01 at 06:37:19 PM EDT 
//


package packetcodegen.jaxb;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for CommunicationDirectionTypes.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * <p>
 * <pre>
 * &lt;simpleType name="CommunicationDirectionTypes">
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string">
 *     &lt;enumeration value="ctols"/>
 *     &lt;enumeration value="lstoc"/>
 *     &lt;enumeration value="ctogs"/>
 *     &lt;enumeration value="gstoc"/>
 *   &lt;/restriction>
 * &lt;/simpleType>
 * </pre>
 * 
 */
@XmlType(name = "CommunicationDirectionTypes")
@XmlEnum
public enum CommunicationDirectionTypes {

    @XmlEnumValue("ctols")
    CTOLS("ctols"),
    @XmlEnumValue("lstoc")
    LSTOC("lstoc"),
    @XmlEnumValue("ctogs")
    CTOGS("ctogs"),
    @XmlEnumValue("gstoc")
    GSTOC("gstoc");
    private final String value;

    CommunicationDirectionTypes(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static CommunicationDirectionTypes fromValue(String v) {
        for (CommunicationDirectionTypes c: CommunicationDirectionTypes.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
