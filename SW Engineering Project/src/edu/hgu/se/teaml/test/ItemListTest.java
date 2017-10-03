package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.Test;

import edu.hgu.se.teaml.ItemList;

public class ItemListTest {

	@Test
	public void testDefaultConstructor() {
		assertNotNull(new ItemList());
	}

}
