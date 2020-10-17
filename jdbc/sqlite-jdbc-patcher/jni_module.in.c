#include <jni.h>

#include "NativeDB.h"

#define SQLITEJDBC_MODULE_NAME /*@SQLITEJDBC_MODULE_NAME@*/
#define SQLITEJDBC_JNI_VERSION JNI_VERSION_1_6

#define _CONCATS(a, b, c) a ## b ## c
#define JNI_FUCNTION_NAME(prefix, postfix) _CONCATS(prefix, SQLITEJDBC_MODULE_NAME, postfix)

jint _sqlitejdbc_jni_onload(JavaVM *vm, void *reserved);
void _sqlitejdbc_jni_onunload(JavaVM *vm, void *reserved);

// JNI Method Registration Table Begin
static const JNINativeMethod method_table[] = {
/*@METHOD_TABLE@*/
};
static const jint method_table_size = sizeof(method_table) / sizeof(method_table[0]);

static jint JNI_FUCNTION_NAME(JNI_OnLoad_, 0)(JavaVM* vm, void* reserved) {
  JNIEnv* env = NULL;
  jclass native_cls;
  jint ret;

  if ((*vm)->GetEnv(vm, (void**) &env, SQLITEJDBC_JNI_VERSION) != JNI_OK) {
    return JNI_ERR;
  }

  native_cls = (*env)->FindClass(env, "org/sqlite/core/NativeDB");
  if (!native_cls) {
    return JNI_ERR;
  }

  ret = (*env)->RegisterNatives(env, native_cls, method_table, method_table_size);
  if (ret != JNI_OK) {
    return ret;
  }

  return _sqlitejdbc_jni_onload(vm, reserved);
}

static void JNI_FUCNTION_NAME(JNI_OnUnload_, 0)(JavaVM* vm, void* reserved) {
  JNIEnv* env = NULL;
  if ((*vm)->GetEnv(vm, (void**) &env, SQLITEJDBC_JNI_VERSION) != JNI_OK) {
    // Something is wrong but nothing we can do about this :(
    return;
  }

  _sqlitejdbc_jni_onunload(vm, reserved);
}

// As we build with -fvisibility=hidden we need to ensure we mark the entry load and unload functions used by the
// JVM as visible.

// Invoked by the JVM when statically linked
JNIEXPORT jint JNICALL JNI_OnLoad_sqlitejdbc(JavaVM* vm, void* reserved) {
  return JNI_FUCNTION_NAME(JNI_OnLoad_, 0)(vm, reserved);
}

// Invoked by the JVM when statically linked
JNIEXPORT void JNICALL JNI_OnUnload_sqlitejdbc(JavaVM* vm, void* reserved) {
  JNI_FUCNTION_NAME(JNI_OnUnload_, 0)(vm, reserved);
}

#ifndef SQLITEJDBC_BUILD_STATIC
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
  return JNI_FUCNTION_NAME(JNI_OnLoad_, 0)(vm, reserved);
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
  JNI_FUCNTION_NAME(JNI_OnUnload_, 0)(vm, reserved);
}
#endif /* SQLITEJDBC_BUILD_STATIC */