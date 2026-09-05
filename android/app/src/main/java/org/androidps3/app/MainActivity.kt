package org.androidps3.app

import android.os.Bundle
import android.view.View
import android.view.animation.AlphaAnimation
import androidx.appcompat.app.AppCompatActivity
import org.androidps3.app.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        window.decorView.systemUiVisibility = View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY or
            View.SYSTEM_UI_FLAG_FULLSCREEN or
            View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or
            View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or
            View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION or
            View.SYSTEM_UI_FLAG_LAYOUT_STABLE

        initializeEmulator()
        bindNavigation()
        bindHomeActions()
        animateHome()
    }

    private fun initializeEmulator() {
        val initialized = NativeBridge.nativeInitialize()
        binding.versionText.text = NativeBridge.nativeGetVersionString()
        binding.statusText.text = if (initialized) {
            NativeBridge.nativeRunFrame()
            NativeBridge.nativeGetStatusString()
        } else {
            "Failed to initialize emulator core"
        }
    }

    private fun bindNavigation() {
        val navigation = listOf(
            binding.navHome to "Home",
            binding.navGames to "Games",
            binding.navLibrary to "Library",
            binding.navDownloads to "Downloads",
            binding.navSettings to "Settings",
            binding.navAbout to "About"
        )

        navigation.forEach { (view, label) ->
            view.setOnClickListener {
                selectNavigation(view)
                binding.titleText.text = label
                binding.statusText.text = when (label) {
                    "Home" -> "Play your favorite PS3 games on Android."
                    else -> "$label interface is ready for Milestone 3 implementation."
                }
                animateContent()
            }
        }
    }

    private fun selectNavigation(selected: View) {
        val all = listOf(binding.navHome, binding.navGames, binding.navLibrary,
            binding.navDownloads, binding.navSettings, binding.navAbout)
        all.forEach { view ->
            view.setBackgroundColor(0x00000000)
            view.alpha = 0.82f
        }
        selected.setBackgroundColor(0xFF102B44.toInt())
        selected.alpha = 1.0f
    }

    private fun bindHomeActions() {
        binding.browseGamesButton.setOnClickListener {
            selectNavigation(binding.navGames)
            binding.titleText.text = "Games"
            binding.statusText.text = "Browse, manage and play your PS3 games."
            animateContent()
        }
        binding.addGameButton.setOnClickListener {
            binding.statusText.text = "Game picker will be connected in the game library milestone."
            pulse(binding.addGameButton)
        }
        binding.scanButton.setOnClickListener {
            binding.statusText.text = "Library scan requested. Game scanning backend is not implemented yet."
            pulse(binding.scanButton)
        }
        binding.configureButton.setOnClickListener {
            selectNavigation(binding.navSettings)
            binding.titleText.text = "Settings"
            binding.statusText.text = "Configure AetherPS3 for your device."
            animateContent()
        }
        binding.resumeButton.setOnClickListener {
            binding.statusText.text = "No active game session is available yet."
            pulse(binding.resumeButton)
        }
    }

    private fun animateHome() {
        val animation = AlphaAnimation(0f, 1f).apply {
            duration = 260
            fillAfter = true
        }
        binding.homeHero.startAnimation(animation)
        animateContent()
    }

    private fun animateContent() {
        val animation = AlphaAnimation(0.65f, 1f).apply {
            duration = 180
            fillAfter = true
        }
        binding.contentContainer.startAnimation(animation)
    }

    private fun pulse(view: View) {
        view.animate().scaleX(0.97f).scaleY(0.97f).setDuration(70).withEndAction {
            view.animate().scaleX(1f).scaleY(1f).setDuration(100).start()
        }.start()
    }

    override fun onDestroy() {
        NativeBridge.nativeShutdown()
        super.onDestroy()
    }
}
