package edu.hgu.se.teaml.test;

import static org.mockito.Mockito.*;

import java.io.BufferedReader;
import java.io.IOException;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.internal.util.reflection.Whitebox;
import org.mockito.runners.MockitoJUnitRunner;

import static org.hamcrest.CoreMatchers.*;

import edu.hgu.se.teaml.HashHandler;
import edu.hgu.se.teaml.Node;
import edu.hgu.se.teaml.Parser;
import edu.hgu.se.teaml.Strong;

@RunWith(MockitoJUnitRunner.class)
public class ParserTest {
	private Parser p;

	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
	}

	@AfterClass
	public static void tearDownAfterClass() throws Exception {
	}

	@Before
	public void setUp() throws Exception {
		p = new Parser(new String[]{ "test.md" }, new HashHandler[]{ new HashHandler() });
	}

	@After
	public void tearDown() throws Exception {
		p = null;
	}

	@Test
	public void testParser() {
		fail("Not yet implemented");
	}

	@Test
	public void testParserStart() {
		fail("Not yet implemented");
	}

//	@Test
//	public void testParseLine_edited() {
//		Node block = Node.create("BLOCK");
//		String str = "# header #  ";
//
//		assertThat(p.ParseLine_edited(
//				null, str, null, 0),
//				is(0));
//
//	}
	
	@Test
	public void testParseLine_edited_CODE() {

		Node block = Node.create("BLOCK");
		
		String str=" ` ";
		p.ParseLine_edited(block, str,"CODE", 0);
	
		str=" ";
		p.ParseLine_edited(block, str,"CODE", 0);
	
		str=" .";
		p.ParseLine_edited(block, str,"CODE", 0);
	
		str=" `.";
		p.ParseLine_edited(block, str,"CODE", 0);
	
		
	}

	@Test
	public void testParseLine_edited_CODE_BLOCK() {
		
		String str = "	test\n";

		BufferedReader bufferedReader = org.mockito.Mockito.mock(BufferedReader.class);

		Parser spy = spy(p);
	    try{
			
	        Whitebox.setInternalState(spy, "br", bufferedReader);
			
	        Node node = Node.create("CODE_BLOCK");

			doReturn(1).when(spy).exit(1);
			

	        when(bufferedReader.readLine()).thenReturn("	1").thenReturn("	2").thenReturn("	2").thenReturn("	3").thenReturn("	3").thenReturn("	4").thenReturn("test");
	        spy.ParseLine_edited(node, str,"CODE_BLOCK", 0);
			
	        when(bufferedReader.readLine()).thenReturn("	").thenReturn(null);
			spy.ParseLine_edited(node, str,"CODE_BLOCK", 0);

	        when(bufferedReader.readLine()).thenReturn("test");
			spy.ParseLine_edited(node, str,"CODE_BLOCK", 0);
			
			when(bufferedReader.readLine()).thenThrow(new IOException());
			spy.ParseLine_edited(node, str, "CODE_BLOCK", 0);
		
	    }catch(Exception e)
	    {
	    	
	    }	
		
	}

		
	@Test
	public void testParseLine_edited_BLOCK() {

		String str = "	test\n";

		BufferedReader bufferedReader = org.mockito.Mockito.mock(BufferedReader.class);

		Parser spy = spy(p);
	    try{
			
	        Whitebox.setInternalState(spy, "br", bufferedReader);
			
	        Node node = Node.create("BLOCK");

			doReturn(1).when(spy).exit(1);
			
	        str="test  ";
	        when(bufferedReader.readLine()).thenReturn("test  ").thenReturn(null);
			spy.ParseLine_edited(node, str,"BLOCK", 0);
		
			str="test\ntest";
	        when(bufferedReader.readLine()).thenReturn("1").thenReturn("2").thenReturn("3").thenReturn("4").thenReturn("5").thenReturn("6").thenReturn("7").thenReturn(null);
			spy.ParseLine_edited(node, str,"BLOCK", 0);
			
			when(bufferedReader.readLine()).thenReturn("1").thenReturn("2").thenReturn("3").thenReturn("4").thenReturn("5").thenReturn("6").thenReturn("7").thenReturn("##a");
			spy.ParseLine_edited(node, str,"BLOCK", 0);
			
			
			when(bufferedReader.readLine()).thenReturn("#aa");
			spy.ParseLine_edited(node, str,"BLOCK", 0);
			
			when(bufferedReader.readLine()).thenThrow(new IOException());
			spy.ParseLine_edited(node, str, "BLOCK", 0);

	    }catch(Exception e)
	    {
	    	
	    }	
	}
	
	@Test
	public void testParseLine_edited_Is_SETEXT_HEAD1() {
		BufferedReader br = mock(BufferedReader.class);

		Parser spy = spy(p);
		try
		{
			String str = "";
			when(spy.Is_SETEXT_HEAD(str)).thenReturn(1);

			when(br.readLine()).thenReturn("");
			Whitebox.setInternalState(spy, "br", br);
			
			spy.ParseLine_edited(null, str, null, 0);
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
		}
	}
	
	@Test
	public void testParseLine_edited_Is_SETEXT_HEAD2() {
		BufferedReader br = mock(BufferedReader.class);

		Parser spy = spy(p);
		try
		{
			String str = "";
			when(spy.Is_SETEXT_HEAD(str)).thenReturn(2);

			when(br.readLine()).thenReturn("");
			Whitebox.setInternalState(spy, "br", br);
			
			spy.ParseLine_edited(null, str, null, 0);
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
		}
	}
	
	@Test
	public void testParseLine_edited_Is_SETEXT_HEAD3() {
		BufferedReader br = mock(BufferedReader.class);

		Parser spy = spy(p);
		try
		{
			String str = "";
			when(spy.Is_SETEXT_HEAD(str)).thenReturn(1);

			when(br.readLine()).thenThrow(new IOException());
			Whitebox.setInternalState(spy, "br", br);
			
			doReturn(1).when(spy).exit(1);

			spy.ParseLine_edited(null, str, null, 0);
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
		}
	}
	
	@Test
	public void testParseLine_edited_Is_CodeBlock() {
		BufferedReader br = mock(BufferedReader.class);

		Parser spy = spy(p);
		try
		{
			String str = "	";

			when(spy.NextLine()).thenReturn("	plain").thenReturn("	").thenReturn(null);
			when(br.readLine()).thenReturn("	");
			Whitebox.setInternalState(spy, "br", br);
			
			spy.ParseLine_edited(null, str, null, 0);
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
		}
	}
	
	@Test
	public void testParseToken1() {
		Node node = Node.create("BLOCK");
		String str = "# a *b* **c** #  ";
		
		p.parseToken(node, str);
	}
	
	@Test
	public void testParseToken2() {
		Node node = Node.create("BLOCK");
		String str = "# a *b* **c** # ";
		
		p.parseToken(node, str);
	}

	@Test
	public void testParseToken3() {
		Node node = Node.create("BLOCK");
		String str = "# a *b* **c** ";
		
		p.parseToken(node, str);
	}
	
	@Test
	public void testParseToken4() {
		Node node = Node.create("BLOCK");
		String str = "# a **b** *c*";
		
		p.parseToken(node, str);
	}

	@Test
	public void testParseToken5() {
		Node node = Node.create("BLOCK");
		String str = "# a `code` ` code `";
		
		p.parseToken(node, str);
	}

	@Test
	public void testParseToken6() {
		Node node = Node.create("BLOCK");
		String str = " ` code`  ";
		
		p.parseToken(node, str);
	}

	@Test
	public void testParseToken7() {
		Node node = Node.create("BLOCK");
		String str = "**";
		
		p.parseToken(node, str);
	}
	
	@Test
	public void testParseToken8() {
		Node node = Node.create("BLOCK");
		String str = "*   ";
		
		p.parseToken(node, str);
	}

	@Test
	public void testParseToken9() {
		Node node = Node.create("BLOCK");
		String str = "";
		
		p.parseToken(node, str);
	}
	
//	@Test
//	public void testParseLine_editedStrong() {
//		Node block = Node.create("BLOCK");
//		String str = "*test*  ";
//
//		assertThat(p.ParseLine_edited(
//				block, str, "STRONG", 0),
//				is(0));
//
//	}

	@Test
	public void testParseLine() {
		fail("Not yet implemented");
	}

	@Test
	public void testNextChar() {
		fail("Not yet implemented");
	}

	@Test
	public void testNextLine() {
		fail("Not yet implemented");
	}

	@Test
	public void testBuild_BlockQutoes() {
		fail("Not yet implemented");
	}

	@Test
	public void testIs_ATX_HEADTrue() {
		String str = "# header #";
		assertTrue(p.Is_ATX_HEAD(str));
	}

	@Test
	public void testIs_ATX_HEADFalse() {
		String str = "not header";
		assertFalse(p.Is_ATX_HEAD(str));
	}

	@Test
	public void testIs_SETEXT_HEADZero() {
		Parser spy = spy(p);
		when(spy.NextLine()).thenReturn(null);
		
		String str = "header";
		assertThat(spy.Is_SETEXT_HEAD(str), is(0));
	}

	@Test
	public void testIs_SETEXT_HEADOne() {
		Parser spy = spy(p);
		when(spy.NextLine()).thenReturn("------");
		
		String str = "header";
		assertThat(spy.Is_SETEXT_HEAD(str), is(1));
	}

	@Test
	public void testIs_SETEXT_HEADTwo() {

		Parser spy = spy(p);
		when(spy.NextLine()).thenReturn("======");
		
		String str = "header";
		assertThat(spy.Is_SETEXT_HEAD(str), is(2));

	}

	@Test
	public void testIs_CodeBlockTrue() {
		String str = "\tcodeblock";
		assertTrue(p.Is_CodeBlock(str));
	}

	@Test
	public void testIs_CodeBlockFalse() {
		String str = "not codeblock";
		assertFalse(p.Is_CodeBlock(str));
	}

	@Test
	public void testIs_BlockQutoes() {
		fail("Not yet implemented");
	}

	@Test
	public void testIs_list() {
		fail("Not yet implemented");
	}

	@Test
	public void testIs_HR() {
		fail("Not yet implemented");
	}

	@Test
	public void testIsImages() {
		fail("Not yet implemented");
	}

	@Test
	public void testIsLink() {
		fail("Not yet implemented");
	}

}
