package edu.hgu.se.teaml.test;

import java.util.LinkedList;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.HtmlValidator;

public class HtmlValidatorTest {

	HtmlValidator htmlValidator;
	
	@Before
	public void setUp(){
		htmlValidator = new HtmlValidator();
	}
	
	@After
	public void tearDown(){
		htmlValidator = null;
	}
	
	
	@Test
	public void testCheckSyntax() {
		List<String> htmlFileList = new LinkedList<String>();
		htmlFileList.add("test/test.html");
		htmlValidator.checkSyntax(htmlFileList);
	}

	@Test
	public void testExecuteJTidyInputNotNull() {
		htmlValidator.executeJTidy("test/test.html");
	}

	@Test
	public void testExecuteJTidyReadHtmlFileFail() {
		htmlValidator.executeJTidy("noFile");
	}

	
}
