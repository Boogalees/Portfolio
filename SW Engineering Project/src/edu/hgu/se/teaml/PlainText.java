package edu.hgu.se.teaml;
import java.util.LinkedList;
import java.util.List;

public class PlainText extends Token
{
	private String value;
	
	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitPlainText(this);
	}
	
	public void setValue(String value) { this.value = value; }
	public String getValue() { return value; }
}
