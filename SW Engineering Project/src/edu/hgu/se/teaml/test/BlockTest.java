package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.Test;

import edu.hgu.se.teaml.Block;
import edu.hgu.se.teaml.PlainVisitor;

public class BlockTest {

	@Test
	public void testAccept() {
		Block block = new Block();
		PlainVisitor plainVisitor = new PlainVisitor();
		block.accept(plainVisitor);
	}

	@Test
	public void testBlock() {
		assertNotNull(new Block());
	}

}
