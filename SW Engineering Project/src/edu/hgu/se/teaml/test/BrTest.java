package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.Test;

import edu.hgu.se.teaml.Br;
import edu.hgu.se.teaml.PlainVisitor;

public class BrTest {

	@Test
	public void testAccept() {
		Br br = new Br();
		PlainVisitor plainVisitor = new PlainVisitor();
		br.accept(plainVisitor);
	}

	@Test
	public void testBr() {
		assertNotNull(new Br());
	}

}
