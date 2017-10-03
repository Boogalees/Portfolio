package edu.hgu.se.teaml;
import java.util.LinkedList;
import java.util.List;

public class Document implements MDElement
{
	private List<Node> nodes = new LinkedList<Node>();
	private String fileName;

	@Override
	public void accept(MDElementVisitor mdElementVisitor)
	{
		mdElementVisitor.visitDocument(this);
	}
	
	public void add(Node node)
	{
		nodes.add(node);
	}
	
	public void setFileName(String fileName)
	{
		this.fileName = fileName;
	}
	
	public List<Node> getNodes()
	{
		return nodes;
	}
}
