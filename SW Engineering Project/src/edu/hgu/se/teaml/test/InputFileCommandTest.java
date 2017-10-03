package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import java.io.File;
import java.io.PrintWriter;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.rules.ExpectedException;

import edu.hgu.se.teaml.CommandException;
import edu.hgu.se.teaml.InputFileCommand;

public class InputFileCommandTest {
	private InputFileCommand c;
	
	@Before
	public void setUp() {
		c = new InputFileCommand();	
	}
	
	@After
	public void tearDown() {
		c = null;
	}

	@Test
	public void testSetOptionsWrongBoundary() {
		String[] args = new String[]{ "-i" };

		try
		{
			c.setOptions(args, 1, 1);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: no input file name given");
		}
	}

	@Test
	public void testSetOptionsWrongLength() {
		String[] args = new String[]{ "-i", "hi" };
		try
		{
			c.setOptions(args, 1, 2);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: invalid input file name");
		}
	}

	@Test
	public void testSetOptionsWrongExtension() {
		String[] args = new String[]{ "-i", "hello" };
		try
		{
			c.setOptions(args, 1, 2);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: invalid input file name");
		}
	}

	@Test
	public void testSetOptionsNoSuchFile() {
		String[] args = new String[]{ "-i", "#!!*@.md" };
		try
		{
			c.setOptions(args, 1, 2);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: " + args[1] + " does not exist");
		}
	}
	
	@Test
	public void testSetOptions() {
		try
		{
			PrintWriter pw1 = new PrintWriter("one.md");
			pw1.println("");
			pw1.close();
		}
		catch(Exception e) { }
		
		String[] args = new String[]{ "-i", "one.md" };
		try
		{
			c.setOptions(args, 1, 2);
		}
		catch(Exception e)
		{
			fail();
		}
		
		assertEquals("one.md", c.getOptions().get(0));
		
		File f1 = new File("one.md");
		f1.delete();
	}
}
