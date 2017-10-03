package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.HashObject;

public class HashObjectTest {

	
	@Test
	public void testDefaultConstructor(){
		assertNotNull(new HashObject());
	}
	
	@Test
	public void testStringConstructor() {
		assertNotNull(new HashObject("url"));
	}

	@Test
	public void testStringStringConstructor() {
		assertNotNull(new HashObject("url", "title"));
	}

	@Test
	public void testSetGetUrl() {
		HashObject hashObject = new HashObject();
		hashObject.setUrl("url");
		assertEquals("url", hashObject.getUrl());
	}

	@Test
	public void testSetGetTitle() {
		HashObject hashObject = new HashObject();
		hashObject.setTitle("url");
		assertEquals("url", hashObject.getTitle());
	}

}
