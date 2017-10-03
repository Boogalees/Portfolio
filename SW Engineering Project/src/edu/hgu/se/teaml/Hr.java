package edu.hgu.se.teaml;

public class Hr extends Node{
	public Hr() {}
	
	public Hr(String string)
	{
		super(string);
	}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitHr(this);
	}
}
