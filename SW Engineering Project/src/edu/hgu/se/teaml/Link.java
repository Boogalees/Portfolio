package edu.hgu.se.teaml;
import java.util.LinkedList;
import java.util.List;

public class Link extends Token
{
	private String id = "";
	private String href = "";
	private List<MDElement> tokens = new LinkedList<MDElement>();
	private String title = "";
	
	public Link() {}
	
	public void setToken(Token token)
	{
		tokens.add(token);
	}
	
	public boolean add(MDElement element){
		tokens.add(element);
		return true;
	
	}
	
	public void addToken(Token token)
	{
		tokens.add(token);
	}
	
	public void addToken(String string)
	{
		PlainText plainText = new PlainText();
		plainText.setValue(string);
		tokens.add(plainText);
		//System.out.println("token: " + plainText.getValue());
	}
		
	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitLink(this);
	}
	
	public boolean isReference(HashHandler hashHandler)
	{
		return !id.isEmpty();
	}
	
	public boolean isMatched(HashHandler hashHandler)
	{
		return hashHandler.contains(id);
	}
	
	public void setParameters(HashHandler hashHandler)
	{
		if(hashHandler.contains(id))
		{
			href = hashHandler.getHref(id);
			title = hashHandler.getTitle(id);
		}
	}
	
	public String getId() { return id; }
	public String getHref() { return href; }
	public List<MDElement> getTokens() { return tokens; }
	public String getTitle() { return title; }
	
	public void setId(String id) { this.id = id; }
	public void setHref(String string) { href = string; }
	public void setTitle(String string) { title = string; }
}
