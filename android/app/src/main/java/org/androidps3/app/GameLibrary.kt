package org.androidps3.app

import android.content.Context
import android.net.Uri
import androidx.documentfile.provider.DocumentFile

data class GameEntry(val title: String, val path: String, val type: String)

class GameLibrary(private val context: Context) {
    private val prefs = context.getSharedPreferences("aether_library", Context.MODE_PRIVATE)
    private val key = "game_sources"

    fun addSource(uri: Uri) {
        val values = prefs.getStringSet(key, emptySet())!!.toMutableSet()
        values += uri.toString()
        prefs.edit().putStringSet(key, values).apply()
    }

    fun scan(): List<GameEntry> {
        val result = mutableListOf<GameEntry>()
        prefs.getStringSet(key, emptySet())!!.forEach { raw ->
            val uri = Uri.parse(raw)
            val doc = DocumentFile.fromTreeUri(context, uri) ?: return@forEach
            scanTree(doc, result)
        }
        return result.distinctBy { it.path }.sortedBy { it.title.lowercase() }
    }

    private fun scanTree(root: DocumentFile, out: MutableList<GameEntry>) {
        val name = root.name.orEmpty()
        val upper = name.uppercase()
        if (root.isDirectory && (upper == "PS3_GAME" || upper.startsWith("PS3_GAME"))) {
            out += GameEntry(root.parentFile?.name ?: name, root.uri.toString(), "PS3 folder")
            return
        }
        if (root.isFile && (upper.endsWith(".ISO") || upper.endsWith(".PKG"))) {
            out += GameEntry(name.substringBeforeLast('.'), root.uri.toString(), name.substringAfterLast('.', "file").uppercase())
            return
        }
        if (root.isDirectory) root.listFiles().forEach { child -> scanTree(child, out) }
    }
}
