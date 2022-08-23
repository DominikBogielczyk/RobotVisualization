package com.example.robotcontrol

import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.content.Intent
import android.content.res.Resources
import android.graphics.PorterDuff
import android.os.Bundle
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import android.content.Context
import androidx.activity.result.contract.ActivityResultContracts
import java.net.Authenticator


class MainActivity : AppCompatActivity() {

    var bluetoothAdapter: BluetoothAdapter? = BluetoothAdapter.getDefaultAdapter()

    @SuppressLint("MissingPermission")
    var pairedDevices = bluetoothAdapter?.getBondedDevices()

    private val lv:ListView
        get() = findViewById(R.id.listView)

    private val bluetoothImage:ImageView
        get() = findViewById(R.id.bluetoothImage)
  //  private val b1:Button
   //     get() = findViewById(R.id.button)
  //  private val b2:Button
  //      get() = findViewById(R.id.button2)
  //  private val b3:Button
  //      get() = findViewById(R.id.button3)
  //  private val b4:Button
   //     get() = findViewById(R.id.button4)

    var list_visible : Boolean = false

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        //HIDE ACTION BAR "ROBOTCONTROL"
        getSupportActionBar()?.setDisplayShowTitleEnabled(false)

        if (bluetoothAdapter?.isEnabled == false)
        {
            bluetoothImage.setColorFilter(bluetoothImage.getContext().getResources().getColor(R.color.grey))
        }
        else
        {
            bluetoothImage.setColorFilter(bluetoothImage.getContext().getResources().getColor(R.color.blue))
        }
    }

    //ACTIONS callback
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?)
    {
        super.onActivityResult(requestCode, resultCode, data)

        //turnOn or getVisible Permission allowed by user
        if(resultCode == RESULT_OK || resultCode == 120)
        {
            bluetoothImage.setColorFilter(bluetoothImage.getContext().getResources().getColor(R.color.blue))
        }
        else
        {
            bluetoothImage.setColorFilter(bluetoothImage.getContext().getResources().getColor(R.color.grey))
        }

    }


    @SuppressLint("MissingPermission")
    fun turnOn(v: View?)
    {
        if (bluetoothAdapter?.isEnabled == false)
        {
            val turnOn = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(turnOn, 0)
        }
        else
        {
            Toast.makeText(applicationContext, "Bluetooth already on", Toast.LENGTH_SHORT).show()
        }
    }

    @SuppressLint("MissingPermission")
    fun turnOff(v: View?)
    {
        bluetoothAdapter?.disable()
        bluetoothImage.setColorFilter(bluetoothImage.getContext().getResources().getColor(R.color.grey))
    }


    @SuppressLint("MissingPermission")
    fun getVisible(v: View?)
    {
        val getVisible = Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE)
        startActivityForResult(getVisible, 0)
    }


    @SuppressLint("MissingPermission")
    fun listDevices(v: View?)
    {
        if (bluetoothAdapter?.isEnabled == false)
        {
            Toast.makeText(applicationContext, "Bluetooth is turned off!", Toast.LENGTH_SHORT).show()
        }


        pairedDevices = bluetoothAdapter?.bondedDevices
        val pairedList = ArrayList<Any>()
        if (!list_visible)
        {
            for (bt in pairedDevices!!)
            {
                pairedList.add(bt.name)
            }
            val adapter: ArrayAdapter<*> = ArrayAdapter(this, android.R.layout.simple_list_item_1, pairedList)
            lv.setAdapter(adapter)
            list_visible = true
            lv.isVisible = true
        }
        else
        {
            lv.isVisible = false
            list_visible = false
        }


    }


}