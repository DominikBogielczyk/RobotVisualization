package com.example.robotcontrol

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.core.view.isVisible
import com.example.robotcontrol.databinding.ActivityConfigurationBinding


class ConfigurationActivity : AppCompatActivity() {

    private val bluetoothAdapter: BluetoothAdapter? = BluetoothAdapter.getDefaultAdapter()
    private lateinit var binding : ActivityConfigurationBinding

    private val requestEnableBluetooth = 1
    private var listVisible: Boolean = false

    companion object {
        var choosenAddress: String = "choosenAddress"
        var choosenName: String = "choosenName"
    }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        binding = ActivityConfigurationBinding.inflate(LayoutInflater.from(this))
        setContentView(binding.root)

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
            binding.bluetoothImage.setColorFilter(ContextCompat.getColor(this, R.color.grey))
        } else {
            binding.bluetoothImage.setColorFilter(ContextCompat.getColor(this, R.color.blue))
        }
    }
    //ACTIONS callback
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        //turnOn - permission allowed by user
        if (requestCode == requestEnableBluetooth) {
            if (resultCode == RESULT_OK) {
                binding.bluetoothImage.setColorFilter(ContextCompat.getColor(this, R.color.blue))
            } else {
                binding.bluetoothImage.setColorFilter(ContextCompat.getColor(this, R.color.grey))
            }
        }
    }


    fun turnOn(@Suppress("UNUSED_PARAMETER")view: View) {
        if (bluetoothAdapter?.isEnabled == false) {
            val turnOn = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH)
                == PackageManager.PERMISSION_GRANTED
            ) {
                startActivityForResult(turnOn, requestEnableBluetooth)
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

    fun turnOff(@Suppress("UNUSED_PARAMETER")view: View) {
        binding.bluetoothImage.setColorFilter(ContextCompat.getColor(this, R.color.grey))

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

    fun listDevices(@Suppress("UNUSED_PARAMETER")view: View) {
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
                binding.listView.adapter = adapter
                binding.listView.isVisible = true

                binding.listView.onItemClickListener = AdapterView.OnItemClickListener { _, _, i, _ ->
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
                binding.listView.isVisible = false
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