package edu.hgu.se.teaml;

public class Block extends Node
{
	public Block() {}

	public Block(String string)
	{
		super(string);
	}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitBlock(this);
	}
}
