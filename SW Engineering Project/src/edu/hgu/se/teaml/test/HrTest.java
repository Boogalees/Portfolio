package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.Test;

import edu.hgu.se.teaml.Hr;
import edu.hgu.se.teaml.PlainVisitor;

public class HrTest {

	@Test
	public void testAccept() {
		Hr hr = new Hr();
		PlainVisitor plainVisitor = new PlainVisitor();
		hr.accept(plainVisitor);
	}

	@Test
	public void testHr() {
		assertNotNull(new Hr());
	}

}
