package org.androidps3.app

import android.os.Bundle
import android.view.View
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
            "Core initialization failed"
        }

        binding.navHome.setOnClickListener { showPage("HOME") }
        binding.navGames.setOnClickListener { showPage("GAMES") }
        binding.navDownloads.setOnClickListener { showPage("DOWNLOADS") }
        binding.navSettings.setOnClickListener { showPage("SETTINGS") }
        binding.navAbout.setOnClickListener { showPage("ABOUT") }
        binding.addGameButton.setOnClickListener { showPage("GAMES") }
        showPage("HOME")
    }

    private fun showPage(page: String) {
        binding.homePanel.visibility = if (page == "SETTINGS") View.GONE else View.VISIBLE
        binding.settingsPanel.visibility = if (page == "SETTINGS") View.VISIBLE else View.GONE
        when (page) {
            "HOME" -> {
                binding.pageTitle.text = "Welcome back"
                binding.pageSubtitle.text = "Your PlayStation 3 library, settings and emulator controls in one place."
                binding.heroTitle.text = "No games added yet"
                binding.heroBody.text = "Add a legally obtained PS3 game folder or disc dump to begin building your library."
            }
            "GAMES" -> {
                binding.pageTitle.text = "Game Library"
                binding.pageSubtitle.text = "Your installed and discovered PS3 titles will appear here."
                binding.heroTitle.text = "Library is ready"
                binding.heroBody.text = "Game scanning and title metadata are planned for the next frontend stage."
            }
            "DOWNLOADS" -> {
                binding.pageTitle.text = "Downloads"
                binding.pageSubtitle.text = "Track emulator resources and user-managed files."
                binding.heroTitle.text = "Nothing downloading"
                binding.heroBody.text = "AetherPS3 will keep downloads separate from copyrighted game and firmware content."
            }
            "SETTINGS" -> {
                binding.pageTitle.text = "Settings"
                binding.pageSubtitle.text = "Configure graphics, controls, audio, system paths and emulator behavior."
            }
            "ABOUT" -> {
                binding.pageTitle.text = "About AetherPS3"
                binding.pageSubtitle.text = "An independent open-source Android PS3 emulator project."
                binding.heroTitle.text = "Built for Android"
                binding.heroBody.text = "ARM64-first architecture with a correctness-first native core. Users provide their own legally obtained system software and games."
            }
        }
        updateNav(page)
    }

    private fun updateNav(selected: String) {
        val active = getColor(org.androidps3.app.R.color.aether_text)
        val muted = getColor(org.androidps3.app.R.color.aether_muted)
        binding.navHome.setTextColor(if (selected == "HOME") active else muted)
        binding.navGames.setTextColor(if (selected == "GAMES") active else muted)
        binding.navDownloads.setTextColor(if (selected == "DOWNLOADS") active else muted)
        binding.navSettings.setTextColor(if (selected == "SETTINGS") active else muted)
        binding.navAbout.setTextColor(if (selected == "ABOUT") active else muted)
    }

    override fun onDestroy() {
        NativeBridge.nativeShutdown()
        super.onDestroy()
    }
}
