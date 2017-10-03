package edu.hgu.se.teaml;
import java.util.LinkedList;
import java.util.List;

public class StyleText extends Token
{
	protected List<MDElement> tokens = new LinkedList<MDElement>();
	
	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitStyleText(this);
	}
	
	public List<MDElement> getTokens() { return tokens; }
	
	public void addToken(Token token) { tokens.add(token); }
	
	public void addToken(String value)
	{
		PlainText plainText = new PlainText();
		plainText.setValue(value);
		
		tokens.add(plainText);
	}
}
