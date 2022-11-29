package com.example.robotcontrol

import android.Manifest
import android.app.ProgressDialog
import android.app.ProgressDialog.show
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.pm.PackageManager
import android.os.*
import android.util.Log
import android.view.LayoutInflater
import android.view.MotionEvent
import android.view.View
import android.widget.*
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import com.example.robotcontrol.databinding.ActivityMainBinding
import com.google.android.material.slider.Slider


import java.io.IOException
import java.lang.Double.parseDouble
import java.util.*
import kotlin.concurrent.schedule

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

    private lateinit var binding : ActivityMainBinding

    private var cmdIndex = 0
    private var cmdTime: Long = 0
    private var velocityLeft = 0.0F
    private var velocityRight = 0.0F
    private var extraDelay : Long = 0
    private var xPosition : Int = 0
    private var yPosition : Int = 0

    private var mode = 0

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(LayoutInflater.from(this))
        setContentView(binding.root)

        //GET BLUETOOTH CLIENT FROM CONFIGURATION
        clientAddress = intent.getStringExtra(ConfigurationActivity.choosenAddress).toString()
        clientName = intent.getStringExtra(ConfigurationActivity.choosenName).toString()

        //INITIAL TEXT "MODE BUTTON"
        binding.modeButton.text = getString(R.string.mode, "v,ω")

        //BEGIN CONNECTION
        ConnectThread(this).execute()

        var textToSend: String
        binding.imageDown.setOnTouchListener { v: View, m: MotionEvent ->
            textToSend = "setA" + (-velocityLeft).toString() + "setB" + (-velocityLeft).toString()
            v.performClick()
            onTouch(m, textToSend, sendStop = true, binding.imageDown)
            true
        }
        binding.imageUp.setOnTouchListener { v: View, m: MotionEvent ->
            textToSend = "setA" + velocityLeft.toString() + "setB" + velocityLeft.toString()
            v.performClick()
            onTouch(m, textToSend, sendStop = true, binding.imageUp)
            true
        }
        binding.imageLeft.setOnTouchListener { v: View, m: MotionEvent ->
            textToSend = "setA" + velocityLeft.toString() + "setB" + (-velocityLeft).toString()
            v.performClick()
            onTouch(m, textToSend, sendStop = true, binding.imageLeft)
            true
        }
        binding.imageRight.setOnTouchListener { v: View, m: MotionEvent ->
            textToSend = "setA" + (-velocityLeft).toString() + "setB" + velocityLeft.toString()
            v.performClick()
            onTouch(m, textToSend, sendStop = true, binding.imageRight)
            true
        }
        binding.imageCamera.setOnTouchListener { v: View, m: MotionEvent ->
            v.performClick()
            onTouch(m, "camera", sendStop = false, binding.imageCamera)
            true
        }

        binding.modeButton.setOnClickListener()
        {
            mode += 1
            mode %= 3
            changeVisibility(mode)

        }

        binding.sliderLeft.addOnSliderTouchListener(object : Slider.OnSliderTouchListener {
            override fun onStartTrackingTouch(slider: Slider) {
            }

            override fun onStopTrackingTouch(slider: Slider) {
                sendFromSlider()
            }
        })
        binding.sliderLeft.addOnChangeListener { _, _, _ ->
            velocityLeft = binding.sliderLeft.value
            binding.sliderLeftValue.text = velocityLeft.toString()
        }

        binding.sliderRight.addOnSliderTouchListener(object : Slider.OnSliderTouchListener {
            override fun onStartTrackingTouch(slider: Slider) {
                //
            }

            override fun onStopTrackingTouch(slider: Slider) {
                    sendFromSlider()
            }
        })
        binding.sliderRight.addOnChangeListener { _, _, _ ->
            velocityRight = binding.sliderRight.value
            binding.sliderRightValue.text = velocityRight.toString()
        }

        binding.sliderBoth.addOnSliderTouchListener(object : Slider.OnSliderTouchListener {
            override fun onStartTrackingTouch(slider: Slider) {
            }

            override fun onStopTrackingTouch(slider: Slider) {
                    sendFromSlider()
            }
        })
        binding.sliderBoth.addOnChangeListener { _, _, _ ->

            velocityLeft = binding.sliderBoth.value
            velocityRight = binding.sliderBoth.value

            binding.sliderLeft.value = velocityLeft
            binding.sliderRight.value = velocityRight
            binding.sliderBothValue.text = binding.sliderBoth.value.toString()
            binding.sliderLeftValue.text = binding.sliderLeft.value.toString()
            binding.sliderRightValue.text = binding.sliderRight.value.toString()
        }

        readAndcheckTimer()

    }

    private fun sendFromSlider() {
        if(mode == 1)
        {
            val text = "setA" + velocityRight.toString() + "setB" + velocityLeft.toString()
            val size = text.toByteArray().size.toString() + "B"
            sendCommand("$text;$cmdIndex;$size;vel")
            cmdTime = System.currentTimeMillis()
            cmdIndex += 1
            cmdIndex %= 9
        }
    }


    @RequiresApi(Build.VERSION_CODES.O)
    private fun onTouch(
        event: MotionEvent?,
        command: String,
        sendStop: Boolean,
        image: ImageView
    ): Boolean {
        val text: String
        val size: String


        when (event?.action) {
            MotionEvent.ACTION_DOWN -> {
                image.setColorFilter(ContextCompat.getColor(this, R.color.orange))
                text = command
                size = text.toByteArray().size.toString() + "B"
                sendCommand("$text;$cmdIndex;$size;vel")
                cmdTime = System.currentTimeMillis()
                cmdIndex += 1
                cmdIndex %= 9
            }
            MotionEvent.ACTION_UP -> {
                image.setColorFilter(ContextCompat.getColor(this, R.color.blue))
                if (sendStop) {
                    text = "setA0.0setB0.0"
                    size = text.toByteArray().size.toString() + "B"
                    sendCommand("$text;$cmdIndex;$size;vel")
                    cmdTime = System.currentTimeMillis()
                    cmdIndex += 1
                    cmdIndex %= 9
                }
            }
        }

        Log.i("info", event.toString())
        return true
    }

    fun sendCommand(cmd: String) {
        var error = false
        if (bluetoothSocket != null) {

            val offDelay : Long
            if(binding.checkBoxRandom.isChecked)
            {
                offDelay = (0..extraDelay).random()
                binding.delayTextInput.setText(offDelay.toString())
            }
            else
            {
                offDelay = extraDelay
            }
            Timer().schedule(offDelay) {
                try {
                    bluetoothSocket!!.outputStream.write(cmd.toByteArray())
                } catch (e: IOException) {
                    e.printStackTrace()
                    this@MainActivity.binding.info.text = getString(R.string.infoText, clientName, "failed")
                    error = true
                    changeVisibility(0)
                }
            }
            if(error)
            {
                binding.info.setTextColor(ContextCompat.getColor(this, R.color.red))
            }


        }
    }

    @RequiresApi(Build.VERSION_CODES.O)
    fun readCommand() {
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
                        binding.textView.text = "Collision !!!"
                        binding.textView.setTextColor(ContextCompat.getColor(this, R.color.red))
                    } else if ("ok" in text) {
                        binding.textView.text = "OK"
                        binding.textView.setTextColor(ContextCompat.getColor(this, R.color.green))
                    }
                    if (text.contains("[0-9]".toRegex())) {

                        for (i in 0..9) {
                            if (i.toString() in text) {
                                val t = System.currentTimeMillis() - cmdTime
                                binding.delayTextView.text = getString(R.string.delayTime, t.toString())
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

    fun disconnect(@Suppress("UNUSED_PARAMETER")view: View) {
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

    fun stop(@Suppress("UNUSED_PARAMETER")view: View) {

        velocityLeft = 0.0F
        velocityRight = 0.0F
        binding.sliderRight.value = 0.0F
        binding.sliderLeft.value = 0.0F
        binding.sliderBoth.value = 0.0F

        sendFromSlider()

    }

    fun changeVisibility(v: Int) {
        when (v) {
            //VELOCITY CONTROL MODE
            0 -> {
                binding.imageDown.visibility = View.VISIBLE
                binding.imageUp.visibility = View.VISIBLE
                binding.imageLeft.visibility = View.VISIBLE
                binding.imageRight.visibility = View.VISIBLE
                binding.sliderLeft.visibility = View.INVISIBLE
                binding.sliderRight.visibility = View.INVISIBLE
                binding.sliderBoth.visibility = View.VISIBLE
                binding.sliderBothValue.visibility = View.VISIBLE
                binding.sliderLeftValue.visibility = View.INVISIBLE
                binding.sliderRightValue.visibility = View.INVISIBLE
                binding.stopButton.visibility = View.INVISIBLE
                binding.xInput.visibility = View.INVISIBLE
                binding.buttonOKxy.visibility = View.INVISIBLE
                binding.yInput.visibility = View.INVISIBLE
                binding.modeButton.text = getString(R.string.mode, "v,ω")
            }
            //WHEELS VELOCITY CONTROL MODE
            1 -> {
                binding.imageDown.visibility = View.INVISIBLE
                binding.imageUp.visibility = View.INVISIBLE
                binding.imageLeft.visibility = View.INVISIBLE
                binding.imageRight.visibility = View.INVISIBLE
                binding.sliderLeft.visibility = View.VISIBLE
                binding.sliderRight.visibility = View.VISIBLE
                binding.sliderBoth.visibility = View.VISIBLE
                binding.sliderBothValue.visibility = View.INVISIBLE
                binding.sliderLeftValue.visibility = View.VISIBLE
                binding.sliderRightValue.visibility = View.VISIBLE
                binding.stopButton.visibility = View.VISIBLE
                binding.modeButton.text = getString(R.string.mode, "ωL, ωP")
            }
            //POSITION CONTROL MODE
            2 -> {
                binding.sliderLeft.visibility = View.INVISIBLE
                binding.sliderRight.visibility = View.INVISIBLE
                binding.sliderBoth.visibility = View.INVISIBLE
                binding.sliderBothValue.visibility = View.INVISIBLE
                binding.sliderLeftValue.visibility = View.INVISIBLE
                binding.sliderRightValue.visibility = View.INVISIBLE
                binding.stopButton.visibility = View.INVISIBLE
                binding.xInput.visibility = View.VISIBLE
                binding.xInput.visibility = View.VISIBLE
                binding.buttonOKxy.visibility = View.VISIBLE
                binding.yInput.visibility = View.VISIBLE
                binding.modeButton.text = getString(R.string.mode, "x,y")
            }
            //ERROR
            4 -> {
                binding.imageDown.visibility = View.INVISIBLE
                binding.imageUp.visibility = View.INVISIBLE
                binding.imageLeft.visibility = View.INVISIBLE
                binding.imageRight.visibility = View.INVISIBLE
                binding.sliderLeft.visibility = View.INVISIBLE
                binding.sliderRight.visibility = View.INVISIBLE
                binding.sliderBoth.visibility = View.INVISIBLE
                binding.sliderBothValue.visibility = View.INVISIBLE
                binding.sliderLeftValue.visibility = View.INVISIBLE
                binding.sliderRightValue.visibility = View.INVISIBLE
            }

        }

    }

    fun ok(@Suppress("UNUSED_PARAMETER")view: View) {
        val extraDelayString = binding.delayTextInput.text.toString()
        var numeric = true

        try {
            parseDouble(extraDelayString)
        } catch (e: NumberFormatException) {
            numeric = false
        }

        if (numeric)
        {
            extraDelay = extraDelayString.toLong()
        }
        binding.delayTextInput.isFocusable = false
        binding.delayTextInput.isFocusableInTouchMode = true
        binding.delayTextInput.setText(extraDelay.toString())

    }

    fun okxy(@Suppress("UNUSED_PARAMETER")view: View) {
        val xString = binding.xInput.text.toString()
        val yString = binding.yInput.text.toString()
        val xmin = -550
        val xmax = 550
        val ymin = -350
        val ymax = 350

        var numeric = true
        try {
            parseDouble(xString)
            parseDouble(yString)
        } catch (e: NumberFormatException) {
            numeric = false
        }

        if (numeric && xString.toInt() in xmin..xmax && yString.toInt() in ymin..ymax)
        {
            xPosition = xString.toInt()
            yPosition = yString.toInt()

            val text = "setA" + xPosition.toString() + "setB" + yPosition.toString()
            val size = text.toByteArray().size.toString() + "B"
            sendCommand("$text;$cmdIndex;$size;pos")
            cmdTime = System.currentTimeMillis()
            cmdIndex += 1
            cmdIndex %= 9
        }
        else
            Toast.makeText(this, "Podaj x w przedziale "+xmin.toString()+"do "+xmax.toString()+
                    " oraz y w przedziale "+ymin.toString()+" do "+ymax.toString(), Toast.LENGTH_LONG).show()

        binding.xInput.isFocusable = false
        binding.xInput.isFocusableInTouchMode = true
        binding.xInput.setText(xPosition.toString())
        binding.yInput.isFocusable = false
        binding.yInput.isFocusableInTouchMode = true
        binding.yInput.setText(yPosition.toString())
    }

    private fun readAndcheckTimer() {
        object : CountDownTimer(1000, 1) {
            @RequiresApi(Build.VERSION_CODES.O)
            override fun onTick(millisUntilFinished: Long) {
                readCommand()
            }

            override fun onFinish() {
                sendCommand("check")
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
            progressDialog = show(context, "Connecting in progress", "Wait a second")
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
                    bluetoothSocket!!.connect()

                    // Cancel discovery because it otherwise slows down the connection.
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
                }

            } catch (e: IOException) {
                connectSuccess = false
                e.printStackTrace()
            }

            return null
        }

        override fun onPostExecute(result: String?) {
            if (!connectSuccess) {
                Log.i("info", "Connection failed")
                this@MainActivity.binding.info.text = getString(R.string.infoText, clientName, "failed")
                this@MainActivity.binding.info.setTextColor(ContextCompat.getColor(this@MainActivity, R.color.red))
                this@MainActivity.changeVisibility(4)

            } else {
                Log.i("info", "Connection OK")
                isConnected = true
                this@MainActivity.binding.info.text = getString(R.string.infoText, clientName, "OK")
                this@MainActivity.binding.info.setTextColor(ContextCompat.getColor(this@MainActivity, R.color.green))
            }
            //TURN OFF PROGRESS DIALOG
            progressDialog.dismiss()
        }
    }
}