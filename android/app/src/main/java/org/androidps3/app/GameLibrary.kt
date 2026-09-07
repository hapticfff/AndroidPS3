package org.androidps3.app

import android.content.Context
import android.net.Uri
import androidx.documentfile.provider.DocumentFile

data class GameEntry(val title: String, val path: String, val type: String, val source: Uri)

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
            val single = DocumentFile.fromSingleUri(context, uri)
            if (single != null && single.exists() && single.isFile) {
                addFile(single, result)
                return@forEach
            }
            val tree = DocumentFile.fromTreeUri(context, uri)
            if (tree != null && tree.exists()) scanTree(tree, result)
        }
        return result.distinctBy { it.path }.sortedBy { it.title.lowercase() }
    }

    private fun scanTree(root: DocumentFile, out: MutableList<GameEntry>) {
        val name = root.name.orEmpty()
        val upper = name.uppercase()
        if (root.isDirectory && (upper == "PS3_GAME" || upper.startsWith("PS3_GAME"))) {
            val title = root.parentFile?.name?.takeIf { it.isNotBlank() } ?: name
            out += GameEntry(title, root.uri.toString(), "PS3 GAME FOLDER", root.uri)
            return
        }
        if (root.isFile) { addFile(root, out); return }
        if (root.isDirectory) root.listFiles().forEach { scanTree(it, out) }
    }

    private fun addFile(file: DocumentFile, out: MutableList<GameEntry>) {
        val name = file.name.orEmpty()
        val upper = name.uppercase()
        if (upper.endsWith(".ISO") || upper.endsWith(".PKG")) {
            val type = name.substringAfterLast('.', "FILE").uppercase()
            out += GameEntry(name.substringBeforeLast('.'), file.uri.toString(), type, file.uri)
        }
    }
}
