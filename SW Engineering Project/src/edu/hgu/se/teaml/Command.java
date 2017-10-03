package edu.hgu.se.teaml;
import java.util.ArrayList;
import java.util.Arrays;

public abstract class Command
{
	protected ArrayList<String> options = new ArrayList<String>();
	
	public ArrayList<String> getOptions()
	{
		return options;
	}
	
	public int size()
	{
		return options.size();
	}
	
	public void printOptions()
	{
		System.out.println(Arrays.toString(options.toArray()));
	}
	
	abstract public void setOptions(String[] args, int startIndex, int boundaryIndex)
			throws CommandException;
}