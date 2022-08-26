package com.example.robotcontrol

import android.annotation.SuppressLint
import android.app.ProgressDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.os.AsyncTask
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import java.io.IOException
import java.util.*


class MainActivity : AppCompatActivity() {

    companion object {
        var myUUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
        var bluetoothSocket: BluetoothSocket? = null
        lateinit var progressDialog: ProgressDialog
        lateinit var bluetoothAdapter: BluetoothAdapter
        var isConnected: Boolean = false
        lateinit var clientAddress: String
    }

    private val imageDown: ImageView
        get() = findViewById(R.id.imageDown)
    private val imageUp: ImageView
        get() = findViewById(R.id.imageUp)
    private val imageLeft: ImageView
        get() = findViewById(R.id.imageLeft)
    private val imageRight: ImageView
        get() = findViewById(R.id.imageRight)

    private val textView: TextView
        get() = findViewById(R.id.textView)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        clientAddress = intent.getStringExtra(ConfigurationActivity.choosenAddress).toString()

        //WHEN MAIN ACTIVITY OPEN THEN EXECUTE CONNECTION
        ConnectThread(this).execute()


        imageDown.setOnTouchListener { v: View, m: MotionEvent ->
            onTouch(m, "down")
            true
        }
        imageUp.setOnTouchListener { v: View, m: MotionEvent ->
            onTouch(m, "up")
            true
        }
        imageLeft.setOnTouchListener { v: View, m: MotionEvent ->
            onTouch(m, "left")
            true
        }
        imageRight.setOnTouchListener { v: View, m: MotionEvent ->
            onTouch(m, "right")
            true
        }

    }

    private fun onTouch(event: MotionEvent?, command: String): Boolean {
        when (event?.action) {
            MotionEvent.ACTION_DOWN -> {
                sendCommand(command)
            }
            MotionEvent.ACTION_UP -> {
                sendCommand("stop")
            }
        }
        Log.i("info", event.toString())
        return true
    }

    private fun sendCommand(cmd: String) {
        if (bluetoothSocket != null) {
            try {
                bluetoothSocket!!.outputStream.write(cmd.toByteArray())
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    fun readComand(v: View?){
        if (bluetoothSocket != null) {
            try {
                val inputBufferSize = bluetoothSocket!!.inputStream.available()
                //create buffer with appropriate size
                val inputBuffer = ByteArray(inputBufferSize)
                Log.i("info", "Reading")

                //inputStream -> inputBuffer variable
                bluetoothSocket!!.inputStream.read(inputBuffer)

                //ByteArray to String conversion
                val text = String(inputBuffer)
                Log.i("info", String(inputBuffer))
                //display read text
                textView.text = text
            } catch (e: IOException) {
                Log.i("info", "Reading failed")
                e.printStackTrace()
            }
        }

    }

    fun disconnect(v: View?) {
        if (bluetoothSocket != null) {
            try {
                bluetoothSocket!!.close()
                bluetoothSocket = null
                isConnected = false
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
        finish()
    }

    private class ConnectThread(c: Context) : AsyncTask<Void, Void, String>() {
        private var connectSuccess: Boolean = true
        private val context: Context


        init {
            this.context = c
        }

        override fun onPreExecute() {
            super.onPreExecute()
            progressDialog = ProgressDialog.show(context, "Connecting in progress", "Wait a second")
        }

        @SuppressLint("MissingPermission")
        override fun doInBackground(vararg p0: Void?): String? {
            //Connect as a client
            try {
                if (bluetoothSocket == null || !isConnected) {
                    bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
                    val device: BluetoothDevice = bluetoothAdapter.getRemoteDevice(clientAddress)
                    bluetoothSocket = device.createRfcommSocketToServiceRecord(myUUID)
                    // Cancel discovery because it otherwise slows down the connection.
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
                    bluetoothSocket!!.connect()
                }

            } catch (e: IOException) {
                connectSuccess = false
                e.printStackTrace()
            }

            return null
        }

        override fun onPostExecute(result: String?) {

            super.onPostExecute(result)
            if (!connectSuccess) {
                Log.i("info", "Connection failed")
            } else {
                Log.i("info", "Connection OK")
                isConnected = true
            }
            //TURN OFF PROGRESS DIALOG
            progressDialog.dismiss()
        }
    }
}