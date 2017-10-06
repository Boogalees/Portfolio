package info.androidhive.materialtabs.activity;



import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

public class GPSModeReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        Intent i = new Intent(context,SignupActivity.class);
        i.setFlags(
                Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_NEW_TASK);
        if (intent.getAction().matches("android.location.PROVIDERS_CHANGED"))
        {
            Toast.makeText(context, "GPS provider changed. restart activity",
                    Toast.LENGTH_SHORT).show();
            Intent pushIntent = new Intent(context, SignupActivity.class);
            context.startService(pushIntent);
        }
        context.startActivity(i);
    }



}
