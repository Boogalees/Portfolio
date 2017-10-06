package info.androidhive.materialtabs.db;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CursorAdapter;
import android.widget.TextView;

import info.androidhive.materialtabs.R;

/**
 * Created by KBPark on 2016. 8. 13..
 */public class DBAdapter extends CursorAdapter
{

    SQLiteDatabase db;
    ListDbHelper helper;

    TextView titleView;
    TextView addressView;
    TextView trashView;

    public DBAdapter(Context context, Cursor c, boolean autoRequery, String db_name)
    {
        super(context, c, autoRequery);

        helper = new ListDbHelper(context, db_name, null, 1);
        db = helper.getWritableDatabase(); // db open!
    }

//------------------------------------------ implement ----------------------------------------------------

    // 여기서는 data를 set하는 일을 하구요
    @Override
    public void bindView(View view, Context context, Cursor cursor)
    {
        titleView.setText(cursor.getString(cursor.getColumnIndex("title")));
        addressView.setText(cursor.getString(cursor.getColumnIndex("address")));
        trashView.setText(cursor.getString(cursor.getColumnIndex("trash")));
    }

    // 여기서는 layout관련 일을 주로 합니다.
    @Override
    public View newView(Context context, Cursor cursor, ViewGroup parent)
    {
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View itemlayout = inflater.inflate(R.layout.item_view, null, true);

        titleView = (TextView) itemlayout.findViewById(R.id.title);
        addressView = (TextView) itemlayout.findViewById(R.id.address);
        trashView = (TextView) itemlayout.findViewById(R.id.trash);

        return itemlayout;
    }


//------------------------------------------ 내가 정의한 메소드들 ------------------------------------------------

    // User db에 바로 추가!
    public void addItemToDb(ListItem item)
    {
        // db에 item추가!
        if(db != null)
        {
            helper.insertRec(db, BasicInfo.USER_TABLE, (String)item.getTitle(), (String)item.getAddress(), (String)item.getTrash()); // 해당 record에 data 추가!
            this.getCursor().requery(); // CursorAdapter의 listView 갱신코드!
        }else{
            Log.d("test", "db open 안됨");
        }
    }

    /*
    public void removeItem(int position, String table_name)
    {
        //
        //  받아온 position을 근거로, db의 '_id'값을 알아내서 그걸로 deletedRec()를 call해야 한다!
        //  -> ListView에서의 position과 Cursor의 position은 어짜피 동일하다(0부터 채워나간다)는 점을 이용.
        //

        Cursor cursor = db.rawQuery("SELECT _id FROM " + table_name, null); // 1) _id 필드를 다 찾아온 다음에
        cursor.moveToPosition(position); // 2) 해당 pisition으로 cursor를 이동시킨 후 - position은 listView에서 몇번째 position에 있는지 알려주는 변수다.
        int index = cursor.getInt(cursor.getColumnIndex("_id")); // 3) '_id'값을 빼온다.
        cursor.close();


        // db에서 item삭제!
        if(db != null)
        {
            helper.deleteRec(db, index); // 해당 record 삭제!
            this.getCursor().requery(); // CursorAdapter의 listView 갱신코드!

        } else {
            Log.d("test", "db open 안됨");
        }

    }
    */

    /*
    public void removeAllItem()
    {
        helper.deleteAllRec(db);
        this.getCursor().requery(); // CursorAdapter의 listView 갱신코드!
    }
    */

}
