package info.androidhive.materialtabs.activity;

import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ListView;

import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.messaging.FirebaseMessaging;

import info.androidhive.materialtabs.R;
import info.androidhive.materialtabs.db.BasicInfo;
import info.androidhive.materialtabs.db.DBAdapter;
import info.androidhive.materialtabs.db.ListDbHelper;
import info.androidhive.materialtabs.db.ListItem;


public class CleanerActivity extends AppCompatActivity {

    //public static final String DB_NAME = "TrashMonitor";

    //public static final String CLEANER_TABLE_NAME = "CleanerTable";


    ListDbHelper helper;
    SQLiteDatabase db;
    Cursor cursor;

    ListView mListView;


    static DBAdapter mAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.cleaner_list);

        mListView = (ListView) findViewById(R.id.cleanerListView);

// -------------------------------- db open하고 CursorAdapter 초기화 -------------------------------------
        helper = new ListDbHelper(getApplicationContext(), BasicInfo.CLEANER_DB, null, 1);
        db = helper.getWritableDatabase(); //db open!

        /**
         *  CursorAdapter의 인자로 보낼 Cursor객체.
         */

        // for testing - drop the table
        // helper.dropTable(db, BasicInfo.CLEANER_TABLE);


        helper.createTable(db, BasicInfo.CLEANER_TABLE);
        cursor = db.rawQuery("SELECT * FROM " + BasicInfo.CLEANER_TABLE, null);

        mAdapter = new DBAdapter(getApplicationContext(), cursor, true, BasicInfo.CLEANER_DB);
// ----------------------------------------------------------------------------------------------------

        // 4. listView에 adapter set
        mListView.setAdapter(mAdapter); // SwipeMenuListView 객체를 사용합니다!

        //추가한 라인
        FirebaseMessaging.getInstance().subscribeToTopic("cleaner");
        FirebaseInstanceId.getInstance().getToken();

    }


    /////////////////////////////////////////////////////////////////////////////////////////
    public static void setListData(String title, String address, String trash)
    {
        ListItem aItem = new ListItem(title, address, trash);
        mAdapter.addItemToDb(aItem);
    }

    /*
    private void deleteAll()
    {
        mAdapter.removeAllItem();
    }
    */



    // onclick
    public void onCheckMapBtnClicked(View v)
    {
        Intent intent = new Intent(getApplicationContext(), CleanerMapActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(intent);
    }


}
