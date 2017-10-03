package edu.hgu.se.teaml;
import java.util.LinkedList;
import java.util.List;

public class HashObject
{
	private String url = ""; // check using isEmpty()
	private String title = ""; // optional
	
	public HashObject() {}
	
	public HashObject(String url)
	{
		this.url = url;
	}
	
	public HashObject(String url, String title)
	{
		this(url);
		this.title = title;
	}
	
	public String getUrl() { return url; }
	public String getTitle() { return title; }
	
	public void setUrl(String string) { url = string; }
	public void setTitle(String string) { title = string; }
}
