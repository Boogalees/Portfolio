package info.androidhive.materialtabs.fragments;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.Calendar;

import info.androidhive.materialtabs.R;


public class UserTrashFragment extends Fragment implements View.OnClickListener
{
    public static final String TAG = "CalAdder";
    Calendar mCal = Calendar.getInstance();

    String title = "a";

    TextView textTotal;
    TextView text100L;
    TextView text50L;
    TextView text10L;
    TextView calendarlist;

    Button plus100L;
    Button plus50L;
    Button plus10L;

    Button minus100L;
    Button minus50L;
    Button minus10L;

    Button addUsingContentProviderButton;

    private int count100L;
    private int count50L;
    private int count10L;

    public UserTrashFragment() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        //View view = inflater.inflate(R.layout.user_trash, container, false);
        View view = inflater.inflate(R.layout.user_trash, null);

        addUsingContentProviderButton = (Button) view.findViewById(R.id.addUsingContentProviderButton);
        calendarlist = (TextView) view.findViewById(R.id.calendarList);

        textTotal = (TextView) view.findViewById(R.id.textTotal);
        text100L = (TextView) view.findViewById(R.id.text100L);
        text50L = (TextView) view.findViewById(R.id.text50L);
        text10L = (TextView) view.findViewById(R.id.text10L);

        plus100L = (Button) view.findViewById(R.id.plus100L);
        plus50L = (Button) view.findViewById(R.id.plus50L);
        plus10L = (Button) view.findViewById(R.id.plus10L);
        minus100L = (Button) view.findViewById(R.id.minus100L);
        minus50L = (Button) view.findViewById(R.id.minus50L);
        minus10L = (Button) view.findViewById(R.id.minus10L);

        addUsingContentProviderButton.setOnClickListener(this);

        plus100L.setOnClickListener(this);
        plus50L.setOnClickListener(this);
        plus10L.setOnClickListener(this);
        minus100L.setOnClickListener(this);
        minus50L.setOnClickListener(this);
        minus10L.setOnClickListener(this);

        // Inflate the layout for this fragment
        return view;
    }

    public int getTotal()
    {
        return (count100L * 100) + (count50L * 50) + (count10L * 10);
    }

    @Override
    public void onClick(View v) {


        final Calendar c = Calendar.getInstance();

        c.set(Calendar.YEAR, c.get(Calendar.YEAR));
        c.set(Calendar.MONTH, c.get(Calendar.MONTH));
        c.set(Calendar.DAY_OF_MONTH, c.get(Calendar.DAY_OF_MONTH));

        final Calendar d = Calendar.getInstance();
        d.setTimeInMillis(c.getTimeInMillis());
        d.roll(Calendar.HOUR, +0);

        switch (v.getId())
        {
            case R.id.plus100L : text100L.setText("" + (++count100L)); break;
            case R.id.minus100L :
                if((count100L > 0) && (getTotal() > 0))
                {
                    text100L.setText("" + (--count100L));
                }
                break;
            case R.id.plus50L : text50L.setText("" + (++count50L)); break;
            case R.id.minus50L :
                if((count50L > 0) && (getTotal() > 0))
                {
                    text50L.setText("" + (--count50L));
                }
                break;

            case R.id.plus10L : text10L.setText("" + (++count10L)); break;
            case R.id.minus10L :
                if((count10L > 0) && (getTotal() > 0))
                {
                    text10L.setText("" + (--count10L));
                }
                break;



            case R.id.addUsingContentProviderButton:

                try {

                    /**
                     *  1. Google Calender에 set
                     */
                    String packageName = getClass().getPackage().getName();
                    EventAdder eventAdder = (EventAdder)Class.forName(packageName + ".AddUsingContentProvider").newInstance();
                    title = textTotal.getText().toString() ;
                    eventAdder.addEvent(getActivity(), title, c, d);

                    /**
                     *  2. UserHistory에 add(+ SQL DB에도 set)
                     */
                    if((count10L != 0) || (count50L != 0) || (count100L != 0))
                    {
                        String temp;
                        UserHistoryFragment.setListData("A1", "장성동 153-12번지", title);
                        temp = "A1," + "장성동 153-12번지," + title;
                        try{
                            sendNotificationToCleaners(temp);
                        } catch(Exception ex){
                            ex.printStackTrace();
                        }
                    }

                } catch (Exception e) {
                    Toast.makeText(getActivity(), "Can't load AddUsingContentProvider: " + e, Toast.LENGTH_LONG).show();
                }
                break;

        }

        textTotal.setText("" + getTotal());
    }

    public static void sendNotificationToCleaners(final String message) {

        FirebaseDatabase database = FirebaseDatabase.getInstance();
        DatabaseReference myRef = database.getReference("notificationRequests");
        myRef.setValue(message);

    }
}
