package edu.hgu.se.teaml;
import java.io.IOException;

public interface MDElementVisitor
{
	public void visitDocument(Document document);
	public void visitNode(Node node);
	public void visitToken(Token token);
	public void visitBlock(Block block);
	public void visitStrong(Strong strong);
	public void visitPlainText(PlainText plainText);
	public void visitCode(Code code);
	public void visitCodeBlock(CodeBlock codeBlock);
//	public void visitLinkInline(LinkInline linkInline);
//	public void visitLinkReference(LinkReference linkReference);

	
	public void visitHr(Hr hr);
	public void visitHeader(Header header);
	public void visitAtxHeader(AtxHeader atxHeader);
	public void visitQuotedBlock(QuotedBlock quotedBlock);
	public void visitEmphasis(Emphasis emphasis);
	public void visitImage(Image image);
//	public void visitImageReference(ImageReference imageReference);
//	public void visitImageInline(ImageInline imageInline);
	public void visitBr(Br br);
	public void visitSetextHeader(SetextHeader setextHeader);
	public void visitLink(Link link);
	public void visitStyleText(StyleText styleText);
	public void visitHtmlCode(HTMLCode htmlCode);
	public void visitOrderedList(OrderedList orderedList);
	public void visitUnorderedList(UnorderedList unorderedList);
}
