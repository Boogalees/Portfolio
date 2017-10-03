package edu.hgu.se.teaml;

public class Emphasis extends StyleText
{

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitEmphasis(this);
	}
}
