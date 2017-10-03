package edu.hgu.se.teaml.test;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.Strong;

public class StrongTest {

	Strong strong;
	PlainVisitor plainVisitor;
	
	@Before
	public void setUp(){
		strong = new Strong();
		plainVisitor = new PlainVisitor();
	}
	
	@After
	public void tearDown(){
		strong = null;
	}
	
	@Test
	public void testAccept(){
		strong.accept(plainVisitor);
	}
	
}
