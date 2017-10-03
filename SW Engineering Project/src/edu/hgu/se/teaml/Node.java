package edu.hgu.se.teaml;
import java.util.LinkedList;
import java.util.List;

public class Node implements MDElement
{
	private List<MDElement> elements = new LinkedList<MDElement>();
	private int level;
	protected String type;
	
	public Node() {}
	
	public Node(String type)
	{
		this.type = type;
	}
	
	@Override
	public void accept(MDElementVisitor mdElementVisitor) {
		mdElementVisitor.visitNode(this);
	}
	
	public static Node create(String type){
		
		if(type.equals("HEADER"))
			return new Header("HEADER");
		else if(type.equals("HR"))
			return new Hr("HR");
		else if(type.equals("BR"))
			return new Br("BR");
		else if(type.equals("SETEXT_HEADER"))
			return new SetextHeader("SETEXT_HEADER");
		else if(type.equals("ATX_HEADER"))
			return new AtxHeader("ATX_HEADER");
		else if(type.equals("BLOCK"))
			return new Block("BLOCK");
		else if(type.equals("QUOTED_BLOCK"))
			return new QuotedBlock("QUOTED_BLOCK");
		else if(type.equals("ITEM_LIST"))
			return new ItemList("ITEM_LIST");
		else if(type.equals("ORDERED_LIST"))
			return new OrderedList("ORDERED_LIST");
		else if(type.equals("UNORDERED_LIST"))
			return new UnorderedList("UNORDERED_LIST");
		else if(type.equals("CODE_BLOCK"))
			return new CodeBlock("CODE_BLOCK");
		else				//In the case of "else", it is considered error. so, in here, return error condition.
			return null;
	}
	
	public boolean add(MDElement element) {
		if(element == null){
			System.out.println("MDElement is null");
			return false;
		}
		else{
			elements.add(element);
			return true;
		}
	}
	
	public List<MDElement> getElements() {
		return elements;
	}
	
	public String getType() {
		return type;
	}
	
}
