package edu.hgu.se.teaml;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parser {

	File file;
	FileReader fr;
	BufferedReader br;
	int Is_nested[] = { 0 };
	int Nested_List = 0;
	int Continue = 0;
	int Continue_nested = 0;
	String Contents = null;
	boolean EndOfNode = false;
	char temp;
	int blocked = 1;
	String[] FN = {};
	boolean IDchecked_img = false;
	boolean IDchecked_link = false;
	int Start, End;
	String LinkText = null;
	String URL = null;
	String Title = null;
	boolean REM = true;
	Document doc;
	HashHandler[] get_HH;
	StringBuffer sb = new StringBuffer();
	ArrayList<String> NL = new ArrayList<>();
	boolean flag=true;

	// New
	String next_line = null;
	String current_line = null;

	public Parser(String[] file_list, HashHandler[] HH) {

		doc = new Document();
		this.get_HH = HH;

		int i = 0;
		FN = new String[file_list.length];
		for (i = 0; i < file_list.length; i++) {
			FN[i] = file_list[i];
		}
	}

	public Document ParserStart() {
		try {
			String s;
			boolean get;

			for (int i = 0; i < FN.length; i++) {
				file = new File(FN[0]);
				fr = null;
				br = null;
				fr = new FileReader(file);
				br = new BufferedReader(fr);

				while ((s = br.readLine()) != null) {

					//ParseLine(s);
					ParseLine_edited(null,s,null,0);
				}
			}

			br.close();

		} catch (IOException e) {
			System.err.println(e);
			exit(1);
		}
		return doc;
	}
	
	public int ParseLine_edited(MDElement node, String str, String type, int cur_pos)
	{
		
		//dont need to modify
		if(type=="STRONG"||type=="EMPHASIS")
		{
			
			int i=0;
			String temp = "";
			//System.out.println(str);
			int size = str.length();
			if(str.endsWith("  ")){
				size = str.length() - 2;
			}
			for(i=0;i<size;i++)
			{
				if(str.charAt(i)=='*')
				{
										
					if(i==str.length()-1)
					{
						Emphasis em = new Emphasis();
						em.addToken(temp);
						node.add(em);
						return i;
					}
					
					else if(str.charAt(i+1)=='*')
					{
						Strong st = new Strong();
						st.addToken(temp);
						node.add(st);
						return i+2;
					}
					
					else
					{
						Emphasis em = new Emphasis();
						em.addToken(temp);
						node.add(em);
						return i;
					}
					
				}
				else
				{
					temp+=str.charAt(i);
					//System.out.println(temp);
				}
				
				if(i == size - 1)
					return -1;
			}
			
			return -1;

		}
	
		//dont need to modify
		else if(type=="CODE")
		{
			String temp = "";
			int size = str.length();
			if(str.endsWith("  ")){
				size = str.length() - 2;
			}
			for(int i=0;i<size;i++)
			{
				if(str.charAt(i)==' ')
				{
					if(i+1 >= size){
						return -1;
					}
					
					if(str.charAt(i+1) == '`'){
						if(i+2 >= size){
							return -1;
						}
						
						if (str.charAt(i + 2) == ' ') {
							Code code = new Code();
							code.setValue(temp);
							node.add(code);
							return i + 4;
						}
					}
										
				}
				else
				{
					temp+=str.charAt(i);
				}
				
				if(i == str.length()-1)
					return -1;
			}

					
		}
		
		//dont need to modify
		else if(type=="CODE_BLOCK")
		{
			String get;

			try{
						
				if((get=br.readLine()).startsWith("\t"))
				{
					//System.out.println(get);
					
					if(NextLine()==null){
						PlainText plaintext2 = new PlainText();
						plaintext2.setValue(get);
						node.add(plaintext2);
						//System.out.println(get);
						//ParseLine_edited(node,get,"CODE_BLOCK",0);
						return 0;	
					}
					
					if(NextLine().startsWith("\t"))
					{
						PlainText plaintext = new PlainText();
						plaintext.setValue(get);
						node.add(plaintext);
						ParseLine_edited(node,NextLine(),"CODE_BLOCK",0);
						return 0;
					}
					else
					{
						PlainText plaintext2 = new PlainText();
						plaintext2.setValue(get);
						node.add(plaintext2);
						return 0;
					}
				}
				
				
				
			}catch (IOException e) {
				System.err.println("aadsfasdf");
				exit(1);
			}
			
			
		}
		else if(type=="BLOCK")
		{
			parseToken(node, str);
			
			String get;
			try{
				get=br.readLine();
				
				if(get.endsWith("  "))
	            {
					//System.out.println("===========");
					Br br = new Br();
					node.add(br);
	            }
				
				if(Is_Block(get))
				{
					
					if(NextLine()==null){
						return 0;	
					}
					
					if(Is_Block(NextLine()))
					{
						ParseLine_edited(node,NextLine(),"BLOCK",0);
						return 0;
					}
					else
					{
						return 0;
					}
					
				}
								
			}catch (IOException e) {
				System.err.println("aadsfasdf");
				exit(1);
			}

		}
		else if(type=="IMAGE")
		{
			
		}
		else if(type=="LINK")
		{
			
		}
		else if(type=="QUTOED_BLOCK")
		{
			//int level = 1;
			int pos = Is_BlockQutoes(str);
			
			if(pos == -1){
				pos = 0;
			}
			
			if (str.charAt(pos) == '>') {
				
				parseToken(node, str.substring(pos+1));
				
				
				try {

					if (str.endsWith("  ")) {
						// System.out.println("===========");
						Br br = new Br();
						node.add(br);
					}

					if (NextLine() != null) {
						// System.out.println(NextLine());
						if (NextLine().length() == 0)
							return 0;
						
				//		if(NextLine())

						ParseLine_edited(node, br.readLine(), "QUTOED_BLOCK", 0);
						return 0;
					} else {
						return 0;
					}

				} catch (IOException e) {
					System.err.println("aadsfasdf");
					exit(1);
				}
				
			} else {
				parseToken(node, str);
				// System.out.print("str:"+ str);

				try {

					if (str.endsWith("  ")) {
						// System.out.println("===========");
						Br br = new Br();
						node.add(br);
					}

					if (NextLine() != null) {
						// System.out.println(NextLine());
						if (NextLine().length() == 0)
							return 0;

						ParseLine_edited(node, br.readLine(), "QUTOED_BLOCK", 0);
						return 0;
					} else {
						return 0;
					}

				} catch (IOException e) {
					System.err.println("aadsfasdf");
					exit(1);
				}

			}
			
		}
		else if(type=="LIST")
		{
			Node li = new Node();
			node.add(li);
			parseToken(li, str);
			
		}
		else if(type=="HR")
		{
			
		}
		
		
		/************************************************************************************/
		//dont need to modify
		if (Is_ATX_HEAD(str)) {
			
			int numOfH=0;
			
			AtxHeader header = new AtxHeader();
			
			numOfH = parseToken(header, str);
			header.setSize(numOfH);
			doc.add(header);
			

		}
		
		//dont need to modify
		else if(Is_SETEXT_HEAD(str)==1||Is_SETEXT_HEAD(str)==2) {
			// End. No more one line
			
			SetextHeader header = new SetextHeader();
			
			parseToken(header, str);
			header.setSize(Is_SETEXT_HEAD(str));
			doc.add(header);
			try{
	            br.readLine();
	            
	         }catch (IOException e) {
	            System.err.println(e);
	            exit(1);
	         }
	         
		}
		//dont need to modify
		else if (Is_CodeBlock(str)) {

			CodeBlock codeblock = new CodeBlock();
			if(NextLine().startsWith("\t"))
			{
				PlainText plaintext2 = new PlainText();
				plaintext2.setValue(str);
				codeblock.add(plaintext2);
				
				ParseLine_edited(codeblock,NextLine(),"CODE_BLOCK",0);
			}
			else
			{
				
				PlainText plaintext = new PlainText();
				plaintext.setValue(str);
				codeblock.add(plaintext);
			}
			
			doc.add(codeblock);
			
		}
		
		//need to modify
		else if(Is_Block(str))
		{
			Block block = new Block();
			
			//System.out.println(str);
			
			parseToken(block, str);
			
			if(str.endsWith("  "))
            {
				Br br = new Br();
				block.add(br);
            }
			
			
			if(NextLine()!=null)
			{
				if(Is_Block(NextLine()))
				{
					ParseLine_edited(block,NextLine(),"BLOCK",0);
				}
				
			}
		
			doc.add(block);
	
		}
		
		else if(Is_list(str)=='o'){
			OrderedList orderedList = new OrderedList();
			Node li = new Node();
			orderedList.add(li);
			parseToken(li, str);

			int line = 0;
			while(NextLine()!=null){

				if (Is_list(NextLine()) == 'o') {
					OrderedList child = new OrderedList();
					ParseLine_edited(child, NextLine(), "LIST", line);
				} else if (Is_list(NextLine()) == 'u') {

				} else {
					break;
				}

			}
			
			
		
		}
	
		else if(Is_BlockQutoes(str) != -1){
			//System.out.println("sdfasfsdfa");
			
			int pos = Is_BlockQutoes(str);
						
			QuotedBlock quotedBlock = new QuotedBlock();
			
			if(pos+1 < str.length()){
				boolean flag = false;
				
				parseToken(quotedBlock, str.substring(pos+1));
				
				String get;
				try{
					
					if(str.endsWith("  "))
		            {
						Br br = new Br();
						node.add(br);
		            }
					
					if(NextLine() != null){
						
						if(NextLine().length() == 0){
							flag = true;
						}
						
						if(!flag)
						ParseLine_edited(quotedBlock, br.readLine(), "QUTOED_BLOCK", 0);
						
					}
					else{		
						
					}
					
					
					
				}catch (IOException e) {
					System.err.println("aadsfasdf");
					exit(1);
				}

			}
			else{
				PlainText plainText = new PlainText();
				plainText.setValue("");
				quotedBlock.add(plainText);
			}
		
		
			doc.add(quotedBlock);
			
		}
		
		else if(Is_Link(str)!='x'){
	         //Link link = new Link();
	         Block block = new Block();
	         //block.add(link);
	         
	         parseToken(block, str);
	         
	         doc.add(block);
	   }
		
		

		
		return 0;
		
		
	}
	
	/******************************************************************************************************/
	
	public int parseToken(MDElement node, String str) {
	      int pos = 0;
	      String temp = "";
	      boolean flag = false;
	      int numOfH = 0;
	      // System.out.println("dltkdgo : " + str);
	      int size = str.length();

	      // if there is double white spaces
	      if (str.endsWith("  ")) {
	         size = str.length() - 2;
	      }

	      for (int i = 0; i < size; i++) {

	         // System.out.println(i);

	         if (str.charAt(i) == '#') {
	            numOfH++;
	         } else if (str.charAt(i) == '*') {
	            flag = true;
	            PlainText plaintext2 = new PlainText();
	            plaintext2.setValue(temp);
	            node.add(plaintext2);
	            

	            if (i + 1 >= size) {
	               flag = false;
	               temp += str.substring(i);
	               break;
	               // string bound exception
	            }

	            if (str.charAt(i + 1) == '*') {
	               if ((pos = ParseLine_edited(node, str.substring(i + 2), "STRONG", i + 1)) == -1) {
	                  temp = "";
	                  temp += str.charAt(i);
	                  continue;
	               }
	               i += pos + 1;
	               temp = "";
	            } else {
	               if ((pos = ParseLine_edited(node, str.substring(i + 1), "EMPHASIS", i + 1)) == -1) {
	                  temp = "";
	                  temp += str.charAt(i);
	                  continue;
	               }
	               i += pos + 1;
	               temp = "";
	            }
	         }
	         else if(str.charAt(i)=='!'){
	        	 
	        	 if(str.charAt(i)=='[')
		         {
		        	 
		            int k, Start, End=0;
		            String URL = null, Title = null;
		            boolean flags = false;
		            
		            PlainText plainText = new PlainText();
		            plainText.setValue(temp);
		            node.add(plainText);
		            
		            Image img = new Image();
		            
		            for(k=i+1;k<str.length();k++)
		            {
		               if(str.charAt(k)=='*')
		               {
		                  flags = true;
		                  img.addToken(temp);
		                  
		                  if (k + 1 >= size) {
		                     flags = false;
		                     temp += str.substring(k);
		                     break;
		                     // string bound exception
		                  }
		                  
		                  if(str.charAt(k+1)=='*')
		                  {
		                     if ((pos = ParseLine_edited(img, str.substring(k + 2), "STRONG", k + 1)) == -1) {
		                        temp = "";
		                        temp += str.charAt(k);
		                        continue;
		                     }
		                     k += pos + 1;
		                     temp = "";
		                  }
		                  else
		                  {
		                     if ((pos = ParseLine_edited(img, str.substring(k + 1), "EMPHASIS", k + 1)) == -1) {
		                        temp = "";
		                        temp += str.charAt(k);
		                        continue;
		                     }
		                     k += pos + 1;
		                     temp = "";
		                  }
		               }
		               else if(str.charAt(k)==']')
		               {
		                  //System.out.println("id" + temp);
		            	  img.addToken(temp);
		                  temp = "";
		                  
		               }
		               else if(str.charAt(k) == '(')
		               {
		                  
		                  String get_sub = str.substring(k,str.length());
		   
		                  Start = get_sub.indexOf('(');
		                  End = get_sub.indexOf('"');
		                  if (End == -1) {
		                     End = get_sub.indexOf(')');
		                     URL = get_sub.substring(Start + 1, End);
		                     Title = "";
		                  } else {
		                     URL = get_sub.substring(Start + 1, End);
		                     Start = get_sub.indexOf('"');
		                     End = get_sub.indexOf(')');
		                     Title = get_sub.substring(Start + 1, End - 1);
		                  }
		                  img.setHref(URL);
		                  img.setTitle(Title);
		                  
		                  System.out.println(Title + "  "+ URL);
		                  
		               }
		               else{
		            	   temp += str.charAt(k);
		               }
		                  
		               

		               if (k >= size - 1) {
		                  // System.out.println("sdfsaf: " + i);
		                  flags = false;
		                  break;
		               }
		                  
		            }
		            node.add(img);
		            //System.out.println("kkkkkk" + k);
		            temp = "";
		            i = k+1;
		            
	         }
	         else if(str.charAt(i)=='[')
	         {
	        	 
	            int k, Start, End=0;
	            String URL = null, Title = null;
	            boolean flags = false;
	            
	            PlainText plainText = new PlainText();
	            plainText.setValue(temp);
	            node.add(plainText);
	            
	            Link link = new Link();
	            
	            for(k=i+1;k<str.length();k++)
	            {
	               if(str.charAt(k)=='*')
	               {
	                  flags = true;
	                  link.addToken(temp);
	                  
	                  if (k + 1 >= size) {
	                     flags = false;
	                     temp += str.substring(k);
	                     break;
	                     // string bound exception
	                  }
	                  
	                  if(str.charAt(k+1)=='*')
	                  {
	                     if ((pos = ParseLine_edited(link, str.substring(k + 2), "STRONG", k + 1)) == -1) {
	                        temp = "";
	                        temp += str.charAt(k);
	                        continue;
	                     }
	                     k += pos + 1;
	                     temp = "";
	                  }
	                  else
	                  {
	                     if ((pos = ParseLine_edited(link, str.substring(k + 1), "EMPHASIS", k + 1)) == -1) {
	                        temp = "";
	                        temp += str.charAt(k);
	                        continue;
	                     }
	                     k += pos + 1;
	                     temp = "";
	                  }
	               }
	               else if(str.charAt(k)==']')
	               {
	                  //System.out.println("id" + temp);
	                  link.addToken(temp);
	                  temp = "";
	                  
	               }
	               else if(str.charAt(k) == '(')
	               {
	                  
	                  String get_sub = str.substring(k,str.length());
	   
	                  Start = get_sub.indexOf('(');
	                  End = get_sub.indexOf('"');
	                  if (End == -1) {
	                     End = get_sub.indexOf(')');
	                     URL = get_sub.substring(Start + 1, End);
	                     Title = "";
	                  } else {
	                     URL = get_sub.substring(Start + 1, End);
	                     Start = get_sub.indexOf('"');
	                     End = get_sub.indexOf(')');
	                     Title = get_sub.substring(Start + 1, End - 1);
	                  }
	                  link.setHref(URL);
	                  link.setTitle(Title);
	                  
	                  System.out.println(Title + "  "+ URL);
	                  
	               }
	               else{
	            	   temp += str.charAt(k);
	               }
	                  
	               

	               if (k >= size - 1) {
	                  // System.out.println("sdfsaf: " + i);
	                  flags = false;
	                  break;
	               }
	                  
	            }
	            node.add(link);
	            //System.out.println("kkkkkk" + k);
	            temp = "";
	            i = k+1;
	            
	         }      
	         else if (str.charAt(i) == ' ') {

	            if (i + 1 >= size) {
	               flag = false;
	               temp += str.substring(i);
	               break;
	               // string bound exception
	            }

	            if (str.charAt(i + 1) == '`') {

	               if (i + 2 >= size) {
	                  temp += str.substring(i);
	                  flag = false;
	                  break;
	               }

	               if (str.charAt(i + 2) == ' ') {
	                  flag = true;
	                  PlainText plainText = new PlainText();
	                  plainText.setValue(temp);
	                  node.add(plainText);

	                  if ((pos = ParseLine_edited(node, str.substring(i + 3), "CODE", i + 3)) == -1) {
	                     temp = "";
	                     temp += str.charAt(i);
	                     continue;
	                  }
	                  i += pos + 1;
	                  temp = "";
	               } else {
	                  temp += str.charAt(i);
	               }
	            } else {
	               temp += str.charAt(i);
	            }
	         } else {
	            temp += str.charAt(i);
	         }

	         if (i >= size - 1) {
	            // System.out.println("sdfsaf: " + i);
	            flag = false;
	            break;
	         }

	      }
	      if (!flag) {
	         //System.out.println("out temp : " + temp);
	         PlainText plaintext = new PlainText();
	         plaintext.setValue(temp + " ");
	         node.add(plaintext);
	      }

	      return numOfH;
	   }
	
	
	//dont need to modify
	public int parseToken11(Node node, String str){
		int pos = 0;
		String temp = "";
		boolean flag = false;
		int numOfH = 0;
		System.out.println("dltkdgo : " + str);
		int size = str.length();

		//if there is double white spaces
		if(str.endsWith("  "))
        {
			size = str.length() - 2;
        }

		
		for(int i=0;i<size;i++)
		{	
			
			System.out.println(i);
			
			
			if(str.charAt(i)=='#')
			{
				numOfH++;
			}			
			else if(str.charAt(i)=='*')
			{				
				flag = true;
				PlainText plaintext2 = new PlainText();
				plaintext2.setValue(temp);
				node.add(plaintext2);
				
				if(i + 1 >= size){
					flag = false;
					temp += str.substring(i);
					break;
					//string bound exception
				}
				
				if(str.charAt(i+1)=='*')
				{
					if((pos = ParseLine_edited(node, str.substring(i+2),"STRONG",i+1)) == -1){
						temp="";
						temp += str.charAt(i);
						continue;
					}
					i += pos+1;
					temp="";
				}
				else
				{
					if((pos = ParseLine_edited(node, str.substring(i+1),"EMPHASIS",i+1)) == -1){
						temp="";
						temp += str.charAt(i);
						continue;
					}
					i += pos+1;
					temp="";
				}
			}
			else if(str.charAt(i) == ' '){
				
				if(i + 1 >= size){
					flag = false;
					temp += str.substring(i);
					break;
					//string bound exception
				}
				
				if(str.charAt(i+1) == '`'){
					
					if(i+2 >= size){
						temp += str.substring(i);
						flag = false;
						break;
					}
					
					if(str.charAt(i+2) == ' '){
						flag = true;
						PlainText plainText = new PlainText();
						plainText.setValue(temp);
						node.add(plainText);

						if ((pos = ParseLine_edited(node, str.substring(i + 3), "CODE", i + 3)) == -1) {
							temp = "";
							temp += str.charAt(i);
							continue;
						}
						i += pos + 1;
						temp = "";
					}
					else{
						temp += str.charAt(i);
					}
				}
				else{
					temp += str.charAt(i);
				}
			}
			else
			{
				temp += str.charAt(i);
			}
			
			if(i >= size-1){
				//System.out.println("sdfsaf: " + i);
				flag=false;
				break;
			}

			

		
		}
		//if(!flag)
		//{
			//System.out.println("out temp : " + temp);
			PlainText plaintext = new PlainText();
			plaintext.setValue(temp + " ");
			node.add(plaintext);
		//}
		
		return numOfH;
	}
	
	public int exit(int code)
	{
		System.exit(1);
		return 1;
	}
	
	/*****************************************************************************************************/
	public boolean Is_Block(String str) {
		if (!Is_CodeBlock(str) && (Is_SETEXT_HEAD(str) == 0) && !Is_ATX_HEAD(str) && !str.equals("")
				&& Is_BlockQutoes(str) == -1 && Is_list(str) == 'x' /*&& Is_Image(str) == 'x'*/ && Is_Link(str) == 'x')
			return true;
		else
			return false;
	}


	/************************************************************************************/

	public String NextChar() {
		String next_char = null;

		try {
			br.mark(1000);
			next_char = String.valueOf(br.read());
			br.reset();
			return next_char;
		} catch (Exception e) {
			return null;
		}
	}

	public String NextLine() {
		String next_line;
		try {
			br.mark(10000);
			next_line = br.readLine();
			br.reset();
			return next_line;
		} catch (Exception e) {
			return null;
		}
	}

	/************************************************************************************/
	public void Build_BlockQutoes(String str) {
		
		int i,j=0;
		char[] get_str = str.toCharArray();
		String temp="";
		Block block = new Block();
		Block block2 = new Block();
		QuotedBlock qt = new QuotedBlock();
		PlainText plaintext = new PlainText();
		PlainText plaintext2 = new PlainText();
		//bufferstream m?
		
		for(i=0;i<get_str.length;i++)
		{
			if(get_str[i]=='>')
			{
				for(j=i+1;j<get_str.length-i;j++)
				{
					if(get_str[j]=='>')
					{
						i=j;
						break;
					}
						
					temp += get_str[j];
				}
				
				plaintext.setValue(temp);
				block.add(plaintext);
			}
			
			
		}
		
	
		qt.add(block);
		
		doc.add(qt);
		
	}

	/************************************************************************************/
	//dont need to modify
	public boolean Is_ATX_HEAD(String str) {
		if (str.startsWith("#"))
			return true;
		else
			return false;
	}

	
	//dont need to modify
	public int Is_SETEXT_HEAD(String str) {
		boolean EOF = false;
		String next_line;
		
		next_line = NextLine(); // why return null?
		if (next_line == null) {
			next_line = "EOF"; // To distinguish continuous file
		}
		if(next_line.startsWith("--"))
		{
			return 1;
		}
		else if(next_line.startsWith("=="))
		{
			return 2;
		}
		return 0;

	}

	//dont need to modify
	public boolean Is_CodeBlock(String str) {
		if (str.startsWith("\t")) {
			return true;
		} else {
			return false;
		}
	}

	//need to modify
	public int Is_BlockQutoes(String str) {
		if (str.startsWith(">"))
			return 0;
		else if(str.startsWith(" >"))
			return 1;
		else if(str.startsWith("  >"))
			return 2;
		else if(str.startsWith("   >"))
			return 3;
		else
			return -1;
	}

	//need to modify
	public char Is_list(String str) {
	      final String regex = "[0-9]+.*";

	      Pattern p = Pattern.compile(regex);
	      Matcher m = p.matcher(str);

	      if (str.startsWith("+") || str.startsWith("*") || str.startsWith("-")) {
	         return 'u';
	      } else if (m.find())
	         return 'o';
	      else
	         return 'x';
	   }

	//need to modify
	public boolean Is_HR(String str) {
		if (str.startsWith("***") || str.startsWith("* * *") || str.startsWith("---") || str.startsWith("- - -"))
			return true;
		else
			return false;
	}
	

	/************************************************************************************/

	//need to moidfy
	public char IsImages(String str) {
		final String inline = "!\\[[^\\]]+\\]\\([^)]+\\)";
		final String reference = "!\\[[^\\]]+\\]";
		Pattern p1 = Pattern.compile(inline);
		Pattern p2 = Pattern.compile(reference);
		Matcher m = p1.matcher(str);
		if (m.find()) {
			return 'i';
		}
		Matcher m2 = p2.matcher(str);
		if (m2.find()) {
			return 'r';
		}
		return 'x'; // for failure case
	}

	//need to modify
	public char Is_Link(String str) {
		final String inline = "\\[[^\\]]+\\]\\([^)]+\\)";
		final String reference = "\\[[^\\]]+\\]\\[[^\\]]+\\]";
		Pattern p1 = Pattern.compile(inline);
		Pattern p2 = Pattern.compile(reference);
		Matcher m = p1.matcher(str);
		if (m.find()) {
			return 'i';
		}
		Matcher m2 = p2.matcher(str);
		if (m2.find()) {
			return 'r';
		}
		return 'x'; // for failure case
	}

}
