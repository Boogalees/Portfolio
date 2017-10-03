package edu.hgu.se.teaml;

public class Br extends Node
{
	public Br() {}
	
	public Br(String string)
	{
		super(string);
	}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitBr(this);
	}
}
