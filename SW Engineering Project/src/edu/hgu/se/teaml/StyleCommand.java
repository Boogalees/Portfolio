package edu.hgu.se.teaml;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class StyleCommand extends Command
{
	private List<String> validStyleList = Arrays.asList(
			"plain", "fancy", "slide");
	
	public void fillInEmptyOptions(InputFileCommand inputFileCommand)
	{
		ArrayList<String> inputFiles = inputFileCommand.getOptions();
		
		int startIndex = options.size();
		for(int i = startIndex; i < inputFiles.size(); i++)
		{
			options.add("plain");
		}
	}
	
	public void setOptions(String[] args, int startIndex, int boundaryIndex)
			throws CommandException
	{
		if(boundaryIndex - startIndex <= 0)
		{
			throw new CommandException("no style option given");
		}
		
		for(int i = startIndex; i < boundaryIndex; i++)
		{
			String style = args[i];
			if(!validStyleList.contains(style.toLowerCase()))
			{
				throw new CommandException(
						"invalid html style option");
			}
			
			options.add(style.toLowerCase());
		}
	}
}