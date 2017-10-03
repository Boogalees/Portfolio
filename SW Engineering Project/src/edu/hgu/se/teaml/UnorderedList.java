package edu.hgu.se.teaml;

public class UnorderedList extends ItemList
{
	public UnorderedList() {}

	public UnorderedList(String string)
	{
		super(string);
	}
	
	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitUnorderedList(this);
	}
	
	
}
