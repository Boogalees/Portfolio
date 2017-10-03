package edu.hgu.se.teaml;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class CommandHandler
{
	private int numOfFiles;

	private List<String> validCommandList = Arrays.asList("-o", "-i", "-style");

	private InputFileCommand inputFileCommand;
	private OutputFileCommand outputFileCommand;
	private StyleCommand styleCommand;
	
	public int getNumOfFiles() { return numOfFiles; }
	
	public ArrayList<String> getInputFileNames()
	{
		return inputFileCommand.getOptions();
	}

	public ArrayList<String> getOutputFileNames()
	{
		return outputFileCommand.getOptions();
	}
	
	public ArrayList<String> getStyleNames()
	{
		return styleCommand.getOptions();
	}
	
	public boolean setCommands(String[] args)
	{
		try
		{
			if(isHelp(args))
			{
				printHelp();
				return false;
			}
			
			int currentCommandIndex = -1;
			ArrayList<Integer> commandIndexList = new ArrayList<Integer>();
			for(int i = 0; i < args.length; i++)
			{
				if(args[i].charAt(0) == '-')
				{
					if(!validCommandList.contains(args[i]))
					{
						throw new CommandException("invalid command option");
					}
					commandIndexList.add(i);
					currentCommandIndex = i;
				}
				else
				{
					if(currentCommandIndex == -1)
					{
						throw new CommandException("Invalid command option");
					}
				}
			}
			
			for(int i = 0; i < commandIndexList.size(); i++)
			{
				int startIndex = commandIndexList.get(i) + 1;
				int boundaryIndex = 0;
				if(i == commandIndexList.size() - 1)
				{
					boundaryIndex = args.length;
				}
				else
				{
					boundaryIndex = commandIndexList.get(i + 1);
				}
				
				if(args[commandIndexList.get(i)].equals("-o"))
				{
					outputFileCommand = new OutputFileCommand();
					outputFileCommand.setOptions(args, startIndex, boundaryIndex);
				}
				if(args[commandIndexList.get(i)].equals("-i"))
				{
					inputFileCommand = new InputFileCommand();
					inputFileCommand.setOptions(args, startIndex, boundaryIndex);
				}
				if(args[commandIndexList.get(i)].equals("-style"))
				{
					styleCommand = new StyleCommand();
					styleCommand.setOptions(args, startIndex, boundaryIndex);
				}
			}
			
			if(inputFileCommand == null)
			{
				throw new CommandException("no input file specified");
			}
			
			if(outputFileCommand == null)
				outputFileCommand = new OutputFileCommand();
			if(styleCommand == null)
				styleCommand = new StyleCommand();
			
			outputFileCommand.fillInEmptyOptions(inputFileCommand);
			styleCommand.fillInEmptyOptions(inputFileCommand);
			
			outputFileCommand.checkDuplicates();
			
			numOfFiles = getInputFileNames().size();
			
			if(outputFileCommand.size() > inputFileCommand.size())
				throw new CommandException("too many output files specified");
			if(styleCommand.size() > inputFileCommand.size())
				throw new CommandException("too many styles specified");
		}
		catch(CommandException ex)
		{
			System.out.println(ex.getMessage());
			System.out.println("type in --help to check the usage");
			return false;
		}

		return true;
	}
	
	public void printHelp()
	{
		System.out.println("Usage: [options]");
		System.out.println("options: ");
		System.out.println("\t--help: displays this information");
		System.out.println("\t(necessary) -i [md_file]+: " +
				"specifies [md_file] to read\n" +
				"\t\t.md extension is needed in the file name");
		System.out.println("\t(optional) -o [html_file]+: " +
				"places the output as [html_file]\n" +
				"\t\tdefault setting is [input_file_name].html\n" +
				"\t\t.html extension is needed in the file name");
		System.out.println("\t(optional) -style [plain|fancy|slide]+: " +
				"sets HTML style as one of plan/fancy/slide\n" +
				"\t\tdefault setting is plain");
		System.out.println("the number of output files and styles\n" +
				"cannot exceed that of input files");
		System.out.println("");
	}

	public boolean isHelp(String[] args)
	{
		for(String arg: args)
		{
			if(arg.equals("--help"))
			{
				return true;
			}
		}
		return false;
	}
	
	public void printOptions()
	{
		if(inputFileCommand != null)
			inputFileCommand.printOptions();
		if(outputFileCommand != null)
			outputFileCommand.printOptions();
		if(styleCommand != null)
			styleCommand.printOptions();
	}
}