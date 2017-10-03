package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.UnorderedList;

public class UnorderedListTest {


	private UnorderedList list;
	
	@Before
	public void setUp(){
		list = new UnorderedList();
	}
	
	@After
	public void tearDown(){
		list=null;
	}
	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		list.accept(plainVisitor);
	}
	@Test
	public void testOrderedListString() {
		list = new UnorderedList("test");
	}

}
