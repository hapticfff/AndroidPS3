package org.androidps3.app

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import org.androidps3.app.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val initialized = NativeBridge.nativeInitialize()
        binding.versionText.text = NativeBridge.nativeGetVersionString()
        binding.statusText.text = if (initialized) {
            NativeBridge.nativeRunFrame()
            NativeBridge.nativeGetStatusString()
        } else {
            "Failed to initialize emulator core"
        }
    }

    override fun onDestroy() {
        NativeBridge.nativeShutdown()
        super.onDestroy()
    }
}
