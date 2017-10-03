package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.Link;
import edu.hgu.se.teaml.OrderedList;
import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.Strong;

public class OrderedListTest {


	private OrderedList list;
	
	@Before
	public void setUp(){
		list = new OrderedList();
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
		list = new OrderedList("test");
	}

}
