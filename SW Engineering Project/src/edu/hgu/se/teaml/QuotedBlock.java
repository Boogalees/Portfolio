package edu.hgu.se.teaml;

public class QuotedBlock extends Node
{
	public QuotedBlock() {}

	public QuotedBlock(String string)
	{
		super(string);
	}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitQuotedBlock(this);
	}
}
