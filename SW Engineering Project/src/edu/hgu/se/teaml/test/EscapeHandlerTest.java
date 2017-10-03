package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;
import edu.hgu.se.teaml.EscapeHandler;

import org.junit.Test;

public class EscapeHandlerTest {

	private EscapeHandler escapeHandler = new EscapeHandler();
	private String str = "AT&amp;T";

	@Test
	public void testAlreadyEscaped() {
		assertTrue(EscapeHandler.alreadyEscaped(str));
	}

	@Test
	public void testForceEscape() {
		assertEquals("AT&amp;amp;T", EscapeHandler.forceEscape(str));
	}

	@Test
	public void testNaturallyEscape() {
		String toEscape = "AT&T";
		String noEscape = "AT&amp;T";
		
		assertEquals("AT&amp;T", EscapeHandler.naturallyEscape(toEscape));
		assertEquals("AT&amp;T", EscapeHandler.naturallyEscape(noEscape));
	}
}
