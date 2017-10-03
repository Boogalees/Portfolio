package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.HashHandler;
import edu.hgu.se.teaml.HashObject;
import edu.hgu.se.teaml.Link;
import edu.hgu.se.teaml.PlainVisitor;
import edu.hgu.se.teaml.Strong;

public class LinkTest {

	private Link link;
	
	@Before
	public void setUp(){
		link = new Link();
	}
	
	@After
	public void tearDown(){
		link = null;
	}
	
	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		link.accept(plainVisitor);
	}

	@Test
	public void testSetToken() {
		Strong strong = new Strong();
		link.setToken(strong);
	}

	@Test
	public void testAddTokenToken() {
		Strong strong = new Strong();
		link.addToken(strong);
	}

	@Test
	public void testAddTokenString() {
		link.addToken("plain text");
	}

	@Test
	public void testIsReference() {
		HashHandler hashHandler = new HashHandler();
		assertFalse(link.isReference(hashHandler));
	}

	@Test
	public void testIsMatched() {
		link.setId("id");
		HashHandler hashHandler = new HashHandler();
		assertFalse(link.isMatched(hashHandler));
	}

	@Test
	public void testSetParametersHashContain() {
		link.setId("test");
		HashObject hashObject = new HashObject("test.jpg", "test");
		HashHandler hashHandler = new HashHandler();
		hashHandler.put(link.getId(), hashObject);
		link.setParameters(hashHandler);
	}
	
	@Test
	public void testSetParametersHashNotContain() {
		link.setId("test");
		HashObject hashObject = new HashObject("test.jpg", "test");
		HashHandler hashHandler = new HashHandler();
		hashHandler.put("id", hashObject);
		link.setParameters(hashHandler);
	}

	@Test
	public void testSetGetId() {
		link.setId("id");
		link.getId();
	}

	@Test
	public void testSetGetHref() {
		link.setHref("href");
		link.getHref();
	}

	@Test
	public void testGetTokens() {
		link.getTokens();
	}

	@Test
	public void testSetGetTitle() {
		link.setTitle("title");
		link.getTitle();
	}


}
