package com.example.robotcontrol

import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Intent

import android.os.Bundle
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import kotlin.collections.ArrayList


class ConfigurationActivity : AppCompatActivity() {

    private val bluetoothAdapter: BluetoothAdapter? = BluetoothAdapter.getDefaultAdapter()

    @SuppressLint("MissingPermission")
    private var pairedDevices = bluetoothAdapter?.bondedDevices

    private val listView :ListView
        get() = findViewById(R.id.listView)

    private val bluetoothImage:ImageView
        get() = findViewById(R.id.bluetoothImage)

    private val REQUEST_ENABLE_BLUETOOTH = 1
    private var listVisible : Boolean = false

    companion object
    {
        var choosenAddress : String = " "
    }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_configuration)

        //HIDE ACTION BAR "ROBOTCONTROL"
        supportActionBar?.setDisplayShowTitleEnabled(false)

        // Device doesn't support Bluetooth
        if (bluetoothAdapter == null)
        {
            Toast.makeText(applicationContext, "Device doesn't support Bluetooth", Toast.LENGTH_LONG).show()
        }

        // BLUETOOH IMAGE: BLUE IF BLUETOOTH TURNED ON, GREY IF TURNED OFF
        if (bluetoothAdapter?.isEnabled == false)
        {
            bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.grey))
        }
        else
        {
            bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.blue))
        }
    }

    //ACTIONS callback
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?)
    {
        super.onActivityResult(requestCode, resultCode, data)

        //turnOn - permission allowed by user
        if(requestCode == REQUEST_ENABLE_BLUETOOTH)
        {
            if(resultCode == RESULT_OK)
            {
                bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.blue))
            }
            else
            {
                bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.grey))
            }
        }
    }


    @SuppressLint("MissingPermission")
    fun turnOn(v: View?)
    {
        if (bluetoothAdapter?.isEnabled == false)
        {
            val turnOn = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(turnOn, REQUEST_ENABLE_BLUETOOTH)
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
    fun listDevices(v: View?)
    {
        if (bluetoothAdapter?.isEnabled == false)
        {
            Toast.makeText(applicationContext, "Bluetooth is turned off!", Toast.LENGTH_SHORT).show()
        }

        pairedDevices = bluetoothAdapter?.bondedDevices
        //LIST ONLY WITH ADRRESSES
        val pairedList = ArrayList<BluetoothDevice>()
        //DISPLAYED LIST - NAMES AND ADDRESSES
        val displayedList = ArrayList<Any>()
        if (!listVisible)
        {
            for (dev in pairedDevices!!)
            {
                pairedList.add(dev)
                displayedList.add(dev.name + "\t \t" + dev.address)
            }
            val adapter: ArrayAdapter<*> = ArrayAdapter(this, android.R.layout.simple_list_item_1, displayedList)
            listView.adapter = adapter
            listView.isVisible = true

            listView.onItemClickListener = AdapterView.OnItemClickListener { _, _, i, _ ->
                val device: BluetoothDevice = pairedList[i]
                val address: String = device.address

                val intent = Intent(this, MainActivity::class.java)
                intent.putExtra(choosenAddress, address)
                startActivity(intent)
            }
            listVisible = true

        }
        else
        {
            listView.isVisible = false
            listVisible = false
        }
    }

}