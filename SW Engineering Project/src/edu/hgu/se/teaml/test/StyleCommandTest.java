package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import java.io.File;
import java.io.PrintWriter;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import static org.hamcrest.CoreMatchers.*;

import edu.hgu.se.teaml.CommandException;
import edu.hgu.se.teaml.InputFileCommand;
import edu.hgu.se.teaml.StyleCommand;

public class StyleCommandTest {
	private StyleCommand c;
	
	@Before
	public void setUp() {
		c = new StyleCommand();	
	}
	
	@After
	public void tearDown() {
		c = null;
	}

	@Test
	public void testSetOptionsWrongBoundary() {
		String[] args = new String[]{ "-i", "one.md", "-style" };

		try
		{
			c.setOptions(args, 3, 3);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: no style option given");
		}
	}
	
	@Test
	public void testSetOptionsWrongLength() {
		String[] args = new String[]{ "-i", "one.md", "-style", "invalid" };
		try
		{
			c.setOptions(args, 3, 4);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: invalid html style option");
		}
	}
	
	@Test
	public void testSetOptions() {
		String[] args = new String[]{ "-i", "one.md", "-style", "fancy" };
		try
		{
			c.setOptions(args, 3, 4);
		}
		catch(Exception e)
		{
			fail();
		}
		
		assertEquals("fancy", c.getOptions().get(0));
	}
	
	@Test
	public void testFillInEmptyOptions() {
		try
		{
			PrintWriter pw1 = new PrintWriter("one.md");
			pw1.println("");
			pw1.close();
		}
		catch(Exception e) { }
		
		InputFileCommand inputFileCommand = new InputFileCommand();
		String[] args = new String[]{ "-i", "one.md" };
		try
		{
			inputFileCommand.setOptions(args, 1, 2);
		}
		catch(Exception e) { }
		
		c.fillInEmptyOptions(inputFileCommand);

		assertThat(c.getOptions().size(), is(1));
		assertEquals("plain", c.getOptions().get(0));
		
		File f1 = new File("one.md");
		f1.delete();
	}
}
