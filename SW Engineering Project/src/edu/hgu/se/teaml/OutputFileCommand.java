package edu.hgu.se.teaml;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

public class OutputFileCommand extends Command
{
	public void fillInEmptyOptions(InputFileCommand inputFileCommand)
	{
		ArrayList<String> inputFiles = inputFileCommand.getOptions();
		
		int startIndex = options.size(); 
		for(int i = startIndex; i < inputFiles.size(); i++)
		{
			options.add(inputFiles.get(i).substring(0, inputFiles.get(i).length() - 3) + ".html");
		}
	}
	
	public void setOptions(String[] args, int startIndex, int boundaryIndex)
			throws CommandException
	{
		if(boundaryIndex - startIndex <= 0)
		{
			throw new CommandException("no output file name given");
		}
		
		for(int i = startIndex; i < boundaryIndex; i++)
		{
			String outputFile = args[i];
			if(outputFile.length() < 6)
			{
				throw new CommandException(
						"invalid output file name");
			}
			if(!outputFile.substring(outputFile.length() - 5,
					outputFile.length()).equalsIgnoreCase(".html"))
			{
				throw new CommandException(
						"invalid output file name");
			}
			
			options.add(outputFile.toLowerCase());
		}
	}
	
	public void checkDuplicates()
			throws CommandException
	{
		Set<String> set = new HashSet<String>(options);
		
		if(set.size() < options.size())
		{
			throw new CommandException(
					"duplicated output file names exist");
		}
	}
}