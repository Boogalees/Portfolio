package info.androidhive.materialtabs.activity;

/**
 * Created by Julia on 2016-12-01.
 */

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.media.RingtoneManager;
import android.net.Uri;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.google.firebase.messaging.RemoteMessage;

import info.androidhive.materialtabs.R;
import info.androidhive.materialtabs.db.BasicInfo;
import info.androidhive.materialtabs.db.ListDbHelper;

public class MyFirebaseMessagingService extends com.google.firebase.messaging.FirebaseMessagingService {
    private static final String TAG = "FirebaseMsgService";

    public String temp="DEFAULT";


    // [START receive_message]
    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {

        Log.d("service", "되니?");

        temp = remoteMessage.getNotification().getBody();

        ////////////////////////////////////
        String[] array = new String[3];

        array = temp.split(",");


        /**
         *  Cleaner DB TABLE
         */



        SQLiteDatabase db;
        ListDbHelper helper;

        helper = new ListDbHelper(getApplicationContext(), BasicInfo.CLEANER_DB, null, 1);
        db = helper.getWritableDatabase(); //db open!

        helper.insertRec(db, BasicInfo.CLEANER_TABLE, array[0], array[1], array[2]);

        //////////////////////////////////

        sendNotification(temp);

        // remoteMessage.getNotification(0.getBody();  가 노티피케이션 내용을 가져오는것.


        //       Log.d(TAG, "From: " + remoteMessage.getFrom());

        // Check if message contains a data payload.
        if (remoteMessage.getData().size() > 0) {
 //           Log.d(TAG, "Message data payload: " + remoteMessage.getData());
        }

        // Check if message contains a notification payload.
        if (remoteMessage.getNotification() != null) {
            Log.d(TAG, "Message Notification Body: " + remoteMessage.getNotification().getBody());

            //String bodydata = remoteMessage.getNotification().getBody();

//            Log.d(TAG, bodydata);
        }

    }

    public String PollingMessage()
    {
        return temp;
    }


    public void sendNotification(String body) {

            Intent intent = new Intent(this, MainActivity.class);
            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
             PendingIntent pendingIntent = PendingIntent.getActivity(this, 0 /* Request code */, intent,
                    PendingIntent.FLAG_ONE_SHOT);

            Uri defaultSoundUri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);

            NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(this)
                    .setSmallIcon(R.mipmap.ic_launcher)
                    .setContentTitle("새 알림이 도착했습니다.")
                    .setContentText(body)
                    .setAutoCancel(true)
                    .setSound(defaultSoundUri)
                    .setContentIntent(pendingIntent);

            NotificationManager notificationManager =
                    (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);

            notificationManager.notify(0 /* ID of notification */, notificationBuilder.build());

    }


}
