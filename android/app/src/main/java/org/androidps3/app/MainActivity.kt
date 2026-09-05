package org.androidps3.app

import android.graphics.Color
import android.os.Bundle
import android.view.View
import android.view.animation.AccelerateDecelerateInterpolator
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import org.androidps3.app.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    private data class NavItem(val view: TextView, val title: String, val subtitle: String)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        hideSystemUi()
        initializeEmulator()
        setupNavigation()
        setupHomeActions()
        showHome()
    }

    private fun hideSystemUi() {
        window.decorView.systemUiVisibility =
            View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY or
                View.SYSTEM_UI_FLAG_FULLSCREEN or
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or
                View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or
                View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION or
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
    }

    private fun initializeEmulator() {
        val initialized = NativeBridge.nativeInitialize()
        binding.versionText.text = NativeBridge.nativeGetVersionString()
        binding.statusText.text = if (initialized) {
            NativeBridge.nativeRunFrame()
            "Play your favorite PS3 games on Android."
        } else {
            "Failed to initialize emulator core"
        }
    }

    private fun setupNavigation() {
        val items = listOf(
            NavItem(binding.navHome, "Home", "Play your favorite PS3 games on Android."),
            NavItem(binding.navGames, "Games", "Browse, manage and play your PS3 games."),
            NavItem(binding.navLibrary, "Library", "Your games, progress and local data."),
            NavItem(binding.navDownloads, "Downloads", "Manage your local download queue."),
            NavItem(binding.navSettings, "Settings", "Fine-tune AetherPS3 for your hardware and games."),
            NavItem(binding.navAbout, "About", "Play Beyond. On Any Device.")
        )

        items.forEach { item ->
            item.view.setOnClickListener {
                selectNavigation(item.view)
                binding.titleText.text = item.title
                binding.statusText.text = item.subtitle
                if (item.title == "Home") {
                    showHome()
                } else {
                    showPlaceholder(item.title)
                }
                animateContent()
            }
        }
    }

    private fun setupHomeActions() {
        binding.browseGamesButton.setOnClickListener {
            binding.navGames.performClick()
        }
        binding.addGameButton.setOnClickListener {
            binding.statusText.text = "Game picker is coming with the library integration."
            pulse(binding.addGameButton)
        }
        binding.scanButton.setOnClickListener {
            binding.statusText.text = "Library scan requested. The scanner backend is not part of this milestone."
            pulse(binding.scanButton)
        }
        binding.configureButton.setOnClickListener {
            binding.navSettings.performClick()
        }
        binding.resumeButton.setOnClickListener {
            binding.statusText.text = "No active game session is available yet."
            pulse(binding.resumeButton)
        }
    }

    private fun showHome() {
        binding.homeHero.isVisible = true
        binding.browseGamesButton.isVisible = true
        binding.addGameButton.isVisible = true
        binding.scanButton.isVisible = true
        binding.configureButton.isVisible = true
        binding.resumeButton.isVisible = true
        binding.recentGamesRow.isVisible = true
        binding.statusText.text = "Play your favorite PS3 games on Android."
    }

    private fun showPlaceholder(title: String) {
        binding.homeHero.isVisible = false
        binding.browseGamesButton.isVisible = false
        binding.addGameButton.isVisible = false
        binding.scanButton.isVisible = false
        binding.configureButton.isVisible = false
        binding.resumeButton.isVisible = false
        binding.recentGamesRow.isVisible = false
        binding.statusText.text = "$title UI foundation is connected and ready for the next implementation pass."
    }

    private fun selectNavigation(selected: TextView) {
        val all = listOf(
            binding.navHome,
            binding.navGames,
            binding.navLibrary,
            binding.navDownloads,
            binding.navSettings,
            binding.navAbout
        )
        all.forEach { view ->
            view.setBackgroundColor(Color.TRANSPARENT)
            view.alpha = 0.82f
        }
        selected.setBackgroundColor(Color.rgb(16, 43, 68))
        selected.alpha = 1f
    }

    private fun animateContent() {
        binding.contentContainer.alpha = 0.65f
        binding.contentContainer.translationX = 5f
        binding.contentContainer.animate()
            .alpha(1f)
            .translationX(0f)
            .setDuration(180)
            .setInterpolator(AccelerateDecelerateInterpolator())
            .start()
    }

    private fun pulse(view: View) {
        view.animate()
            .scaleX(0.97f)
            .scaleY(0.97f)
            .setDuration(65)
            .withEndAction {
                view.animate().scaleX(1f).scaleY(1f).setDuration(100).start()
            }
            .start()
    }

    override fun onDestroy() {
        NativeBridge.nativeShutdown()
        super.onDestroy()
    }
}
