package info.androidhive.materialtabs.db;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

/**
 * Created by KBPark on 2016. 8. 13..
 */
public class ListDbHelper extends SQLiteOpenHelper
{
    /*
    private static final String CREATE_TABLE = "CREATE TABLE if not exists " + BasicInfo.TABLE_NAME + "("
            + "_id integer PRIMARY KEY autoincrement, "
            + "title text, "
            + "address text"
            + "trash text"
            + ")";

    private static final String DELETE_REC = "DELETE FROM " + BasicInfo.TABLE_NAME + " WHERE _id="; // 해당 record 하나 삭제
    private static final String DELETE_ALL_REC = "DELETE FROM " + BasicInfo.TABLE_NAME; // table의 entry를 다 삭제
    private static final String DROP_TABLE = "DROP TABLE if exists " + BasicInfo.TABLE_NAME; //table 자체를 삭제
    */

    public ListDbHelper(Context context, String db_name, SQLiteDatabase.CursorFactory factory, int version) {
        super(context, db_name, factory, version);
    }

//------------------------------------------ implement ----------------------------------------------------

    @Override
    public void onOpen(SQLiteDatabase db) {
        Log.i("test", "db open!");
        //createTable(db);
    }

    @Override
    public void onCreate(SQLiteDatabase db)
    {
        //createTable(db);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)
    {

    }

//------------------------------------------ 내가 정의한 메소드들 ----------------------------------------------

    public void insertRec(SQLiteDatabase db, String table_name, String title, String address, String trash)
    {
        // USER_TABLE에 item 추가!
        db.execSQL("INSERT INTO " + table_name
                + "(title, address, trash) VALUES"
                + "('"+ title + "' ," + "'" + address + "', '" + trash + "')"
        );

        Log.i("DB", "insert 완료");
    }

    public void createTable(SQLiteDatabase db, String table_name)
    {
        db.execSQL("CREATE TABLE if not exists " + table_name + "("
                + "_id integer PRIMARY KEY autoincrement, "
                + "title text, "
                + "address text, "
                + "trash text"
                + ")");
        Log.i("DB", "table 생성 완료");
    }

    public void dropTable(SQLiteDatabase db, String table_name)
    {
        db.execSQL("DROP TABLE " + table_name);
        Log.i("DB", "drop 완료");
    }

    /*
    public void deleteRec(SQLiteDatabase db, int index, String table_name)
    {
        db.execSQL("DELETE FROM " + table_name + " WHERE _id=" + index);
        // DELETE FROM [TABLE] WHERE _id=[index]

        Log.i("test", "Row 삭제 완료");
    }


    public void deleteAllRec(SQLiteDatabase db)
    {
        db.execSQL(DELETE_ALL_REC);
    }
    */

}

