package info.androidhive.materialtabs.fragments;

import android.content.Context;

import java.util.Calendar;

public interface EventAdder {
    void addEvent(Context ctx, String title, Calendar start, Calendar end);
}
