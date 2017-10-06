package info.androidhive.materialtabs.db;

/**
 * Created by KBPark on 2016. 8. 13..
 */public class ListItem
{
    private static final int NUM_OF_DATA = 3;

    Object[] mData = new Object[NUM_OF_DATA];

    public ListItem(String title, String address, String trash)
    {
        mData[0] = title;
        mData[1] = address;
        mData[2] = trash;
    }

    public Object getTitle()
    {
        return mData[0];
    }

    public Object getAddress()
    {
        return mData[1];
    }

    public Object getTrash()
    {
        return mData[2];
    }

    public Object[] getData()
    {
        return mData;
    }
}
