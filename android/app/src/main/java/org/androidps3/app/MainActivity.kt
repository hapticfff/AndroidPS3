package org.androidps3.app

import android.graphics.Color
import android.os.Bundle
import android.view.View
import android.view.animation.AccelerateDecelerateInterpolator
import android.widget.LinearLayout
import android.widget.Switch
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import org.androidps3.app.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    private val blue = Color.rgb(8, 127, 241)
    private val panel = Color.rgb(10, 27, 45)
    private val text = Color.WHITE
    private val muted = Color.rgb(132, 155, 175)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        hideSystemUi()
        initializeEmulator()
        setupNavigation()
        setupHomeActions()
        setupSettingsCategories()
        showHome()
    }

    private fun hideSystemUi() {
        window.decorView.systemUiVisibility =
            View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY or View.SYSTEM_UI_FLAG_FULLSCREEN or
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or
                View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
    }

    private fun initializeEmulator() {
        val initialized = NativeBridge.nativeInitialize()
        binding.versionText.text = NativeBridge.nativeGetVersionString()
        binding.statusText.text = if (initialized) {
            NativeBridge.nativeRunFrame()
            "Play your favorite PS3 games on Android."
        } else "Failed to initialize emulator core"
    }

    private fun setupNavigation() {
        binding.navHome.setOnClickListener { selectNavigation(binding.navHome); showHome() }
        binding.navGames.setOnClickListener { selectNavigation(binding.navGames); showSection("Games", "Your PS3 game library", "games") }
        binding.navLibrary.setOnClickListener { selectNavigation(binding.navLibrary); showSection("Library", "Manage local games, favorites and play history", "library") }
        binding.navDownloads.setOnClickListener { selectNavigation(binding.navDownloads); showSection("Downloads", "Manage emulator and library downloads", "downloads") }
        binding.navSettings.setOnClickListener { selectNavigation(binding.navSettings); showSettings() }
        binding.navAbout.setOnClickListener { selectNavigation(binding.navAbout); showSection("About AetherPS3", "Play Beyond. On Any Device.", "about") }
    }

    private fun setupHomeActions() {
        binding.browseGamesButton.setOnClickListener { binding.navGames.performClick() }
        binding.configureButton.setOnClickListener { binding.navSettings.performClick() }
        binding.addGameButton.setOnClickListener { showToastState("Game picker will connect to the Android file picker in the library milestone.") }
        binding.scanButton.setOnClickListener { showToastState("Library scanner foundation is reserved for the game-management milestone.") }
        binding.resumeButton.setOnClickListener { showToastState("No active game session yet.") }
    }

    private fun showHome() {
        binding.titleText.text = "Welcome Back!"
        binding.statusText.text = "Play your favorite PS3 games on Android."
        binding.homeHero.isVisible = true
        binding.homeActions.isVisible = true
        binding.homePanel.isVisible = true
        binding.sectionPanel.isVisible = false
        binding.readyText.isVisible = true
        animateContent()
    }

    private fun showSection(title: String, description: String, type: String) {
        binding.homeHero.isVisible = false
        binding.homeActions.isVisible = false
        binding.homePanel.isVisible = false
        binding.sectionPanel.isVisible = true
        binding.settingsLayout.isVisible = false
        binding.gamesLayout.isVisible = type == "games"
        binding.infoContent.isVisible = type != "games"
        binding.titleText.text = title
        binding.statusText.text = description
        binding.sectionHeading.text = title
        binding.sectionDescription.text = description
        binding.infoContent.text = when (type) {
            "library" -> "LIBRARY\n\nYour local PS3 game collection will appear here after you add legally obtained game data. Favorites, play history, metadata and per-game settings will live in this section.\n\nNo copyrighted game files are bundled with AetherPS3."
            "downloads" -> "DOWNLOADS\n\nDownload queue and task history will be connected here. The frontend is ready for progress cards, pause/resume controls and storage information."
            else -> "AETHERPS3\n\nAetherPS3 is an open-source Android PS3 emulator project focused on a polished frontend, correctness-first emulation and support for a wide range of Android hardware.\n\nInstagram: AetherPS3\nGitHub: Hapticfff\n\nThis project does not distribute Sony firmware, encryption keys or copyrighted game data."
        }
        animateContent()
    }

    private fun showSettings() {
        binding.homeHero.isVisible = false
        binding.homeActions.isVisible = false
        binding.homePanel.isVisible = false
        binding.sectionPanel.isVisible = true
        binding.settingsLayout.isVisible = true
        binding.gamesLayout.isVisible = false
        binding.infoContent.isVisible = false
        binding.titleText.text = "Settings"
        binding.statusText.text = "Fine-tune AetherPS3 for your hardware and games."
        binding.sectionHeading.text = "Settings"
        binding.sectionDescription.text = "Every emulator option is organized by purpose."
        renderSettings("General")
        animateContent()
    }

    private fun setupSettingsCategories() {
        val categories = listOf(
            binding.settingsGeneral to "General", binding.settingsSystem to "System",
            binding.settingsGraphics to "Graphics", binding.settingsAudio to "Audio",
            binding.settingsController to "Controller", binding.settingsEmulation to "Emulation",
            binding.settingsAdvanced to "Advanced"
        )
        categories.forEach { (view, name) -> view.setOnClickListener { renderSettings(name) } }
    }

    private fun renderSettings(category: String) {
        val all = listOf(binding.settingsGeneral, binding.settingsSystem, binding.settingsGraphics, binding.settingsAudio, binding.settingsController, binding.settingsEmulation, binding.settingsAdvanced)
        all.forEach { it.setBackgroundColor(Color.TRANSPARENT) }
        all.firstOrNull { it.text.toString() == category }?.setBackgroundColor(Color.rgb(16, 43, 68))
        val box = binding.settingsContent
        box.removeAllViews()
        addHeading(box, category)
        when (category) {
            "General" -> {
                addSwitch(box, "Fullscreen UI", "Use immersive landscape presentation", true)
                addSwitch(box, "Show performance overlay", "Display FPS and frame timing while playing", false)
                addChoice(box, "Language", "System default")
                addChoice(box, "Theme", "Aether Dark")
            }
            "System" -> {
                addChoice(box, "PS3 firmware", "User-provided system software")
                addChoice(box, "Console region", "Auto")
                addSwitch(box, "Enable system logging", "Keep detailed emulator diagnostics", true)
                addChoice(box, "Storage location", "Internal storage")
            }
            "Graphics" -> {
                addChoice(box, "Renderer", "Vulkan")
                addChoice(box, "Resolution scale", "100%")
                addChoice(box, "VSync", "Auto")
                addSwitch(box, "Shader cache", "Reuse compiled shaders to reduce stutter", true)
                addSwitch(box, "Async shader compilation", "Compile compatible shaders without blocking", true)
            }
            "Audio" -> {
                addChoice(box, "Audio backend", "Android Audio")
                addChoice(box, "Output", "System default")
                addChoice(box, "Latency", "Balanced")
                addSwitch(box, "Audio stretching", "Keep audio stable when emulation speed varies", true)
            }
            "Controller" -> {
                addChoice(box, "Port 1", "Virtual Controller")
                addChoice(box, "Port 2", "Unassigned")
                addSwitch(box, "Haptic feedback", "Use supported Android vibration", true)
                addSwitch(box, "Show touch controls", "Display the on-screen PS3 controller", true)
            }
            "Emulation" -> {
                addChoice(box, "CPU mode", "Interpreter (current foundation)")
                addChoice(box, "SPU mode", "Auto")
                addSwitch(box, "Accurate timing", "Prefer deterministic timing over speed", true)
                addSwitch(box, "JIT", "Future accelerator; not active in this milestone", false)
            }
            "Advanced" -> {
                addSwitch(box, "Developer logging", "Expose detailed core diagnostics", false)
                addSwitch(box, "Debug overlays", "Show internal emulator state", false)
                addSwitch(box, "Experimental features", "Allow unfinished options to be exposed", false)
                addChoice(box, "Log level", "Info")
            }
        }
    }

    private fun addHeading(parent: LinearLayout, value: String) {
        val v = TextView(this)
        v.text = "$value Settings"
        v.setTextColor(text)
        v.textSize = 16f
        v.setPadding(4, 4, 4, 12)
        parent.addView(v)
    }

    private fun addSwitch(parent: LinearLayout, title: String, subtitle: String, enabled: Boolean) {
        val row = LinearLayout(this)
        row.orientation = LinearLayout.HORIZONTAL
        row.gravity = android.view.Gravity.CENTER_VERTICAL
        row.setPadding(16, 12, 12, 12)
        row.setBackgroundColor(panel)
        val labels = LinearLayout(this)
        labels.orientation = LinearLayout.VERTICAL
        labels.layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f)
        val t = TextView(this); t.text = title; t.setTextColor(text); t.textSize = 11f
        val s = TextView(this); s.text = subtitle; s.setTextColor(muted); s.textSize = 8f; s.setPadding(0, 3, 0, 0)
        labels.addView(t); labels.addView(s)
        val sw = Switch(this); sw.isChecked = enabled; sw.buttonTintList = android.content.res.ColorStateList.valueOf(blue)
        row.addView(labels); row.addView(sw)
        val lp = LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 64); lp.setMargins(0, 0, 0, 7)
        parent.addView(row, lp)
    }

    private fun addChoice(parent: LinearLayout, title: String, value: String) {
        val row = LinearLayout(this); row.orientation = LinearLayout.HORIZONTAL; row.gravity = android.view.Gravity.CENTER_VERTICAL; row.setPadding(16, 10, 16, 10); row.setBackgroundColor(panel)
        val labels = LinearLayout(this); labels.orientation = LinearLayout.VERTICAL; labels.layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f)
        val t = TextView(this); t.text = title; t.setTextColor(text); t.textSize = 11f
        val s = TextView(this); s.text = value; s.setTextColor(muted); s.textSize = 8f; s.setPadding(0, 3, 0, 0)
        labels.addView(t); labels.addView(s)
        val arrow = TextView(this); arrow.text = "›"; arrow.setTextColor(blue); arrow.textSize = 22f
        row.addView(labels); row.addView(arrow)
        row.setOnClickListener { s.text = if (s.text == value) "Default / Auto" else value }
        val lp = LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 64); lp.setMargins(0, 0, 0, 7)
        parent.addView(row, lp)
    }

    private fun selectNavigation(selected: TextView) {
        val all = listOf(binding.navHome, binding.navGames, binding.navLibrary, binding.navDownloads, binding.navSettings, binding.navAbout)
        all.forEach { it.setBackgroundColor(Color.TRANSPARENT); it.alpha = 0.82f }
        selected.setBackgroundColor(Color.rgb(16, 43, 68)); selected.alpha = 1f
    }

    private fun animateContent() {
        binding.contentContainer.alpha = 0.65f
        binding.contentContainer.translationX = 5f
        binding.contentContainer.animate().alpha(1f).translationX(0f).setDuration(180).setInterpolator(AccelerateDecelerateInterpolator()).start()
    }

    private fun showToastState(message: String) {
        binding.statusText.text = message
    }

    override fun onDestroy() {
        NativeBridge.nativeShutdown()
        super.onDestroy()
    }
}
