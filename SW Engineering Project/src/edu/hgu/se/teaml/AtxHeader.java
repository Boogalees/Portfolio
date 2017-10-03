package edu.hgu.se.teaml;

public class AtxHeader extends Header
{
	public AtxHeader() {}

	public AtxHeader(String string)
	{
		super(string);
	}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitAtxHeader(this);
	}
}
