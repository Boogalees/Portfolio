package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.Header;
import edu.hgu.se.teaml.PlainVisitor;

public class HeaderTest {
	
	Header header;
	
	@Before
	public void setUp(){
		header = new Header();
	}
	
	@After
	public void tearDown(){
		header = null;
	}

	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		header.accept(plainVisitor);
	}

	@Test
	public void testSetGetSize() {
		header.setSize(10);
		assertEquals(10, header.getSize());
	}

}
