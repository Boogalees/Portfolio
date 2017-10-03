package edu.hgu.se.teaml;
import java.util.HashMap;
import java.util.List;

public class HashHandler
{
	private HashMap<String, HashObject> hash = new HashMap<String, HashObject>();
	
	public HashObject get(String id) // null if none
	{
		return hash.get(id);
	}
	
	public boolean contains(String id)
	{
		return !(hash.get(id) == null);
	}
	
	public String getHref(String id)
	{
		if(contains(id))
			return get(id).getUrl();
		return null;
	}

	public String getSrc(String id)
	{
		if(contains(id))
			return get(id).getUrl();
		return null;
	}

	public String getTitle(String id)
	{
		if(contains(id))
			return get(id).getTitle();
		return null;
	}
	
	public void put(String id, HashObject hashObject)
	{
		hash.put(id, hashObject);
	}
}
