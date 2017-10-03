package edu.hgu.se.teaml;

public class SetextHeader extends Header
{
	public SetextHeader() {}

	public SetextHeader(String string)
	{
		super(string);
	}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitSetextHeader(this);
	}
}
