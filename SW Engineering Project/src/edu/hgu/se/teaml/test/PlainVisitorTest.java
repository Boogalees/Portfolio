package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import java.io.BufferedWriter;
import java.util.LinkedList;
import java.util.List;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import edu.hgu.se.teaml.HashHandler;
import edu.hgu.se.teaml.Header;
import edu.hgu.se.teaml.PlainText;
import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.Block;
import edu.hgu.se.teaml.CodeBlock;
import edu.hgu.se.teaml.Document;
import edu.hgu.se.teaml.HashObject;
import edu.hgu.se.teaml.MDElement;
import edu.hgu.se.teaml.Token;
import edu.hgu.se.teaml.UnorderedList;
import edu.hgu.se.teaml.Strong;
import edu.hgu.se.teaml.Code;
import edu.hgu.se.teaml.Hr;
import edu.hgu.se.teaml.Link;
import edu.hgu.se.teaml.SetextHeader;
import edu.hgu.se.teaml.AtxHeader;
import edu.hgu.se.teaml.QuotedBlock;
import edu.hgu.se.teaml.Br;
import edu.hgu.se.teaml.Emphasis;
import edu.hgu.se.teaml.Node;
import edu.hgu.se.teaml.OrderedList;
import edu.hgu.se.teaml.StyleText;
import edu.hgu.se.teaml.HTMLCode;
import edu.hgu.se.teaml.Image;
import edu.hgu.se.teaml.UnorderedList;
import edu.hgu.se.teaml.OrderedList;
import edu.hgu.se.teaml.ItemList;




public class PlainVisitorTest {

   String outputFileName;
   private BufferedWriter file;
   
   
   @Before
   public void setUp()
   {
   
   }
   
   @After
   public void tearDown()
   {
      
   }
   
   @Test
   public void testSetHashHandler() {
      
      HashHandler hashHandler = new HashHandler();
      HashObject object = new HashObject("http://www.hisnet.handong.edu","hisnet");
      hashHandler.put("hisnet", object);
      
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      
      visitor.setHashHandler(hashHandler);
      
      HashHandler compare = visitor.getHashHandler();

      assertEquals("message","http://www.hisnet.handong.edu",compare.getSrc("hisnet"));
      assertEquals("message","hisnet",compare.getTitle("hisnet"));

      

   }

   
   @Test
   public void testPlainVisitor() {
      HashHandler hashHandler = new HashHandler();
      HashObject object = new HashObject("http://www.hisnet.handong.edu","hisnet");
      hashHandler.put("hisnet", object);

      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);

      HashHandler compare = visitor.getHashHandler();
      
      assertEquals("message","http://www.hisnet.handong.edu",compare.getSrc("hisnet"));
      assertEquals("message","hisnet",compare.getTitle("hisnet"));

      PlainVisitor visitorEmpty = new PlainVisitor();
      
      PlainVisitor visitorNull = new PlainVisitor(null,null );

   }

   @Test
   public void testVisitDocument() {
   
      Document doc = new Document();
      PlainText plainText = new PlainText();
      HashHandler hashHandler = new HashHandler();
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      
      doc.setFileName("fileName");
      plainText.setValue("Contents");
      
      CodeBlock codeblock = new CodeBlock();
      codeblock.add(plainText);
      
      doc.add(codeblock);
      visitor.visitDocument(doc);

      visitor.visitDocument(null);

   }
   
   @Test
   public void testVisitElementsListOfMDElement() {
      HashHandler hashHandler = new HashHandler();
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      List<MDElement> mdElements = new LinkedList<MDElement>();
      
      Header header = new Header();
      PlainText plainText = new PlainText();
      
      plainText.setValue("Contents");
      header.add(plainText);
      
      mdElements.add(header);
      
    
      
   }

   @Test
   public void testVisitTokens() {
      HashHandler hashHandler = new HashHandler();
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      List<Token> tokens = new LinkedList<Token>();

      PlainText plainText = new PlainText();
      plainText.setValue("Contents");
      
      tokens.add(plainText);
      
      visitor.visitTokens(tokens);
   }

   @Test
   public void testVisitBlock() {
      HashHandler hashHandler = new HashHandler();
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      
      PlainText plainText = new PlainText();
      plainText.setValue("Contents");
      
      Block block = new Block();
      block.add(plainText);
 
      visitor.visitBlock(block);

      visitor.visitBlock(null);

   }

   @Test
   public void testVisitStrong() {
      HashHandler hashHandler = new HashHandler();
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      
      PlainText plainText = new PlainText();
      plainText.setValue("Contents");
      
      Strong strong = new Strong();
      strong.addToken("Contents");
            
      visitor.visitStrong(strong);
      
      visitor.visitStrong(null);
      
   }

   @Test
   public void testVisitCode() {
	  HashHandler hashHandler = new HashHandler();
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      
      Code code = new Code();
      
      visitor.visitCode(code);
      
      code.setValue("Contents");
      
      visitor.visitCode(code);
   }

   @Test
   public void testVisitCodeBlock() {
	  HashHandler hashHandler = new HashHandler();
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      
      CodeBlock codeBlock = new CodeBlock();
      PlainText plainText = new PlainText();
      plainText.setValue("Contents");
      
      codeBlock.add(plainText);
      
      visitor.visitCodeBlock(codeBlock);

      visitor.visitCodeBlock(null);

   }


   @Test
   public void testVisitLink() {
	  HashHandler hashHandler = new HashHandler();
	  HashObject object = new HashObject("url","title");
      hashHandler.put("key", object);
	  
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
	  
      Link link = new Link();
      link.setHref("url");
      visitor.visitLink(link);


      link.setId("key");
      visitor.visitLink(link);

    
      link.setId("key1");
      link.setTitle("");
      visitor.visitLink(link);

      link.setTitle("title");
      visitor.visitLink(link);

      visitor.visitLink(null);
   }

   @Test
   public void testVisitPlainText() {
	  HashHandler hashHandler = new HashHandler();
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
      
	  PlainText plainText = new PlainText();
      plainText.setValue("Contents");
   
      visitor.visitPlainText(plainText);
      
      assertEquals("message","Contents",plainText.getValue());

      visitor.visitPlainText(null);

   }

   @Test
   public void testVisitHr() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
	  
	  Hr hr = new Hr();
	  
	  visitor.visitHr(hr);

	  visitor.visitHr(null );

   }

   @Test
   public void testVisitHeader() {  
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
   
	  Header header = new Header();
	  PlainText plainText = new PlainText();
   
	  plainText.setValue("Contents");
	  header.add(plainText);
	  header.setSize(1);
	  
	  visitor.visitHeader(header);
	  visitor.visitHeader(null);
    }

   @Test
   public void testVisitSetextHeader() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
   
	  SetextHeader setextHeader = new SetextHeader();
	  PlainText plainText = new PlainText();
   
	  plainText.setValue("Contents");
	  setextHeader.add(plainText);
	  setextHeader.setSize(1);
	  
	  visitor.visitSetextHeader(setextHeader);      
   }

   @Test
   public void testVisitAtxHeader() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
   
	  AtxHeader atxHeader = new AtxHeader();
	  PlainText plainText = new PlainText();
   
	  plainText.setValue("Contents");
	  atxHeader.add(plainText);
	  atxHeader.setSize(1);
	  
	  visitor.visitAtxHeader(atxHeader);      
   }

   @Test
   public void testVisitQuotedBlock() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
   
	  QuotedBlock quotedBlock = new QuotedBlock();
	  PlainText plainText = new PlainText();
   
	  plainText.setValue("Contents");
	  quotedBlock.add(plainText);
   
	  visitor.visitQuotedBlock(quotedBlock);

	  visitor.visitQuotedBlock(null);

   }

   @Test
   public void testVisitEmphasis() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
   
	  Emphasis emphasis = new Emphasis();
	  PlainText plainText = new PlainText();
	   
	  plainText.setValue("Contents");
	  
	  emphasis.addToken("Contents");
	  visitor.visitEmphasis(emphasis);
	  
	  emphasis.addToken(plainText);
	  visitor.visitEmphasis(emphasis);

	  visitor.visitEmphasis(null);

   }

   @Test
   public void testVisitImage() {
	  HashHandler hashHandler = new HashHandler();
	  HashObject object = new HashObject("url","title");
      hashHandler.put("key", object);
	  
      PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
	  
      Image image = new Image();
      image.setSrc("url");
      visitor.visitImage(image);


      image.setId("key");
      visitor.visitImage(image);

     
      image.setId("key1");
      image.setTitle("");
      visitor.visitImage(image);

      image.setTitle("title");
      visitor.visitImage(image);

      visitor.visitImage(null);
     }

   @Test
   public void testVisitBr() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
	  
	  Br br = new Br();
	  
	  visitor.visitBr(br);
	  visitor.visitBr(null);

   }

   @Test
   public void testVisitElementsNode() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
	  
	  PlainText plainText = new PlainText();
	  plainText.setValue("Contents");
	  Node node = new Node();
	  node.add(plainText);
	  
	  visitor.visitElements(node);
   }

   @Test
   public void testVisitStyleText() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
   
	  StyleText styleText = new StyleText();
 
	  styleText.addToken("Contents");
	  visitor.visitStyleText(styleText);
	  
   }

   @Test
   public void testVisitNode() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
	  
	  Node node = new Node();
	  
	  visitor.visitNode(node);

   }

   @Test
   public void testVisitToken() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
	  
	  Token token = new Token();
	  
	  visitor.visitToken(token);

   }

   @Test
   public void testVisitHtmlCode() {
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
	  
	  HTMLCode htmlCode = new HTMLCode();
	  visitor.visitHtmlCode(htmlCode);

	  htmlCode.setValue("Contents");
	  visitor.visitHtmlCode(htmlCode);

   }
   @Test
	public void testVisitUnorderedList() {
		
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
   
	  UnorderedList unorderedList = new UnorderedList();
	  PlainText plainText = new PlainText();
      plainText.setValue("Contents");

      unorderedList.add(plainText);
     
	  visitor.visitUnorderedList(unorderedList);
	  visitor.visitUnorderedList(null);
		
	}
   
   @Test
	public void testVisitOrderedList() {
	
	  HashHandler hashHandler = new HashHandler();
	  PlainVisitor visitor = new PlainVisitor("fileName",hashHandler);
   
	  OrderedList orderedList = new OrderedList();
	  OrderedList orderedListNext = new OrderedList();
	  
	  PlainText plainText = new PlainText();
      plainText.setValue("Contents");

      orderedList.add(plainText);
      orderedList.add(orderedListNext);
      
	  visitor.visitOrderedList(orderedList);

	  visitor.visitOrderedList(null);
	
	}

}