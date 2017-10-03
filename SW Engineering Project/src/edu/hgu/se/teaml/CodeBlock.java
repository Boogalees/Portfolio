package edu.hgu.se.teaml;

public class CodeBlock extends Node
{
	public CodeBlock() {}

	public CodeBlock(String string)
	{
		super(string);
	}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitCodeBlock(this);
	}
	
}
