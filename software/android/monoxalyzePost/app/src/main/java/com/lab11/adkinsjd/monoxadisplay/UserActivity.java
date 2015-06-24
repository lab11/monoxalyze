package com.lab11.adkinsjd.monoxadisplay;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;


public class UserActivity extends ActionBarActivity {

    SharedPreferences sharedPref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user);

        sharedPref = this.getSharedPreferences(getString(R.string.preference_file_key), Context.MODE_PRIVATE);
        boolean previouslyLoggedIn = sharedPref.getBoolean(getString(R.string.previously_logged_in), false);

        //if there are already preferences then turn off hints and fill the boxes
        if(previouslyLoggedIn) {
            EditText usernameText = (EditText)findViewById(R.id.username);
            EditText useridText = (EditText)findViewById(R.id.userid);
            EditText useremailText = (EditText)findViewById(R.id.useremail);
            usernameText.setHint("");
            usernameText.setText(sharedPref.getString(getString(R.string.username).toString(), ""));
            useridText.setHint("");
            useridText.setText(sharedPref.getString(getString(R.string.userid).toString(), ""));
            useremailText.setHint("");
            useremailText.setText(sharedPref.getString(getString(R.string.useremail).toString(), ""));
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        //getMenuInflater().inflate(R.menu.menu_user, menu);
        return true;
    }

    public void saveButton(View view) {
        //get strings
        EditText usernameText = (EditText)findViewById(R.id.username);
        EditText useridText = (EditText)findViewById(R.id.userid);
        EditText useremailText = (EditText)findViewById(R.id.useremail);

        //check all of the strings for validity

        //if valid save preferences
        SharedPreferences.Editor editPref = sharedPref.edit();
        editPref.putString(getString(R.string.username),usernameText.getText().toString());
        editPref.putString(getString(R.string.userid), useridText.getText().toString());
        editPref.putString(getString(R.string.useremail),useremailText.getText().toString());
        editPref.putBoolean(getString(R.string.previously_logged_in), true);
        editPref.commit();

        Intent intent = new Intent(this,DisplayActivity.class);
        startActivity(intent);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
