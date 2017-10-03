package edu.hgu.se.teaml.test;

import static org.junit.Assert.assertSame;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.PlainText;
import edu.hgu.se.teaml.PlainVisitor;

public class PlainTextTest {

	PlainText plainText;
	
	@Before
	public void setUp(){
		plainText = new PlainText(); 
	}
	
	@After
	public void tearDown(){
		plainText = null;
	}
	
	@Test
	public void testAccept(){
		PlainVisitor plainVisitor = new PlainVisitor();
		plainText.accept(plainVisitor);
	}
	
	@Test
	public void testSetGetValue(){
		plainText.setValue("team_l");
		assertSame("team_l", plainText.getValue());
	}


}
