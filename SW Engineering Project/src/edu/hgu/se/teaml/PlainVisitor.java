package edu.hgu.se.teaml;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;

public class PlainVisitor implements MDElementVisitor
{
	String outputFileName;
	private BufferedWriter file;
	private HashHandler hashHandler;
	
//	public void setLinkHandler(LinkHandler linkHandler)
//	{
//		this.linkHandler=linkHandler;
//	}
	
	// TODO: constructor
	
	public void setHashHandler(HashHandler hashHandler)
	{
		this.hashHandler = hashHandler;
	}
	
	public PlainVisitor(){}
	
	public PlainVisitor(String outputFileName, HashHandler hashHandler)
	{
		try
		{
			this.outputFileName = outputFileName;
			this.hashHandler = hashHandler;
			this.file = new BufferedWriter(new FileWriter(outputFileName));
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitDocument(Document document)
	{
		try
		{
			file.write("<!DOCTYPE html>");
			file.write("<html>");
			file.write("<head><title>" + outputFileName + "</title></head>");
			file.write("<body>");
			
			for(Node node: document.getNodes())
			{
				node.accept(this);
			}
			
			file.write("</body>");
			file.write("</html>");
			file.close();
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitElements(List<MDElement> mdElements)
	{
		for(MDElement mdElement: mdElements)
		{
			mdElement.accept(this);
		}
	}
	
	public void visitTokens(List<MDElement> tokens)
	{
		for(MDElement token: tokens)
		{
			token.accept(this);
		}
	}
	
	public void visitBlock(Block block)
	{
		try
		{
			file.write("<p>");
			
			visitElements(block.getElements());

			file.write("</p>");
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitStrong(Strong strong)
	{
		try
		{
			file.write("<strong>");
			
			visitTokens(strong.getTokens());
			
			file.write("</strong>");
		}
		catch(Exception ex)
		{
			
		}
		
	}
	
	public void visitCode(Code code)
	{
		try
		{
			file.write("<code>");
			
			file.write(EscapeHandler.forceEscape(code.getValue()));
			
			file.write("</code>");
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitCodeBlock(CodeBlock codeBlock)
	{
		try
		{
			file.write("<pre><code>");
			
			visitElements(codeBlock.getElements());
			
			file.write("</code></pre>");
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitLink(Link link)
	{
		try
		{
			if(link.isReference(hashHandler))
			{
				if(!link.isMatched(hashHandler))
				{
					file.write("[");
					
					visitTokens(link.getTokens());
					
					file.write("](" + link.getHref());
					if(!link.getTitle().isEmpty())
						file.write(" \"" + link.getTitle() + "\")");
					return;
				}
				else
				{
					link.setParameters(hashHandler);
				}
			}
			file.write("<a href=\"" + link.getHref() + "\"");
			if(!link.getTitle().isEmpty())
				file.write(" title=\"" + link.getTitle() + "\"");
			file.write(">");
			
			visitTokens(link.getTokens());
			
			file.write("</a>");
		}
		catch(Exception ex)
		{
			
		}
	}
		
	public void visitPlainText(PlainText plainText)
	{
		try
		{
			file.write(EscapeHandler.naturallyEscape(
					plainText.getValue()));
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitHr(Hr hr)
	{
		try
		{
			hr.getElements();
			file.write("<hr/>");
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitHeader(Header header)
	{
		try
		{
			file.write("<h" + header.getSize() + ">");
			
			visitElements(header.getElements());
			
			file.write("</h" + header.getSize() + ">");
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitSetextHeader(SetextHeader setextHeader)
	{
		visitHeader(setextHeader);
	}
	
	public void visitAtxHeader(AtxHeader atxHeader)
	{
		visitHeader(atxHeader);
	}
	
	public void visitQuotedBlock(QuotedBlock quotedBlock)
	{
		try
		{
			file.write("<blockquote>");
			
			visitElements(quotedBlock.getElements());
			
			file.write("</blockquote>");
		}
		catch(Exception ex)
		{
			
		}
	}
	
	public void visitEmphasis(Emphasis emphasis)
	{
		try
		{
			file.write("<em>");
			
			visitTokens(emphasis.getTokens());
			
			file.write("</em>");
		}
		catch(Exception ex)
		{

		}
	}

	public void visitImage(Image image)
	{
		try
		{
			if(image.isReference(hashHandler))
			{
				if(!image.isMatched(hashHandler))
				{
					file.write("![" + image.getAlt() + "](" + image.getSrc());
					if(!image.getTitle().isEmpty())
						file.write(" \"" + image.getTitle() + "\")");
					return;
				}
				else
				{
					image.setParameters(hashHandler);
				}
			}
			file.write("<img src=\"" + image.getSrc() + "\"");
			if(!image.getTitle().isEmpty())
				file.write(" title=\"" + image.getTitle() + "\"");
			file.write(" alt=\"" + image.getAlt() + "\"/>");
		}
		catch(Exception ex)
		{
			
		}
	}


	public void visitBr(Br br)
	{
		try
		{
			//�̷��ص� �ǳ�...
			br.getElements();
			file.write("<br/>");
		}
		catch(Exception ex)
		{
			
		}
	}

	public void visitElements(Node node)
	{
		for(MDElement mdElement: node.getElements())
		{
			mdElement.accept(this);
		}
	}

	public void visitStyleText(StyleText styleText)
	{
		
	}

	@Override
	public void visitNode(Node node) {
		try
		{
			visitElements(node.getElements());
		}
		catch(Exception ex)
		{
			
		}
	}

	@Override
	public void visitToken(Token token) {
		
	}

	@Override
	public void visitHtmlCode(HTMLCode htmlCode) {
		try
		{
			file.write(htmlCode.getValue());
		}
		catch(Exception ex)
		{
			
		}
	}

	public HashHandler getHashHandler() {
		return this.hashHandler;
	}

	public void visitUnorderedList(UnorderedList unorderedList) {
		try
		{
			file.write("<ul>");
			
			for(MDElement mdElement: unorderedList.getElements())
			{
				file.write("<li>");
				mdElement.accept(this);
				file.write("</li>");
			}
			
			file.write("</ul>");
		}
		catch(Exception ex)
		{
			
		}
	}

	public void visitOrderedList(OrderedList orderedList) {
		try
		{
			file.write("<ol>");
			
			for(MDElement mdElement: orderedList.getElements())
			{
				if(mdElement instanceof ItemList)
				{
					mdElement.accept(this);
				}
				else
				{
					file.write("<li>");
					mdElement.accept(this);
					file.write("</li>");
				}
			}
			
			file.write("</ol>");
		}
		catch(Exception ex)
		{
			
		}
	}
}