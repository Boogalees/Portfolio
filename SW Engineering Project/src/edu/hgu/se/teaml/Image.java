package edu.hgu.se.teaml;

public class Image extends Token
{
	private String id = "";
	private String src = ""; // check using isEmpty()
	private String alt = "";
	private String title = "";
	
	public Image() {}

	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitImage(this);
	}
	
	//TODO: isReference don't have to has parameter.
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
			src = hashHandler.getSrc(id);
			title = hashHandler.getTitle(id);
		}
	}
	
	public String getId() { return id; }
	public String getSrc() { return src; }
	public String getAlt() { return alt; }
	public String getTitle() { return title; }
	
	public void setId(String id) { this.id = id; }
	public void setSrc(String string) { src = string; }
	public void setAlt(String string) { alt = string; }
	public void setTitle(String string) { title = string; }
}
