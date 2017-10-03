package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.Node;
import edu.hgu.se.teaml.Document;
import edu.hgu.se.teaml.PlainVisitor;

public class DocumentTest {

	Document document;
	
	@Before
	public void setUp(){
		document = new Document();
	}
	
	@After
	public void tearDown(){
		document = null;
	}
	
	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		document.accept(plainVisitor);
	}

	@Test
	public void testAdd() {
		 Node node = Node.create("SETEXT_HEADER");
		 document.add(node);
	}

	@Test
	public void testSetFileName() {
		document.setFileName("test.md");
	}

	@Test
	public void testGetNodes() {
		assertNotNull(document.getNodes());
	}

}
