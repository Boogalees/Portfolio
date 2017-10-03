package edu.hgu.se.teaml.test;
import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.MDElement;
import edu.hgu.se.teaml.Node;
import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.SetextHeader;

public class NodeTest {
	
	@Test
	public void testAccept(){
		Node node = new Node();
		PlainVisitor plainVisitor = new PlainVisitor();
		node.accept(plainVisitor);
	}

	@Test
	public void testCreateNotNull() {
		assertNotNull("create() considers \"HEADER\" as null", Node.create("HEADER"));
		assertNotNull("create() considers \"HR\" as null", Node.create("HR"));
		assertNotNull("create() considers \"BR\" as null", Node.create("BR"));
		assertNotNull("create() considers \"SETEXT_HEADER\" as null", Node.create("SETEXT_HEADER"));
		assertNotNull("create() considers \"ATX_HEADER\" as null", Node.create("ATX_HEADER"));
		assertNotNull("create() considers \"BLOCK\" as null", Node.create("BLOCK"));
		assertNotNull("create() considers \"QUOTED_BLOCK\" as null", Node.create("QUOTED_BLOCK"));
		assertNotNull("create() considers \"ITEM_LIST\" as null", Node.create("ITEM_LIST"));
		assertNotNull("create() considers \"ORDERED_LIST\" as null", Node.create("ORDERED_LIST"));
		assertNotNull("create() considers \"UNORDERED_LIST\" as null", Node.create("UNORDERED_LIST"));
		assertNotNull("create() considers \"CODE_BLOCK\" as null", Node.create("CODE_BLOCK"));
	}
	
	@Test
	public void testCreateNull(){
		assertNull("create() considers \"HEADET\" as not null", Node.create("HEADET"));
	}

	@Test
	public void testAddNullElement() {
		Node node = new Node();
		MDElement element = null;
		assertFalse("add() can add null object in \"elements\" attribute", node.add(element));
	}
	
	@Test
	public void testAddNotNullElement(){
		Node node = new Node();
		MDElement element = Node.create("HEADER");
		assertTrue(node.add(element));
	}

	@Test
	public void testGetElements() {
		Node node = new Node();
		assertNotNull("Node class cannot initiate \"elements\" attribute", node.getElements());
	}

	@Test
	public void testGetTypeNotNull() {
		Node node = new Node();
		node = Node.create("HEADER");
		assertNotNull(node.getType());
	}
	
	@Test(expected = NullPointerException.class)
	public void testGetTypeNull(){
		Node node = new Node();
		node = Node.create("HEADET");
		assertNull(node.getType());
	}
	

}
