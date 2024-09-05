package com.duyanh.haui.RSA_algorithm;

import com.aspose.words.Document;
import com.aspose.words.DocumentBuilder;
import com.aspose.words.SaveFormat;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;
import java.util.List;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;
import org.apache.commons.io.IOUtils;
import org.apache.poi.xwpf.converter.xhtml.XHTMLConverter;
import org.apache.poi.xwpf.converter.xhtml.XHTMLOptions;
import org.apache.poi.xwpf.usermodel.ParagraphAlignment;
import org.apache.poi.xwpf.usermodel.XWPFDocument;
import org.apache.poi.xwpf.usermodel.XWPFParagraph;
import org.apache.poi.xwpf.usermodel.XWPFRun;
import org.docx4j.openpackaging.exceptions.Docx4JException;

/**
 *
 * @author Admin
 */
public class FileUtils {

    private final String cipherHtml = "D:\\banma.html";
    private final static String plainHtml = "D:\\banro.html";
    private final static String tempPath = "";

    /**
     * Convert word in docx format to html
     *
     * @param fileName docx file path
     * @param outPutFile html output file path
     * @throws TransformerException
     * @throws IOException
     * @throws ParserConfigurationException
     */
    public static File docx2Html(String fileName, String outPutFile) throws TransformerException, IOException, ParserConfigurationException {
        String fileOutName = outPutFile;
        XWPFDocument document = new XWPFDocument(new FileInputStream(fileName));
        XHTMLOptions options = XHTMLOptions.create().indent(4);
        File outFile = new File(fileOutName);
        outFile.getParentFile().mkdirs();
        try (OutputStream out = new FileOutputStream(outFile)) {
            XHTMLConverter.getInstance().convert(document, out, options);
        } finally {
            document.close(); // Close the document to prevent resource leak
        }
        return outFile;
    }

    public static String readFromFileText(String fileName) throws IOException {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(fileName), "UTF-8"))) {
            return reader.readLine();
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public static void writeFileText(String fileName, String text) throws IOException {
        try (OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(fileName), StandardCharsets.UTF_8)) {
            writer.write(text);
            System.out.println("Successfully write to path " + fileName);
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }

    public static String readFromFileDocx(String fileName) throws IOException, TransformerException, ParserConfigurationException {
        File htmlResult = null;
        try {
            htmlResult = docx2Html(fileName, plainHtml);
            FileInputStream fis = new FileInputStream(htmlResult);
            return IOUtils.toString(fis, StandardCharsets.UTF_8);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            if (htmlResult != null && htmlResult.exists()) {
                htmlResult.delete(); // Clean up temporary HTML file
            }
        }
    }

    public static String readDocxFile(String fileName) {
        StringBuilder data = new StringBuilder();
        try (FileInputStream fis = new FileInputStream(new File(fileName))) {
            XWPFDocument document = new XWPFDocument(fis);
            List<XWPFParagraph> paragraphs = document.getParagraphs();
            for (XWPFParagraph para : paragraphs) {
                data.append(para.getText());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return data.toString();
    }

    public static void writeFileDocx(String fileName, String text) throws IOException {
        try (XWPFDocument document = new XWPFDocument(); FileOutputStream out = new FileOutputStream(fileName)) {
            XWPFParagraph para1 = document.createParagraph();
            para1.setAlignment(ParagraphAlignment.BOTH);
            XWPFRun para1Run = para1.createRun();
            para1Run.setText(text);
            document.write(out);
            System.out.println("Successfully write to path " + fileName);
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }

    public static void convertToDocx(String text, String fileName) throws Docx4JException, IOException {
        try (Document doc = new Document()) {
            DocumentBuilder builder = new DocumentBuilder(doc);
            builder.insertHtml(text);
            doc.save(fileName, SaveFormat.DOCX);
            System.out.println("Successfully write to path " + fileName);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) throws IOException, TransformerException, ParserConfigurationException, Docx4JException {
        String fileName = "D:\\test.docx";
        String outPutFile = "D:\\banro.html";
        XWPFDocument document = new XWPFDocument(new FileInputStream(fileName));
        XHTMLOptions options = XHTMLOptions.create().indent(4);
        File outFile = new File(outPutFile);
        outFile.getParentFile().mkdirs();
        try (OutputStream out = new FileOutputStream(outFile)) {
            XHTMLConverter.getInstance().convert(document, out, options);
        } finally {
            document.close(); // Close the document to prevent resource leak
        }
    }
}