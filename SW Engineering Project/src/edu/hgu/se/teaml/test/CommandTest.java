package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import edu.hgu.se.teaml.Command;
import edu.hgu.se.teaml.StyleCommand;
import static org.hamcrest.CoreMatchers.*;

public class CommandTest {
	
	private Command c;
	
	@Before
	public void setUp() {
		c = new StyleCommand();
		String[] args = new String[]{ "-i", "one.md", "-style", "fancy" };
		
		try
		{
			c.setOptions(args, 3, 4);
		}
		catch(Exception ex) { }
	}
	
	@After
	public void tearDown() {
		c = null;
	}

	@Test
	public void testGetOptions() {
		List<String> options = c.getOptions();
		assertEquals(options.get(0), "fancy");
	}

	@Test
	public void testSize() {
		assertThat(c.size(), is(1));
	}

	@Test
	public void testPrintOptions() {
		ByteArrayOutputStream output = new ByteArrayOutputStream();
		System.setOut(new PrintStream(output));
		
		c.printOptions();
		String outputString = output.toString();
		outputString = outputString.replaceAll("\r\n", "");
		
		assertEquals("[fancy]", outputString);
		
		System.setOut(null);
	}

	@Test
	public void testSetOptions() {
		String[] args = new String[]{ "-i", "one.md", "-style", "fancy" };

		try
		{
			c.setOptions(args, 3, 4);
		}
		catch(Exception ex) { }
		
		assertEquals(c.getOptions().get(0), "fancy");
	}
}
