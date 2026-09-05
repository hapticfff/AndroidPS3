package org.androidps3.app

object NativeBridge {
    init { System.loadLibrary("androidps3_jni") }
    external fun nativeInitialize(): Boolean
    external fun nativeShutdown()
    external fun nativeRunFrame(): Boolean
    external fun nativeGetStatusString(): String
    external fun nativeGetVersionString(): String
}
