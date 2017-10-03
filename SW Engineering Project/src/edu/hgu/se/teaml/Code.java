package edu.hgu.se.teaml;

public class Code extends Token{
	private String value;
	
	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitCode(this);
	}
	
	public void setValue(String string) { value = string; }
	public String getValue() { return value; }
}
