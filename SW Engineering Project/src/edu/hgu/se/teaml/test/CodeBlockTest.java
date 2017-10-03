package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.Test;

import edu.hgu.se.teaml.CodeBlock;
import edu.hgu.se.teaml.PlainVisitor;

public class CodeBlockTest {

	@Test
	public void testAccept() {
		CodeBlock codeBlock = new CodeBlock();
		PlainVisitor plainVisitor = new PlainVisitor();
		codeBlock.accept(plainVisitor);
	}

	@Test
	public void testDefaultConstructor() {
		assertNotNull(new CodeBlock());
	}

}
