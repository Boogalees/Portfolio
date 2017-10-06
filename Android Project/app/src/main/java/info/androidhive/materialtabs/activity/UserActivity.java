package info.androidhive.materialtabs.activity;

import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.messaging.FirebaseMessaging;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import info.androidhive.materialtabs.R;
import info.androidhive.materialtabs.fragments.UserHistoryFragment;
import info.androidhive.materialtabs.fragments.UserTrashFragment;


public class UserActivity extends AppCompatActivity {

    // content Provider
    public static final String TAG = "CalAdder";
    Calendar mCal = Calendar.getInstance();
    private EditText string_title;

    String location = "Boardroom";
    int duration = 1;	// hours


    Button addUsingContentProvider;

    //---------- UserActivity -----------//
    private TabLayout tabLayout;
    private ViewPager viewPager;

    TextView textTotal;
    TextView text100L;
    TextView text50L;
    TextView text10L;

    private int count100L;
    private int count50L;
    private int count10L;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_custom_view_icon_text_tabs);

        viewPager = (ViewPager) findViewById(R.id.viewpager);
        setupViewPager(viewPager);

        textTotal = (TextView) findViewById(R.id.textTotal);
        text100L = (TextView) findViewById(R.id.text100L);
        text50L = (TextView) findViewById(R.id.text50L);
        text10L = (TextView) findViewById(R.id.text10L);

        tabLayout = (TabLayout) findViewById(R.id.tabs);
        tabLayout.setupWithViewPager(viewPager);
        setupTabIcons();
    }

    /**
     * Adding custom view to tab
     */
    private void setupTabIcons() {

        TextView tabOne = (TextView) LayoutInflater.from(this).inflate(R.layout.custom_tab, null);
        tabOne.setText("Trash");
        // tabOne.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_tab_favourite, 0, 0);
        tabLayout.getTabAt(0).setCustomView(tabOne);

        TextView tabTwo = (TextView) LayoutInflater.from(this).inflate(R.layout.custom_tab, null);
        tabTwo.setText("List");
        // tabTwo.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_tab_call, 0, 0);
        tabLayout.getTabAt(1).setCustomView(tabTwo);

    }

    /**
     * Adding fragments to ViewPager
     * @param viewPager
     */
    private void setupViewPager(ViewPager viewPager) {
        ViewPagerAdapter adapter = new ViewPagerAdapter(getSupportFragmentManager());
        adapter.addFrag(new UserTrashFragment(), "ONE");
        adapter.addFrag(new UserHistoryFragment(), "TWO");
        viewPager.setAdapter(adapter);
    }


    /**
     * for UserTrashFragment
     * @param v
     */
    public void onCountBtnClicked(View v)
    {
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
        }

        textTotal.setText("" + getTotal());
    }
    public int getTotal()
    {
        return (count100L * 100) + (count50L * 50) + (count10L * 10);
    }



    /**
     *  for UserHistoryFragment -------------------------------------------------
     */




    class ViewPagerAdapter extends FragmentPagerAdapter {
        private final List<Fragment> mFragmentList = new ArrayList<>();
        private final List<String> mFragmentTitleList = new ArrayList<>();

        public ViewPagerAdapter(FragmentManager manager) {
            super(manager);
        }

        @Override
        public Fragment getItem(int position) {
            return mFragmentList.get(position);
        }

        @Override
        public int getCount() {
            return mFragmentList.size();
        }

        public void addFrag(Fragment fragment, String title)
        {
            mFragmentList.add(fragment);
            mFragmentTitleList.add(title);
        }

        @Override
        public CharSequence getPageTitle(int position) {
            return mFragmentTitleList.get(position);
        }
    }




}
