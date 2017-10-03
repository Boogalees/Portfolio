package edu.hgu.se.teaml;
public class CommandException extends Exception
{
	public CommandException(String message)
	{
		super("CommandException: " + message);
	}
}