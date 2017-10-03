package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.Code;
import edu.hgu.se.teaml.PlainVisitor;

public class CodeTest {

	Code code;
	
	@Before
	public void setUp(){
		code = new Code();
	}
	
	@After
	public void tearDown(){
		code = null;
	}
	
	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		code.accept(plainVisitor);
	}

	@Test
	public void testSetGetValue() {
		code.setValue("team_l");
		assertEquals("team_l", code.getValue());
	}

}
