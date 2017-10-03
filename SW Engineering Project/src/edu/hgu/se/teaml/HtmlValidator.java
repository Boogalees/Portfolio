package edu.hgu.se.teaml;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.StringWriter;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import java.util.List;

import org.w3c.tidy.Tidy;

public class HtmlValidator {
	public HtmlValidator(){
		
	}
	
	public void checkSyntax(List<String> htmlFileList){
		for(int i = 0; i < htmlFileList.size(); i++){
			executeJTidy(htmlFileList.get(i));
		}
	}
	
	
	public void executeJTidy(String htmlFilePath){

		String content = readHtmlFile(htmlFilePath);		
		Writer stringWriter = new StringWriter();
		InputStream inputStream = getByteArrayInputStream(content);
		if(inputStream == null)
			return;
			
		Tidy tidy = new Tidy();
		tidy.setPrintBodyOnly(true);
		tidy.setQuiet(true);
		tidy.setShowWarnings(true);

		System.out.println("---------------Jtidy Message---------------");
		tidy.parse(inputStream, stringWriter);
	
	}
	
	private String readHtmlFile(String htmlFilePath) {
		StringBuilder contentBuilder = new StringBuilder();
		BufferedReader in;
		try{
			in = new BufferedReader(new FileReader(htmlFilePath));
			String str;
			while((str = in.readLine()) != null)
				contentBuilder.append(str);
			
			in.close();
		} catch (IOException e) {
			System.out.println("Failed to read file");
			return null;
		}
		
		return contentBuilder.toString();
	}
	
	private InputStream getByteArrayInputStream(String content) {
		try{
			if(content != null)
				return new ByteArrayInputStream(content.getBytes("UTF-8"));
			else
				throw new UnsupportedEncodingException();
		}catch (UnsupportedEncodingException e) {
			System.out.println("unsupported encoding exceptioin");
			return null;
		}	
	}

}
