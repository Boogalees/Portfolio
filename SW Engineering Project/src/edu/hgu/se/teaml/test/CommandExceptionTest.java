package edu.hgu.se.teaml.test;

import static org.junit.Assert.*;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import org.junit.Test;

import edu.hgu.se.teaml.CommandHandler;

public class CommandExceptionTest {

	@Test
	public void testCommandExceptionString() {
		ByteArrayOutputStream output = new ByteArrayOutputStream();
		System.setOut(new PrintStream(output));
		
		CommandHandler commandHandler = new CommandHandler();
		commandHandler.setCommands(new String[]{ "-i" });

		String outputString = output.toString();
		outputString = outputString.replaceAll("\r\n", "");

		assertEquals("CommandException: no input file name giventype in --help to check the usage",
				outputString);

		System.setOut(null);
	}
}
