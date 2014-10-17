#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {

jint
JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_madtape_dllbuild_TestMethod(JNIEnv *env, jobject thiz, jstring msgj)
{
	const char* msg = env->GetStringUTFChars(msgj, 0);
	//__android_log_print(ANDROID_LOG_DEBUG, "DllBuild", "message:[%s]", msg);
	env->ReleaseStringUTFChars(msgj, msg);
}

}
