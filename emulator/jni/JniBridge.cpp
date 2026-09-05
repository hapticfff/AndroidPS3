#include <jni.h>
#include <memory>
#include <string>
#include "emulator/core/EmulatorCore.h"

namespace { std::unique_ptr<emu::core::EmulatorCore> g_core; }
extern "C" {
JNIEXPORT jboolean JNICALL Java_org_androidps3_app_NativeBridge_nativeInitialize(JNIEnv*, jobject){
    if(!g_core) g_core=std::make_unique<emu::core::EmulatorCore>();
    return g_core->Initialize()?JNI_TRUE:JNI_FALSE;
}
JNIEXPORT void JNICALL Java_org_androidps3_app_NativeBridge_nativeShutdown(JNIEnv*, jobject){if(g_core)g_core->Shutdown();}
JNIEXPORT jboolean JNICALL Java_org_androidps3_app_NativeBridge_nativeRunFrame(JNIEnv*, jobject){return g_core&&g_core->RunFrame()?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jstring JNICALL Java_org_androidps3_app_NativeBridge_nativeGetStatusString(JNIEnv* env,jobject){std::string s=g_core?g_core->GetStatusString():"EmulatorCore not created";return env->NewStringUTF(s.c_str());}
JNIEXPORT jstring JNICALL Java_org_androidps3_app_NativeBridge_nativeGetVersionString(JNIEnv* env,jobject){std::string s=g_core?g_core->GetVersionString():"unknown";return env->NewStringUTF(s.c_str());}
}
