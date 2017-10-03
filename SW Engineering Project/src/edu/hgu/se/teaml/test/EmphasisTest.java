package edu.hgu.se.teaml.test;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.Emphasis;
import edu.hgu.se.teaml.PlainVisitor;

public class EmphasisTest {

	Emphasis emphasis;
	
	@Before
	public void setUp(){
		emphasis = new Emphasis();
	}
	
	@After
	public void tearDown(){
		emphasis = null;
	}
	
	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		emphasis.accept(plainVisitor);
	}

}
