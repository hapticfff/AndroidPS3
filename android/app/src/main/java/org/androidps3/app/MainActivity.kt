package org.androidps3.app
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import org.androidps3.app.databinding.ActivityMainBinding
class MainActivity:AppCompatActivity(){private lateinit var b:ActivityMainBinding;override fun onCreate(s:Bundle?){super.onCreate(s);b=ActivityMainBinding.inflate(layoutInflater);setContentView(b.root);val ok=NativeBridge.nativeInitialize();b.versionText.text=NativeBridge.nativeGetVersionString();b.statusText.text=if(ok){NativeBridge.nativeRunFrame();NativeBridge.nativeGetStatusString()}else"Failed to initialize emulator core"};override fun onDestroy(){NativeBridge.nativeShutdown();super.onDestroy()}}
