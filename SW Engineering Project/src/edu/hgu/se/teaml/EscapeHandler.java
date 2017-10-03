package edu.hgu.se.teaml;
import org.apache.commons.lang3.StringEscapeUtils;

public class EscapeHandler
{
	public static boolean alreadyEscaped(String string)
	{
		return !string.equals(StringEscapeUtils.unescapeHtml4(string));
	}
	
	public static String forceEscape(String string)
	{
		return StringEscapeUtils.escapeHtml4(string);
	}
	
	
	public static String naturallyEscape(String string)
	{
		String result = string;

		if(!alreadyEscaped(string))
		{
			return StringEscapeUtils.escapeHtml4(string);
		}
		
		return result;
	}
}