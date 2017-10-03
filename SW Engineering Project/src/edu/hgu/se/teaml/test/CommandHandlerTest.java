package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.PrintStream;
import java.io.PrintWriter;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import static org.hamcrest.CoreMatchers.*;

import edu.hgu.se.teaml.CommandException;
import edu.hgu.se.teaml.CommandHandler;
import edu.hgu.se.teaml.InputFileCommand;

public class CommandHandlerTest {

	private CommandHandler ch;
	
	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
		PrintWriter pw1 = new PrintWriter("one.md");
		pw1.println("");
		pw1.close();

		PrintWriter pw2 = new PrintWriter("two.md");
		pw2.println("");
		pw2.close();
	}
	
	@AfterClass
	public static void tearDownAfterClass() throws Exception {
		File f1 = new File("one.md");
		File f2 = new File("two.md");
		File f3 = new File("one.html");
		File f4 = new File("two.html");
		f1.delete();
		f2.delete();
		f3.delete();
		f4.delete();
	}
	
	@Before
	public void setUp() {
		ch = new CommandHandler();
	}
	
	@After
	public void tearDown() {
		ch = null;
	}

	@Test
	public void testGetNumOfFiles() {
		String[] args = new String[]{ "-i", "one.md", "two.md" };
		ch.setCommands(args);
		
		assertThat(ch.getNumOfFiles(), is(2));
	}

	@Test
	public void testGetInputFileNames() {
		String[] args = new String[]{ "-i", "one.md", "two.md" };
		ch.setCommands(args);
		
		assertEquals(ch.getInputFileNames().get(0), "one.md");
		assertEquals(ch.getInputFileNames().get(1), "two.md");
	}

	@Test
	public void testGetOutputFileNames() {
		String[] args = new String[]{ "-i", "one.md", "two.md" };
		ch.setCommands(args);
		
		assertEquals(ch.getOutputFileNames().get(0), "one.html");
		assertEquals(ch.getOutputFileNames().get(1), "two.html");
	}

	@Test
	public void testGetStyleNames() {
		String[] args = new String[]{ "-i", "one.md", "two.md", "-style", "fancy" };
		ch.setCommands(args);
		
		assertEquals(ch.getStyleNames().get(0), "fancy");
		assertEquals(ch.getStyleNames().get(1), "plain");
	}

	@Test
	public void testSetCommandsHelp() {
		String[] args = new String[]{ "--help", "-i", "one.md" };

		assertFalse(ch.setCommands(args));
	}

	@Test
	public void testSetCommandsInvalidCommandOption2() {
		final ExpectedException exception = ExpectedException.none();
		String[] args = new String[]{ "invalidcommand" };
		ch.setCommands(args);
		
		exception.expect(CommandException.class);
		exception.expectMessage("CommandException: invalid command option");
	}

	@Test
	public void testSetCommandsInvalidCommandOption1() {
		final ExpectedException exception = ExpectedException.none();
		String[] args = new String[]{ "-invalidcommand" };
		ch.setCommands(args);
		
		exception.expect(CommandException.class);
		exception.expectMessage("CommandException: invalid command option");
	}

	@Test
	public void testSetCommandsInput() {
		String[] args = new String[]{ "-i", "one.md" };
		ch.setCommands(args);

		assertNotNull(ch.getInputFileNames());
		assertNotNull(ch.getOutputFileNames());
		assertNotNull(ch.getStyleNames());

		assertThat(ch.getInputFileNames().size(), is(1));
		assertThat(ch.getOutputFileNames().size(), is(1));
		assertThat(ch.getStyleNames().size(), is(1));
		
		assertEquals(ch.getInputFileNames().get(0), "one.md");
		assertEquals(ch.getOutputFileNames().get(0), "one.html");
		assertEquals(ch.getStyleNames().get(0), "plain");
	}

	@Test
	public void testSetCommandsInputOutput() {
		String[] args = new String[]{ "-i", "one.md", "-o", "two.html" };
		ch.setCommands(args);

		assertNotNull(ch.getInputFileNames());
		assertNotNull(ch.getOutputFileNames());
		assertNotNull(ch.getStyleNames());
		
		assertThat(ch.getInputFileNames().size(), is(1));
		assertThat(ch.getOutputFileNames().size(), is(1));
		assertThat(ch.getStyleNames().size(), is(1));
		
		assertEquals(ch.getInputFileNames().get(0), "one.md");
		assertEquals(ch.getOutputFileNames().get(0), "two.html");
		assertEquals(ch.getStyleNames().get(0), "plain");
	}

	@Test
	public void testSetCommandsInputStyle() {
		String[] args = new String[]{ "-i", "one.md", "-style", "fancy" };
		ch.setCommands(args);
		
		assertNotNull(ch.getInputFileNames());
		assertNotNull(ch.getOutputFileNames());
		assertNotNull(ch.getStyleNames());
		
		assertThat(ch.getInputFileNames().size(), is(1));
		assertThat(ch.getOutputFileNames().size(), is(1));
		assertThat(ch.getStyleNames().size(), is(1));
		
		assertEquals(ch.getInputFileNames().get(0), "one.md");
		assertEquals(ch.getOutputFileNames().get(0), "one.html");
		assertEquals(ch.getStyleNames().get(0), "fancy");
	}

	@Test
	public void testSetCommandsNoInputSpecified1() {
		final ExpectedException exception = ExpectedException.none();
		
		String[] args = new String[]{ "-i" };
		ch.setCommands(args);
		
		exception.expect(CommandException.class);
		exception.expectMessage("CommandException: no input file specified");
	}

	@Test
	public void testSetCommandsNoInputSpecified2() {
		final ExpectedException exception = ExpectedException.none();
		
		String[] args = new String[]{ "-o", "output.html" };
		ch.setCommands(args);
		
		exception.expect(CommandException.class);
		exception.expectMessage("CommandException: no input file specified");
	}

	@Test
	public void testSetCommandsTooManyOutputsSpecified() {
		final ExpectedException exception = ExpectedException.none();

		String[] args = new String[]{ "-i", "one.md", "-o", "one.html", "two.html" };
		ch.setCommands(args);
		
		exception.expect(CommandException.class);
		exception.expectMessage("CommandException: too many output files specified");
	}

	@Test
	public void testSetCommandsTooManyStylesSpecified() {
		final ExpectedException exception = ExpectedException.none();

		String[] args = new String[]{ "-i", "one.md", "-style", "plain", "fancy" };
		ch.setCommands(args);
		
		exception.expect(CommandException.class);
		exception.expectMessage("CommandException: too may styles specified");
	}

	@Test
	public void testPrintHelp() {
		ch.printHelp();
	}

	@Test
	public void testIsHelp1() {
		String[] args = new String[]{ "-i", "one.md", "--help" };
		ch.setCommands(args);
		
		assertTrue(ch.isHelp(args));
	}

	@Test
	public void testIsHelp2() {
		String[] args = new String[]{ "-i", "one.md" };
		ch.setCommands(args);
		
		assertFalse(ch.isHelp(args));
	}

	@Test
	public void testPrintOptions1() {
		String[] args = new String[]{ "--help" };
		ch.setCommands(args);
		ch.printOptions();
	}

	@Test
	public void testPrintOptions2() {
		String[] args = new String[]{ "-i", "one.md" };
		ch.setCommands(args);
		ch.printOptions();
	}
}
