package edu.hgu.se.teaml;

public class Header extends Node
{
	protected int size;
	
	public Header() {}
	
	public Header(String string)
	{
		super(string);
	}

	public int getSize()
	{
		return size;
	}
	
	public void setSize(int size)
	{
		this.size = size;
	}
	
	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitHeader(this);
	}
}
