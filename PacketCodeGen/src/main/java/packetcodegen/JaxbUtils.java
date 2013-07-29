/**
 * For copyright information see the LICENSE document.
 */

package packetcodegen;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.net.MalformedURLException;
import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Unmarshaller;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import org.xml.sax.SAXException;


/**
 * Used to handle XML files with the JAXB framework
 * 
 * Thx to iDemmel for parts of the sourcecode
 * 
 * @author _rusty
 */
public final class JaxbUtils 
{
    /**
     * Tries to read an XML input stream and validate and marshal it.
     * Returns a valid object of type T if succesful.
     * 
     * @param       <T>                     The type that will be validated and marshalled
     * @param       docClass                The class that represents the type T
     * @param       xmlFile                 The xml file to be parsed
     * @param       schemaFile              The schema file to validate the XML file
     * @return      An object of type T filled with the content from the
     *              XML document (if everything went right that is)
     * @throws      JAXBException           If the JAXB framework threw an error
     * @throws      SAXException            If the SAX framework threw an error
     * @throws      FileNotFoundException   If specified input files could not be read
     */
    public static <T> T validateAndUnmarshal(Class<T> docClass, File xmlFile, File schemaFile)
            throws JAXBException, SAXException, FileNotFoundException 
    {
        // try to get the file
        if (!xmlFile.isFile() || !schemaFile.isFile()) { throw new FileNotFoundException("Cannot find input file"); }
        
        InputStream inputStream = new FileInputStream(xmlFile);
        
        // get the schema to validate the inputStream later on
        SchemaFactory sf = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
        
        Schema schema;
        try 
        {
            schema = sf.newSchema(schemaFile.toURI().toURL());
        } 
        catch (MalformedURLException ex) 
        {
            // should never happen
            throw new FileNotFoundException("Cannot find input file");
        }
        
        // create the unmarshaller that will parse/validate/marshal the inputStream
        String packageName = docClass.getPackage().getName();
        Unmarshaller unmarshaller = JAXBContext.newInstance(packageName).createUnmarshaller();
        
        // set the schema that the unmarshaller will use
        unmarshaller.setSchema(schema);
        
        // unmarshal and return the type
        return (T) unmarshaller.unmarshal(inputStream);
    }
}
