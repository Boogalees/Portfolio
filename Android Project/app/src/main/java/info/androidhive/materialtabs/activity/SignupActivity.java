package info.androidhive.materialtabs.activity;

import android.location.LocationManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import info.androidhive.materialtabs.R;

/**
 * Created by KBPark on 2016. 12. 3..
 */
public class SignupActivity extends AppCompatActivity
{
    EditText id;
    EditText pw;
    EditText email;
    EditText address;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.signup);

        LocationManager locationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER))
        {
            Toast.makeText(this, "GPS on",
                    Toast.LENGTH_SHORT).show();
        }
        else
        {
            Toast.makeText(this, "GPS off",
                    Toast.LENGTH_SHORT).show();
        }
    }

    public void onSaveBtnClicked(View v)
    {
        String idStr = id.getText().toString();
        String pwStr = pw.getText().toString();
        String emailStr = email.getText().toString();
        String addressStr = address.getText().toString();

        // DB에 save!
    }
}
