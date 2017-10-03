package edu.hgu.se.teaml;

public class OrderedList extends ItemList
{
	public OrderedList() {}

	public OrderedList(String string)
	{
		super(string);
	}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitOrderedList(this);
	}
}
