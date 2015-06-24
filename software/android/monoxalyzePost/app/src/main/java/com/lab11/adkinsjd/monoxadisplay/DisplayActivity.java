package com.lab11.adkinsjd.monoxadisplay;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.util.UUID;


public class DisplayActivity extends ActionBarActivity {

    private String toConnect;
    private final static int REQUEST_ENABLE_BT = 1;
    BluetoothManager mBluetoothManager;
    BluetoothAdapter mBluetoothAdapter;
    BluetoothGatt mBluetoothGatt;
    Context ctx = this;
    BluetoothGattCharacteristic mGasChar;
    BluetoothGattCharacteristic mPressChar;
    private int descNumber = 0;

    private static final UUID CONFIG_DESCRIPTOR = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb");
    private static final UUID GasService = UUID.fromString("00001523-1212-efde-1523-785feabcd123");
    private static final UUID GasChar = UUID.fromString("00001525-1212-efde-1523-785feabcd123");
    private static final UUID PressChar = UUID.fromString("00001524-1212-efde-1523-785feabcd123");

    BluetoothGattDescriptor pressDescriptor;
    BluetoothGattDescriptor gasDescriptor;

    boolean isReading;
    final Object readLock = new Object();
    Handler mHandler;


    private boolean readWhich = false;

    private void doRead() {
        if(readWhich) {
            isReading = mBluetoothGatt.readCharacteristic(mGasChar);
            readWhich = false;
        } else {
            isReading = mBluetoothGatt.readCharacteristic(mPressChar);
            readWhich = true;
        }

        while(isReading) {
            synchronized (readLock) {
                try {
                    readLock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        SharedPreferences sharedPref = this.getSharedPreferences(getString(R.string.preference_file_key), Context.MODE_PRIVATE);
        boolean previouslyLoggedIn = sharedPref.getBoolean(getString(R.string.previously_logged_in), false);

        //If the user has never setup their account before take them to that screen
        if(previouslyLoggedIn == false) {
            Intent intent = new Intent(this,UserActivity.class);
            startActivity(intent);
        } else {
            setContentView(R.layout.activity_display_acitivity);
        }



        mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = mBluetoothManager.getAdapter();
        mHandler = new Handler();

        if (mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }
    }

    @Override
    protected void onResume() {
        mBluetoothAdapter.startLeScan(mLeScanCallback);
    }

    @Override
    protected void onPause() {
        mBluetoothGatt.disconnect();
        mBluetoothAdapter.stopLeScan(mLeScanCallback);
    }

    private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            if(newState == BluetoothProfile.STATE_CONNECTED) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                    TextView connectedText = (TextView)findViewById(R.id.ConnectedLabel);
                    connectedText.setText("Connected");
                    connectedText.setTextColor(Color.GREEN);
                    }
                });

                mBluetoothAdapter.stopLeScan(mLeScanCallback);
                mBluetoothGatt.discoverServices();
            }

            if(newState == BluetoothProfile.STATE_DISCONNECTED) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                    TextView connectedText = (TextView)findViewById(R.id.ConnectedLabel);
                    connectedText.setText("Disconnected");
                    connectedText.setTextColor(Color.RED);
                    }
                });
                mBluetoothAdapter.startLeScan(mLeScanCallback);
            }
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            if(characteristic.getUuid().equals(GasChar)) {
                Log.d("changed","gas characteristic changed");
                final int COValue = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT32, 0);
                Log.d("Gas Value",Integer.toString(COValue));
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        //TextView COText = (TextView)findViewById(R.id.COMeasure);
                        //COText.setText(Float.toString(COValue));
                        if(COValue < 10) {
                            //COText.setTextColor(Color.GREEN);
                        }
                        else {
                            //COText.setTextColor(Color.RED);
                        }
                    }
                });
            } /*else if (characteristic.getUuid().equals(PressChar)) {
                Log.d("changed","Press characteristic changed");
                final int PressValue = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT32, 0);
                final double pressure = PressValue/40.96;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        TextView PressText = (TextView)findViewById(R.id.PressMeasure);
                        PressText.setText(String.format("%.2f",pressure));
                    }
                });
                Log.d("Press Value",Integer.toString(PressValue));
            } else {
                Log.d("notification", "notification error");
            }*/

            synchronized (readLock) {
                isReading = false;
                readLock.notifyAll();
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            //Log.d("size",Integer.toString(gatt.getServices().size()));

            mGasChar = gatt.getService(GasService).getCharacteristic(GasChar);
            mPressChar = gatt.getService(GasService).getCharacteristic(PressChar);

            mPressChar.getDescriptors();
            mGasChar.getDescriptors();

            mBluetoothGatt.setCharacteristicNotification(mGasChar,true);
            mBluetoothGatt.setCharacteristicNotification(mPressChar,true);

            pressDescriptor = mPressChar.getDescriptor(CONFIG_DESCRIPTOR);
            pressDescriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);

            gasDescriptor = mGasChar.getDescriptor(CONFIG_DESCRIPTOR);
            gasDescriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);

            mBluetoothGatt.writeDescriptor(pressDescriptor);
            //mBluetoothGatt.writeDescriptor(descriptor2);


            /*new Thread(new Runnable() {
                @Override
                public void run() {
                    while(true) {
                        doRead();
                    }
                }
            }
            ).start();*/
        }

        @Override
        public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
            if(descNumber == 0) {
                mBluetoothGatt.writeDescriptor(gasDescriptor);
                descNumber += 1;
            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic character) {

            Log.d("Notifcaiton UUID", character.getUuid().toString());

            if (character.getUuid().equals(GasChar)) {
                //Log.d("changed","gas characteristic changed");
                int COValue = character.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT32, 0);
                COValue = (COValue -315)/10;
                if(COValue < 0) {
                    COValue = 0;
                }
                final int CoValue = COValue;

            } /*else if (character.getUuid().equals(PressChar)) {
                final int PressValue = character.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT32, 0);
                final double pressure = PressValue;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        TextView PressText = (TextView) findViewById(R.id.PressMeasure);
                        PressText.setText(String.format("%.2f", pressure));
                    }
                });
            } else if (character.getUuid().equals(TempChar)) {
                final int TempValue = character.getIntValue(BluetoothGattCharacteristic.FORMAT_FLOAT, 0);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        TextView PressText = (TextView) findViewById(R.id.TempMeasure);
                        PressText.setText(String.format("%.4f", TempValue));
                    }
                });
            }*/
        }
    };

    private BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {

                @Override
                public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {

                    if(device.getUuids()[0].getUuid() == GasService) {
                            mBluetoothGatt = device.connectGatt(ctx, true, mGattCallback);
                            mBluetoothAdapter.stopLeScan(mLeScanCallback);
                    }
                }
    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
       if (requestCode == REQUEST_ENABLE_BT && resultCode == Activity.RESULT_CANCELED) {
           finish();
           return;
       }
       super.onActivityResult(requestCode, resultCode, data);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_display_acitivity, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();


        switch (id) {
            case R.id.user_settings:
                Intent intent = new Intent(this,UserActivity.class);
                startActivity(intent);
                return true;
            case R.id.help:
                Intent intentHelp = new Intent(this,HelpActivity.class);
                startActivity(intentHelp);
                return true;
            case R.id.advanced_settings:
                Intent intentAdvanced = new Intent(this,AdvancedActivity.class);
                startActivity(intentAdvanced);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
}
