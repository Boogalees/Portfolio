package edu.hgu.se.teaml;
import java.util.List;

public class md2html
{
	public static void main(String[] args)
	{
		CommandHandler commandHandler = new CommandHandler();
		if(!commandHandler.setCommands(args))
			System.exit(0);
		else
			commandHandler.printOptions();
		
		int numOfFiles = commandHandler.getNumOfFiles();
		String[] inputFileNames =
				commandHandler.getInputFileNames().toArray(
						new String[numOfFiles]);
		List<String> outputFileNames = 
				commandHandler.getOutputFileNames();
		HashHandler[] hashHandlers = new HashHandler[inputFileNames.length];
		for(int i = 0; i < hashHandlers.length; i++)
		{
			hashHandlers[i] = new HashHandler();
		}
		
		Parser parser = new Parser(inputFileNames, hashHandlers);
		Document document = parser.ParserStart();
		
		document.accept(new PlainVisitor(outputFileNames.get(0), hashHandlers[0]));
		
		

		
		
		//LinkHandler example
//		LinkHandler linkHandler = new LinkHandler();
		
//		linkHandler.setHash("NAVER", "http://www.daum.net");
//		linkHandler.setHash("DAUM", "http://www.google.com","GOOGLE");
//		List<String> a = linkHandler.getList("DAUM");
		
//		System.out.println(a.get(1).toString());
//		System.out.println(a.size());
		///////
		
//		Node block = Node.create("BLOCK");
//		Token text =Token.create("PLAIN_TEXT");
//		text.setValue("hello world");
//		block.add(text);
//		
//		document.add(block);
//		
//		Node strong = Node.create("STRONG");
//		Token strongText = Token.create("PLAIN_TEXT");
//		strongText.setValue("stront text bold!");
//		strong.add(strongText);
//		
//		document.add(strong);
//		
//		Node code = Node.create("CODE");
//		Token codeText = Token.create("PLAIN_TEXT");
//		codeText.setValue("<strong> this is code example. </strong> &copy; &amp;");
//		code.add(codeText);
//		
//		document.add(code);
//		
//		Node codeBlock = Node.create("CODE_BLOCK");
//		Token codeBlockText = Token.create("PLAIN_TEXT");
//		codeBlockText.setValue("what is different? < > &amp <em>ahih</em>");
//		codeBlock.add(codeBlockText);
//		
//		document.add(codeBlock);
//		
//		Node linkInline = Node.create("LINK_INLINE");
//		Token link=Token.create("PLAIN_TEXT");
//		Token url=Token.create("PLAIN_TEXT");
//		Token optionTitle = Token.create("PLAIN_TEXT");
//		link.setValue("Click this!");
//		url.setValue("http://www.naver.com");
//		optionTitle.setValue("DAUM WEBSITE");
//		linkInline.add(strong);
//		linkInline.add(url);
//		linkInline.add(optionTitle);
//		
//		document.add(linkInline);
		
//		System.out.println(EscapeCharacterHandler.process("4 < 5"));
//		System.out.println(EscapeCharacterHandler.process("<div>"));
//		System.out.println(EscapeCharacterHandler.process("http://images.google.com/images?num=30&q=larry+bird"));
//		System.out.println(EscapeCharacterHandler.process("&copy;"));
//		System.out.println(EscapeCharacterHandler.process("AT&T"));
		
		
//		Node hr = Node.create("HR");
//		document.add(hr);
//		
//		Node header = Node.create("SETEXT_HEADER");
//		((Header)header).setSize(2);
//		Token headerToken1 = Token.create("PLAIN_TEXT");
//		headerToken1.setValue("this is h3");
//		header.add(headerToken1);
//		header.add(Node.create("BR"));
//		Token headerToken2 = Token.create("PLAIN_TEXT");
//		headerToken2.setValue("continued after br");
//		header.add(headerToken2);
//		document.add(header);
//		
//		Node quotedBlock1 = Node.create("QUOTED_BLOCK");
//		Token quotedBlockToken1 = Token.create("PLAIN_TEXT");
//		quotedBlockToken1.setValue("quoted block here");
//		quotedBlock1.add(quotedBlockToken1);
//		
//		Node quotedBlock2 = Node.create("QUOTED_BLOCK");
//		
//		Node emphasis = Node.create("EMPHASIS");
//		Token val = Token.create("PLAIN_TEXT");
//		val.setValue("quoted block emphasized nested");
//		emphasis.add(val);
//		quotedBlock2.add(emphasis);
//		
//		quotedBlock1.add(quotedBlock2);
//		document.add(quotedBlock1);
//		
//		
//		Node image = Node.create("IMAGE_INLINE");
//		Token alt = Token.create("PLAIN_TEXT");
//		alt.setValue("image");
//		Token imageUrl = Token.create("PLAIN_TEXT");
//		imageUrl.setValue("http://www.naver.com");
//		image.add(alt);
//		image.add(imageUrl);
//		document.add(image);
//		
//		document.accept(new PlainVisitor("test.html"));

		HtmlValidator htmlValidator = new HtmlValidator();
		htmlValidator.checkSyntax(outputFileNames);
		
	}
}
