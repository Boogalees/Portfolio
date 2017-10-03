package edu.hgu.se.teaml;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class InputFileCommand extends Command
{
	public void setOptions(String[] args, int startIndex, int boundaryIndex)
			throws CommandException
	{
		if(boundaryIndex - startIndex <= 0)
		{
			throw new CommandException("no input file name given");
		}
		
		for(int i = startIndex; i < boundaryIndex; i++)
		{
			String inputFile = args[i];
			if(inputFile.length() < 4)
			{
				throw new CommandException(
						"invalid input file name");
			}
			if(!inputFile.substring(inputFile.length() - 3,
					inputFile.length()).equalsIgnoreCase(".md"))
			{
				throw new CommandException(
						"invalid input file name");
			}
			if(!new File("./", inputFile).exists())
			{
				throw new CommandException(
						inputFile + " does not exist");
			}
			
			options.add(inputFile.toLowerCase());
		}
	}
}