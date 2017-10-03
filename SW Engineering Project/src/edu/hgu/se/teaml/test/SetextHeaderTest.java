package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.Test;

import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.SetextHeader;

public class SetextHeaderTest {

	@Test
	public void testDefaultConstructor() {
		assertNotNull(new SetextHeader());
	}

	@Test
	public void testAccept(){
		SetextHeader setextHeader = new SetextHeader();
		PlainVisitor plainVisitor = new PlainVisitor();
		setextHeader.accept(plainVisitor);
	}
}
