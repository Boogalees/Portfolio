package edu.hgu.se.teaml;

public class HTMLCode extends Token
{
	private String value;
		
	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitHtmlCode(this);
	}

	public void setValue(String value) { this.value = value; }
	public String getValue() { return value; }
}
