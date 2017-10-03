package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.HTMLCode;
import edu.hgu.se.teaml.PlainVisitor;

public class HTMLCodeTest {

	HTMLCode htmlCode;
	
	@Before
	public void setUp(){
		htmlCode = new HTMLCode();
	}
	
	@After
	public void tearDown(){
		htmlCode = null;
	}
	
	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		htmlCode.accept(plainVisitor);
	}

	@Test
	public void testsetGetValue() {
		htmlCode.setValue("team_l");
		assertEquals("team_l", htmlCode.getValue());
	}

}
