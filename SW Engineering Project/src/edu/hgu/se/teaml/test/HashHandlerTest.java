package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.HashHandler;
import edu.hgu.se.teaml.HashObject;

public class HashHandlerTest {

	HashHandler hashHandler;
	
	@Before
	public void setUp(){
		hashHandler = new HashHandler();
	}
	
	@After
	public void tearDown(){
		hashHandler = null;
	}
	
	@Test
	public void testGet() {
		assertNull(hashHandler.get("id"));
	}

	@Test
	public void testContains() {
		assertFalse(hashHandler.contains("id"));
	}

	@Test
	public void testGetHrefNotNull() {
		HashObject hashObject = new HashObject("google.com", "google");
		hashHandler.put("google", hashObject);
		assertNotNull(hashHandler.getHref("google"));
	}
	
	@Test
	public void testGetHrefNull() {
		assertNull(hashHandler.getHref("google"));
	}

	@Test
	public void testGetSrcNotNull() {
		HashObject hashObject = new HashObject("google.com", "google");
		hashHandler.put("google", hashObject);
		assertNotNull(hashHandler.getSrc("google"));
	}
	
	@Test
	public void testGetSrcNull() {
		assertNull(hashHandler.getSrc("google"));	
	}

	@Test
	public void testGetTitleNotNull() {
		HashObject hashObject = new HashObject("google.com", "google");
		hashHandler.put("google", hashObject);
		assertNotNull(hashHandler.getTitle("google"));
	}
	
	@Test
	public void testGetTitleNull() {
		assertNull(hashHandler.getTitle("google"));
	}

	@Test
	public void testPut() {

		HashObject hashObject = new HashObject("google.com", "google");
		hashHandler.put("google", hashObject);

	}

}
