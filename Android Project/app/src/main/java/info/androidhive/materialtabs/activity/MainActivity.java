package info.androidhive.materialtabs.activity;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.Toast;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import info.androidhive.materialtabs.R;


public class MainActivity extends AppCompatActivity {

    private Toolbar toolbar;
    private Button signupBtn;
    private Button signinBtn;
    private RadioButton userBtn;
    private RadioButton cleanerBtn;
    private EditText emailEditText;
    private EditText passEditText;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Address the email and password field
        signupBtn = (Button) findViewById(R.id.signupBtn);
        signinBtn = (Button) findViewById(R.id.signinBtn);
        userBtn = (RadioButton) findViewById(R.id.userRadioButton);
        cleanerBtn = (RadioButton) findViewById(R.id.cleanerRadioButton);
        emailEditText = (EditText) findViewById(R.id.username);
        passEditText = (EditText) findViewById(R.id.password);

        toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        // 회원가입
        signupBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                // 회원가입 Activity로 이동!
                Intent intent = new Intent(getApplicationContext(), SignupActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(intent);
                finish();
            }
        });

        // 로그인
        signinBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 1. login 유효성 조사
                if(isAvailableLogin())
                {
                    Intent intent;
                    // 2. 사용자, 청소부 조사
                    if(userBtn.isChecked())
                    {
                        intent = new Intent(getApplicationContext(), UserActivity.class);
                        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        intent.putExtra("from", "UserActivity");
                        startActivity(intent);
                        finish();

                    }else if(cleanerBtn.isChecked())
                    {
                        intent = new Intent(getApplicationContext(), CleanerActivity.class);
                        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        intent.putExtra("from", "CleanerActivity");
                        startActivity(intent);
                        finish();
                    }else
                    {
                        Toast.makeText(getApplicationContext(), "select the button", Toast.LENGTH_LONG).show();
                    }
                }
            }
        });

    }

    public boolean isAvailableLogin()
    {
        final String email = emailEditText.getText().toString();
        if (!isValidEmail(email)) {
            //Set error message for email field
            emailEditText.setError("Invalid Email");
            return false;
        }

        final String pass = passEditText.getText().toString();
        if (!isValidPassword(pass)) {
            //Set error message for password field
            passEditText.setError("Password cannot be empty");
            return false;
        }

        return true;
    }

    // validating email id
    private boolean isValidEmail(String email) {
        String EMAIL_PATTERN = "^[_A-Za-z0-9-\\+]+(\\.[_A-Za-z0-9-]+)*@"
                + "[A-Za-z0-9-]+(\\.[A-Za-z0-9]+)*(\\.[A-Za-z]{2,})$";

        Pattern pattern = Pattern.compile(EMAIL_PATTERN);
        Matcher matcher = pattern.matcher(email);
        return matcher.matches();
    }

    // validating password
    private boolean isValidPassword(String pass) {
        if (pass != null && pass.length() >= 4) {
            return true;
        }
        return false;
    }

    public void onRadioBtnClicked(View v)
    {
        // toggle
        switch(v.getId())
        {
            case R.id.userRadioButton:
                cleanerBtn.setChecked(false);
                break;

            case R.id.cleanerRadioButton:
                userBtn.setChecked(false);
                break;
        }

    }

}