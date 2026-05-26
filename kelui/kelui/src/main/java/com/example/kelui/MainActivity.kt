package com.example.kelui

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val tv = TextView(this)
        tv.text = stringFromJNI()
        setContentView(tv)
    }

    /**
     * A native method that is implemented by the 'nativelibcpp' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'nativelibcpp' library on application startup.
        init {
            System.loadLibrary("nativelibcpp")
        }
    }
}
