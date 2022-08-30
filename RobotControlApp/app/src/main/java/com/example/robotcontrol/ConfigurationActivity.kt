package com.example.robotcontrol

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.*
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.core.view.isVisible


class ConfigurationActivity : AppCompatActivity() {

    private val bluetoothAdapter: BluetoothAdapter? = BluetoothAdapter.getDefaultAdapter()


    private val listView: ListView
        get() = findViewById(R.id.listView)

    private val bluetoothImage: ImageView
        get() = findViewById(R.id.bluetoothImage)

    private val REQUEST_ENABLE_BLUETOOTH = 1
    private var listVisible: Boolean = false

    val REQUEST_ID_MULTIPLE_PERMISSIONS = 1;

    companion object {
        var choosenAddress: String = "choosenAddress"
        var choosenName: String = "choosenName"
    }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_configuration)

        checkAndRequestPermissions()


        // Device doesn't support Bluetooth
        if (bluetoothAdapter == null) {
            Toast.makeText(
                applicationContext,
                "Device doesn't support Bluetooth",
                Toast.LENGTH_LONG
            ).show()
        }

        // BLUETOOH IMAGE: BLUE IF BLUETOOTH TURNED ON, GREY IF TURNED OFF
        if (bluetoothAdapter?.isEnabled == false) {
            bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.grey))
        } else {
            bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.blue))
        }
    }

    private fun checkAndRequestPermissions() {
        val bthConnect =
            ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT)
        val bthAdmin =
            ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_ADMIN)

        val bthScan =
            ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_SCAN)

        if (bthConnect != PackageManager.PERMISSION_GRANTED) {

            Log.i("info", "bthConnect != PackageManager.PERMISSION_GRANTED")

            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.BLUETOOTH_CONNECT),
                201
            )
        }
        if (bthAdmin != PackageManager.PERMISSION_GRANTED) {

            Log.i("info", "bthAdmin != PackageManager.PERMISSION_GRANTED")

            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.BLUETOOTH_ADMIN),
                202
            )
        }
        if (bthScan != PackageManager.PERMISSION_GRANTED) {

            Log.i("info", "bthScan != PackageManager.PERMISSION_GRANTED")

            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.BLUETOOTH_SCAN),
                203
            )
        }
    }

    //ACTIONS callback
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        //turnOn - permission allowed by user
        if (requestCode == REQUEST_ENABLE_BLUETOOTH) {
            if (resultCode == RESULT_OK) {
                bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.blue))
            } else {
                bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.grey))
            }
        }
    }


    fun turnOn(v: View?) {
        if (bluetoothAdapter?.isEnabled == false) {
            val turnOn = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH)
                == PackageManager.PERMISSION_GRANTED
            ) {
                startActivityForResult(turnOn, REQUEST_ENABLE_BLUETOOTH)
            } else {
                Toast.makeText(
                    applicationContext,
                    "BLUETOOTH permission required!",
                    Toast.LENGTH_SHORT
                ).show()
            }

        } else {
            Toast.makeText(applicationContext, "Bluetooth already on", Toast.LENGTH_SHORT).show()
        }

    }

    //@SuppressLint("MissingPermission")
    fun turnOff(v: View?) {
        bluetoothImage.setColorFilter(bluetoothImage.context.resources.getColor(R.color.grey))

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH)
            == PackageManager.PERMISSION_GRANTED
        ) {
            bluetoothAdapter?.disable()
        } else {
            Toast.makeText(
                applicationContext,
                "BLUETOOTH permission required!",
                Toast.LENGTH_SHORT
            ).show()
        }

    }


    //@SuppressLint("MissingPermission")
    fun listDevices(v: View?) {
        if (bluetoothAdapter?.isEnabled == false) {
            Toast.makeText(applicationContext, "Bluetooth is turned off!", Toast.LENGTH_SHORT)
                .show()
        }

        if (ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.BLUETOOTH
            ) == PackageManager.PERMISSION_GRANTED
        ) {
            val pairedDevices = bluetoothAdapter?.bondedDevices
            //LIST ONLY WITH ADRRESSES
            val pairedList = ArrayList<BluetoothDevice>()
            //DISPLAYED LIST - NAMES AND ADDRESSES
            val displayedList = ArrayList<Any>()
            if (!listVisible) {
                for (dev in pairedDevices!!) {
                    pairedList.add(dev)
                    displayedList.add(dev.name + "\t \t" + dev.address)
                }
                val adapter: ArrayAdapter<*> =
                    ArrayAdapter(this, android.R.layout.simple_list_item_1, displayedList)
                listView.adapter = adapter
                listView.isVisible = true

                listView.onItemClickListener = AdapterView.OnItemClickListener { _, _, i, _ ->
                    val device: BluetoothDevice = pairedList[i]
                    val address: String = device.address
                    val name: String = device.name

                    val intent  = Intent(this, MainActivity::class.java)
                    intent.putExtra(choosenAddress, address)
                    intent.putExtra(choosenName, name)
                    startActivity(intent)
                }
                listVisible = true

            } else {
                listView.isVisible = false
                listVisible = false
            }
        } else {
            Toast.makeText(
                applicationContext,
                "BLUETOOTH_CONNECT permission required",
                Toast.LENGTH_SHORT
            ).show()
        }

    }

}