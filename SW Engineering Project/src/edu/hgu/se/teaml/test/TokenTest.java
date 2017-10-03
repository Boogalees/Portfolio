package edu.hgu.se.teaml.test;
import org.junit.Test;

import edu.hgu.se.teaml.MDElementVisitor;
import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.Token;

public class TokenTest {

	@Test
	public void testAccept() {
		Token token = new Token();
		MDElementVisitor mdElementVisitor = new PlainVisitor();
		token.accept(mdElementVisitor);
	}

}
