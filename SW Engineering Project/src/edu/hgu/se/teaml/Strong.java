package edu.hgu.se.teaml;

public class Strong extends StyleText
{

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitStrong(this);
	}
}
