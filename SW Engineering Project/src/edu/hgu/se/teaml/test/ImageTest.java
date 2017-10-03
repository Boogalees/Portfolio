package edu.hgu.se.teaml.test;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.HashHandler;
import edu.hgu.se.teaml.HashObject;
import edu.hgu.se.teaml.Image;
import edu.hgu.se.teaml.PlainVisitor;

public class ImageTest {

	Image image;
	
	@Before
	public void setUp(){
		image = new Image();
	}
	
	@After
	public void tearDown(){
		image = null;
	}
	
	@Test
	public void testAccept() {
		PlainVisitor plainVisitor = new PlainVisitor();
		image.accept(plainVisitor);
	}

	@Test
	public void testDefaultConstructor() {
		assertNotNull(new Image());
	}

	@Test
	public void testIsReference() {
		image.setId("id");
		assertTrue(image.isReference(null));
	}

	@Test
	public void testIsMatched() {
		image.setId("id");
		HashHandler hashHandler = new HashHandler();
		assertFalse(image.isMatched(hashHandler));
	}

	@Test
	public void testSetParametersHashContain() {
		image.setId("test");
		HashObject hashObject = new HashObject();
		hashObject.setUrl("test.jpg");
		hashObject.setTitle("test");
		HashHandler hashHandler = new HashHandler();
		hashHandler.put(image.getId(), hashObject);
		image.setParameters(hashHandler);
	}
	
	@Test
	public void testSetParametersHashNotContain() {
		image.setId("test");
		HashObject hashObject = new HashObject("test.jpg", "test");
		HashHandler hashHandler = new HashHandler();
		hashHandler.put("id", hashObject);
		image.setParameters(hashHandler);
	}
	

	@Test
	public void testSetGetId() {
		image.setId("id");
		image.getId();
	}

	@Test
	public void testSetGetSrc() {
		image.setSrc("src");
		image.getSrc();
	}

	@Test
	public void testSetGetAlt() {
		image.setAlt("alt");
		image.getAlt();
	}

	@Test
	public void testSetGetTitle() {
		image.setTitle("title");
		image.getTitle();
	}
}
