package info.androidhive.materialtabs.fragments;

import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.util.TypedValue;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.baoyz.swipemenulistview.SwipeMenu;
import com.baoyz.swipemenulistview.SwipeMenuCreator;
import com.baoyz.swipemenulistview.SwipeMenuItem;

import info.androidhive.materialtabs.R;
import info.androidhive.materialtabs.db.BasicInfo;
import info.androidhive.materialtabs.db.DBAdapter;
import info.androidhive.materialtabs.db.ListDbHelper;
import info.androidhive.materialtabs.db.ListItem;




public class UserHistoryFragment extends android.support.v4.app.Fragment
{
    //--------- UserHistoryFragment ------//
    ListDbHelper helper;
    SQLiteDatabase db;

    ListView mListView;

    static DBAdapter mAdapter;

    public UserHistoryFragment() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        View view = inflater.inflate(R.layout.user_list, null);



        mListView = (ListView) view.findViewById(R.id.userListView);


// -------------------------------- db open하고 CursorAdapter 초기화 -------------------------------------
        helper = new ListDbHelper(container.getContext(), BasicInfo.USER_DB, null, 1);
        db = helper.getWritableDatabase(); //db open!

        /**
         *  for testing - dropTable
         */
        // helper.dropTable(db, BasicInfo.USER_TABLE);

        // 1. create table
        helper.createTable(db, BasicInfo.USER_TABLE);

        // 2. select
        String SELECT_SQL = "SELECT * FROM " + BasicInfo.USER_TABLE;

        /**
         *  CursorAdapter의 인자로 보낼 Cursor객체.
         */
        Cursor cursor = db.rawQuery(SELECT_SQL, null);

        mAdapter = new DBAdapter(container.getContext(), cursor, true, BasicInfo.USER_DB);
// ----------------------------------------------------------------------------------------------------


        // 4. listView에 adapter set
        mListView.setAdapter(mAdapter); // SwipeMenuListView 객체를 사용합니다!

        return view;

    }

    /////////////////////////////////////////////////////////////////////////////////////////
    public static void setListData(String title, String address, String trash)
    {
        ListItem aItem = new ListItem(title, address, trash);
        mAdapter.addItemToDb(aItem);

        mAdapter.notifyDataSetChanged();
    }

    /*
    private void deleteAll()
    {
        mAdapter.removeAllItem();
    }
    */


}

