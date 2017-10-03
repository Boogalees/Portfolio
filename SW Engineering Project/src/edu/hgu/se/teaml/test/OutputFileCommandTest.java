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
import edu.hgu.se.teaml.OutputFileCommand;

public class OutputFileCommandTest {
	private OutputFileCommand c;
	
	@Before
	public void setUp() {
		c = new OutputFileCommand();	
	}
	
	@After
	public void tearDown() {
		c = null;
	}
	
	@Test
	public void testSetOptionsWrongBoundary() {
		String[] args = new String[]{ "-i", "one.md", "-o" };

		try
		{
			c.setOptions(args, 3, 3);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: no output file name given");
		}
	}

	@Test
	public void testSetOptionsWrongLength() {
		String[] args = new String[]{ "-i", "one.md", "-o", "hi" };
		try
		{
			c.setOptions(args, 3, 4);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: invalid output file name");
		}
	}

	@Test
	public void testSetOptionsWrongExtension() {
		String[] args = new String[]{ "-i", "one.md", "-o", "invalidname" };
		try
		{
			c.setOptions(args, 3, 4);
		}
		catch(CommandException e)
		{
			assertEquals(e.getMessage(), "CommandException: invalid output file name");
		}
	}
	
	@Test
	public void testSetOptions() {
		String[] args = new String[]{ "-i", "one.md", "-o", "one.html" };
		try
		{
			c.setOptions(args, 3, 4);
		}
		catch(Exception e)
		{
			fail();
		}
		
		assertEquals("one.html", c.getOptions().get(0));
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
		assertEquals("one.html", c.getOptions().get(0));
		
		File f1 = new File("one.md");
		f1.delete();
	}
	
	@Test
	public void testCheckDuplicatesNoDuplicates() {
		String[] args = new String[]{ "-i", "one.md", "two.md",
				"-o", "one.html", "two.html" };
		try
		{
			c.setOptions(args, 4, 6);
			c.checkDuplicates();
		}
		catch(CommandException e)
		{
			fail();
		}
	}

	@Test
	public void testCheckDuplicatesDuplicatesExist() {
		String[] args = new String[]{ "-i", "one.md", "two.md",
				"-o", "dup.html", "dup.html" };
		try
		{
			c.setOptions(args, 4, 6);
			c.checkDuplicates();
		}
		catch(CommandException e)
		{
			assertEquals("CommandException: duplicated output file names exist",
					e.getMessage());
		}
	}

}
