package edu.hgu.se.teaml;

public interface MDElement
{
	void accept(MDElementVisitor mdElementVisitor);
	boolean add(MDElement element);
}
