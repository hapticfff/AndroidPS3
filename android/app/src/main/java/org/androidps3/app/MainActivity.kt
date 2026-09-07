package org.androidps3.app

import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.TextView
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import org.androidps3.app.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var library: GameLibrary
    private val folderPicker = registerForActivityResult(ActivityResultContracts.OpenDocumentTree()) { uri ->
        if (uri != null) {
            runCatching { contentResolver.takePersistableUriPermission(uri, Intent.FLAG_GRANT_READ_URI_PERMISSION) }
            library.addSource(uri); refreshLibrary(); showPage("GAMES")
        }
    }
    private val filePicker = registerForActivityResult(ActivityResultContracts.OpenDocument()) { uri ->
        if (uri != null) {
            runCatching { contentResolver.takePersistableUriPermission(uri, Intent.FLAG_GRANT_READ_URI_PERMISSION) }
            library.addSource(uri); refreshLibrary(); showPage("GAMES")
        }
    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState); binding = ActivityMainBinding.inflate(layoutInflater); setContentView(binding.root)
        library = GameLibrary(this)
        val initialized = NativeBridge.nativeInitialize()
        binding.versionText.text = NativeBridge.nativeGetVersionString()
        binding.statusText.text = if (initialized) { NativeBridge.nativeRunFrame(); NativeBridge.nativeGetStatusString() } else "Core initialization failed"
        binding.navHome.setOnClickListener { showPage("HOME") }; binding.navGames.setOnClickListener { refreshLibrary(); showPage("GAMES") }
        binding.navDownloads.setOnClickListener { showPage("DOWNLOADS") }; binding.navSettings.setOnClickListener { showPage("SETTINGS") }; binding.navAbout.setOnClickListener { showPage("ABOUT") }
        binding.addGameButton.setOnClickListener { chooseGameFile() }; binding.addGameButtonGames.setOnClickListener { chooseGameFile() }
        binding.addFolderButton.setOnClickListener { folderPicker.launch(null) }; binding.addFolderButtonGames.setOnClickListener { folderPicker.launch(null) }
        refreshLibrary(); showPage("HOME")
    }
    private fun chooseGameFile() { filePicker.launch(arrayOf("application/octet-stream", "application/zip", "application/x-iso9660-image", "*/*")) }
    private fun refreshLibrary() {
        val games = library.scan(); binding.libraryCount.text = "${games.size} GAME${if (games.size == 1) "" else "S"}"; binding.gamesList.removeAllViews()
        if (games.isEmpty()) { val empty = TextView(this); empty.text = "No PS3 games detected yet.\nUse IMPORT GAME for a file or ADD GAMES FOLDER for a PS3_GAME directory."; empty.setTextColor(0xFFA8B6C8.toInt()); empty.textSize = 15f; empty.setPadding(18,24,18,24); binding.gamesList.addView(empty); return }
        games.forEach { game -> val card = TextView(this); card.text = "${game.title}\n${game.type}\nTap for game details"; card.setTextColor(0xFFFFFFFF.toInt()); card.textSize = 15f; card.setPadding(20,20,20,20); card.setBackgroundColor(0xFF101B2A.toInt()); card.setOnClickListener { binding.statusText.text = "Selected: ${game.title} • ${game.type}" }; val lp = android.widget.LinearLayout.LayoutParams(-1,-2); lp.setMargins(0,0,0,10); binding.gamesList.addView(card,lp) }
    }
    private fun showPage(page: String) {
        binding.homePanel.visibility = if (page == "HOME") View.VISIBLE else View.GONE; binding.gamesPanel.visibility = if (page == "GAMES") View.VISIBLE else View.GONE; binding.downloadsPanel.visibility = if (page == "DOWNLOADS") View.VISIBLE else View.GONE; binding.settingsPanel.visibility = if (page == "SETTINGS") View.VISIBLE else View.GONE
        binding.pageTitle.text = when(page) { "GAMES"->"Game Library"; "DOWNLOADS"->"Downloads"; "SETTINGS"->"Settings"; "ABOUT"->"About AetherPS3"; else->"Welcome back" }
        binding.pageSubtitle.text = when(page) { "GAMES"->"Your discovered PS3 titles"; "DOWNLOADS"->"Transfers and installation tasks"; "SETTINGS"->"Configure AetherPS3"; "ABOUT"->"Independent Android PS3 emulator"; else->"Your PlayStation 3 library and emulator controls in one place." }
        val active=getColor(R.color.aether_text); val muted=getColor(R.color.aether_muted); binding.navHome.setTextColor(if(page=="HOME")active else muted); binding.navGames.setTextColor(if(page=="GAMES")active else muted); binding.navDownloads.setTextColor(if(page=="DOWNLOADS")active else muted); binding.navSettings.setTextColor(if(page=="SETTINGS")active else muted); binding.navAbout.setTextColor(if(page=="ABOUT")active else muted)
    }
    override fun onDestroy() { NativeBridge.nativeShutdown(); super.onDestroy() }
}
