package edu.hgu.se.teaml.test;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;

import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.Strong;
import edu.hgu.se.teaml.StyleText;

public class StyleTextTest {
	
	StyleText styleText;
	
	@Before
	public void setUp(){
		styleText = new StyleText();
	}
	
	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		styleText.accept(plainVisitor);
	}

	@Test
	public void testGetTokens() {
		StyleText styleText = new StyleText();
		assertNotNull(styleText.getTokens());
	}

	@Test
	public void testAddTokenToken() {
		StyleText strong = new Strong();
		styleText.addToken(strong);
	}

	@Test
	public void testAddTokenString() {
		styleText.addToken("team_l");
	}

}
