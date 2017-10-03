package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.Test;

import edu.hgu.se.teaml.AtxHeader;
import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.SetextHeader;

public class AtxHeaderTest {

	@Test
	public void testDefaultConstructor() {
		assertNotNull(new AtxHeader());
	}
	
	@Test
	public void testAccept(){
		AtxHeader atxHeader = new AtxHeader();
		PlainVisitor plainVisitor = new PlainVisitor();
		atxHeader.accept(plainVisitor);
	}

}
