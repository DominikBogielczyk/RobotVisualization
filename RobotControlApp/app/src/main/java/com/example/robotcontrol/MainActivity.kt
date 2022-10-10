package com.example.robotcontrol

import android.Manifest
import android.app.ProgressDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.pm.PackageManager
import android.os.AsyncTask
import android.os.Build
import android.os.Bundle
import android.os.CountDownTimer
import android.util.Log
import android.view.MotionEvent
import android.view.View
import android.widget.*
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import com.google.android.material.slider.Slider


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
        lateinit var clientName: String
    }

    private val imageDown: ImageView
        get() = findViewById(R.id.imageDown)
    private val imageUp: ImageView
        get() = findViewById(R.id.imageUp)
    private val imageLeft: ImageView
        get() = findViewById(R.id.imageLeft)
    private val imageRight: ImageView
        get() = findViewById(R.id.imageRight)
    private val imageCamera: ImageView
        get() = findViewById(R.id.imageCamera)
    private val textView: TextView
        get() = findViewById(R.id.textView)
    private val delayTextView: TextView
        get() = findViewById(R.id.delayTextView)
    private val infoText: TextView
        get() = findViewById(R.id.info)
    private val modeButton: ToggleButton
        get() = findViewById(R.id.modeButton)
    private val test: TextView
        get() = findViewById(R.id.test)
    private val slider: Slider
        get() = findViewById(R.id.slider)

    private var cmdIndex = 0
    private var cmdTime: Long = 0
    private var velocity: Float = 5.0F

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        clientAddress = intent.getStringExtra(ConfigurationActivity.choosenAddress).toString()
        clientName = intent.getStringExtra(ConfigurationActivity.choosenName).toString()

        //BEGIN CONNECTION
        ConnectThread(this).execute()

        var textToSend = ""
        imageDown.setOnTouchListener { v: View, m: MotionEvent ->
            textToSend = "wp" + (-velocity).toString() + "wl" + (-velocity).toString()
            onTouch(m, textToSend, sendStop = true)
            true
        }
        imageUp.setOnTouchListener { v: View, m: MotionEvent ->
            textToSend = "wp" + velocity.toString() + "wl" + velocity.toString()
            onTouch(m, textToSend, sendStop = true)
            true
        }
        imageLeft.setOnTouchListener { v: View, m: MotionEvent ->
            textToSend = "wp" + velocity.toString() + "wl" + (-velocity).toString()
            onTouch(m, textToSend, sendStop = true)
            true
        }
        imageRight.setOnTouchListener { v: View, m: MotionEvent ->
            textToSend = "wp" + (-velocity).toString() + "wl" + velocity.toString()
            onTouch(m, textToSend, sendStop = true)
            true
        }
        imageCamera.setOnTouchListener { v: View, m: MotionEvent ->
            onTouch(m, "camera", sendStop = false)
            true
        }
        modeButton.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                changeVisibility(1)
            } else {
                changeVisibility(2)
            }
        }
        slider.addOnSliderTouchListener(object : Slider.OnSliderTouchListener {
            override fun onStartTrackingTouch(slider: Slider) {
                //
            }

            override fun onStopTrackingTouch(slider: Slider) {
                //
            }
        })

        slider.addOnChangeListener { slider, value, fromUser ->
            velocity = slider.value
            test.text = velocity.toString()
        }


        readTimer()

    }


    @RequiresApi(Build.VERSION_CODES.O)
    private fun onTouch(event: MotionEvent?, command: String, sendStop: Boolean): Boolean {
        var text = " "
        var size = ""
        when (event?.action) {
            MotionEvent.ACTION_DOWN -> {
                text = command
                size = text.toByteArray().size.toString() + "B"
                sendCommand("$text;$cmdIndex;$size")
                cmdTime = System.currentTimeMillis()
                cmdIndex += 1
                cmdIndex = cmdIndex % 9
            }
            MotionEvent.ACTION_UP -> {
                if (sendStop) {
                    text = "wp0.0wl0.0"
                    size = text.toByteArray().size.toString() + "B"
                    sendCommand("$text;$cmdIndex;$size")
                    cmdTime = System.currentTimeMillis()
                    cmdIndex += 1
                    cmdIndex = cmdIndex % 9
                }
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
                infoText.text = clientName + " connection failed"
                infoText.setTextColor(this.resources.getColor(R.color.red))
                changeVisibility(0)
            }
        }
    }

    @RequiresApi(Build.VERSION_CODES.O)
    fun readComand() {
        if (bluetoothSocket != null) {
            try {
                val inputBufferSize = bluetoothSocket!!.inputStream.available()
                //create buffer with appropriate size
                if (inputBufferSize > 0) {
                    val inputBuffer = ByteArray(inputBufferSize)
                    Log.i("info", "Reading")

                    //inputStream -> inputBuffer variable
                    bluetoothSocket!!.inputStream.read(inputBuffer)

                    //ByteArray to String conversion
                    val text = String(inputBuffer)
                    Log.i("info", text)

                    if ("collision" in text) {
                        textView.text = "Collision !!!"
                        textView.setTextColor(getResources().getColor(R.color.red))
                    } else if ("ok" in text) {
                        textView.text = "OK"
                        textView.setTextColor(getResources().getColor(R.color.green))
                    }
                    if (text.contains("[0-9]".toRegex())) {

                        for (i in 0..9) {
                            if (i.toString() in text) {
                                var t = System.currentTimeMillis() - cmdTime
                                delayTextView.text = t.toString() + " ms"
                            }
                        }
                    }

                    //display read text

                }
            } catch (e: IOException) {
                Log.i("info", "Reading failed")
                // infoText.text = "Connection failed"
                // infoText.setTextColor(this.resources.getColor(R.color.red))
                // changeVisibility(false)
                e.printStackTrace()
            }
        }
    }

    fun clear() {
        textView.text = ""
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

    fun changeVisibility(v: Int) {
        when (v) {
            0 -> {
                imageDown.visibility = View.INVISIBLE
                imageUp.visibility = View.INVISIBLE
                imageLeft.visibility = View.INVISIBLE
                imageRight.visibility = View.INVISIBLE
            }
            1 -> {
                imageDown.visibility = View.INVISIBLE
                imageUp.visibility = View.INVISIBLE
                imageLeft.visibility = View.INVISIBLE
                imageRight.visibility = View.INVISIBLE
            }
            2 -> {
                imageDown.visibility = View.VISIBLE
                imageUp.visibility = View.VISIBLE
                imageLeft.visibility = View.VISIBLE
                imageRight.visibility = View.VISIBLE
            }
        }

    }

    fun readTimer() {
        object : CountDownTimer(10000000, 1) {
            @RequiresApi(Build.VERSION_CODES.O)
            override fun onTick(millisUntilFinished: Long) {
                readComand()
            }

            override fun onFinish() {
                this.start()
            }
        }.start()
    }

    private inner class ConnectThread(c: Context) : AsyncTask<Void, Void, String>() {
        private var connectSuccess: Boolean = true
        private val context: Context

        init {
            this.context = c
        }

        override fun onPreExecute() {
            super.onPreExecute()
            progressDialog = ProgressDialog.show(context, "Connecting in progress", "Wait a second")
        }


        override fun doInBackground(vararg p0: Void?): String? {
            //Connect as a client
            try {
                //  connectSuccess = true
                if ((bluetoothSocket == null || !isConnected) && ContextCompat.checkSelfPermission(
                        context,
                        Manifest.permission.BLUETOOTH
                    )
                    == PackageManager.PERMISSION_GRANTED
                ) {
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
                this@MainActivity.infoText.text = clientName + " connection failed"
                this@MainActivity.infoText.setTextColor(context.resources.getColor(R.color.red))
                this@MainActivity.changeVisibility(0)

            } else {
                Log.i("info", "Connection OK")
                isConnected = true
                this@MainActivity.infoText.text = clientName + " connection OK"
                this@MainActivity.infoText.setTextColor(context.resources.getColor(R.color.green))

                //
            }
            //TURN OFF PROGRESS DIALOG
            progressDialog.dismiss()
        }
    }
}