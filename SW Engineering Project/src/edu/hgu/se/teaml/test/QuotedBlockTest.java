package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.Test;

import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.QuotedBlock;
import edu.hgu.se.teaml.SetextHeader;

public class QuotedBlockTest {

	@Test
	public void testDefaultConstructor() {
		assertNotNull(new QuotedBlock());
	}
	
	@Test
	public void testAccept(){
		QuotedBlock quotedBlock = new QuotedBlock();
		PlainVisitor plainVisitor = new PlainVisitor();
		quotedBlock.accept(plainVisitor);
	}

}
