#include <jni.h>

#define SQLITEJDBC_JNI_VERSION JNI_VERSION_1_6

#include "NativeDB.h"


// JNI Method Registration Table Begin
static const JNINativeMethod method_table[] = {
    {"_open_utf8", "([BI)V", _Java_org_sqlite_core_NativeDB__1open_1utf8},
    {"_close", "()V", _Java_org_sqlite_core_NativeDB__1close},
    {"_exec_utf8", "([B)I", _Java_org_sqlite_core_NativeDB__1exec_1utf8},
    {"shared_cache", "(Z)I", _Java_org_sqlite_core_NativeDB_shared_1cache},
    {"enable_load_extension", "(Z)I", _Java_org_sqlite_core_NativeDB_enable_1load_1extension},
    {"interrupt", "()V", _Java_org_sqlite_core_NativeDB_interrupt},
    {"busy_timeout", "(I)V", _Java_org_sqlite_core_NativeDB_busy_1timeout},
    {"busy_handler", "(Lorg/sqlite/BusyHandler;)V", _Java_org_sqlite_core_NativeDB_busy_1handler},
    {"prepare_utf8", "([B)J", _Java_org_sqlite_core_NativeDB_prepare_1utf8},
    {"errmsg_utf8", "()[B", _Java_org_sqlite_core_NativeDB_errmsg_1utf8},
    {"libversion_utf8", "()[B", _Java_org_sqlite_core_NativeDB_libversion_1utf8},
    {"changes", "()I", _Java_org_sqlite_core_NativeDB_changes},
    {"total_changes", "()I", _Java_org_sqlite_core_NativeDB_total_1changes},
    {"finalize", "(J)I", _Java_org_sqlite_core_NativeDB_finalize},
    {"step", "(J)I", _Java_org_sqlite_core_NativeDB_step},
    {"reset", "(J)I", _Java_org_sqlite_core_NativeDB_reset},
    {"clear_bindings", "(J)I", _Java_org_sqlite_core_NativeDB_clear_1bindings},
    {"bind_parameter_count", "(J)I", _Java_org_sqlite_core_NativeDB_bind_1parameter_1count},
    {"column_count", "(J)I", _Java_org_sqlite_core_NativeDB_column_1count},
    {"column_type", "(JI)I", _Java_org_sqlite_core_NativeDB_column_1type},
    {"column_decltype_utf8", "(JI)[B", _Java_org_sqlite_core_NativeDB_column_1decltype_1utf8},
    {"column_table_name_utf8", "(JI)[B", _Java_org_sqlite_core_NativeDB_column_1table_1name_1utf8},
    {"column_name_utf8", "(JI)[B", _Java_org_sqlite_core_NativeDB_column_1name_1utf8},
    {"column_text_utf8", "(JI)[B", _Java_org_sqlite_core_NativeDB_column_1text_1utf8},
    {"column_blob", "(JI)[B", _Java_org_sqlite_core_NativeDB_column_1blob},
    {"column_double", "(JI)D", _Java_org_sqlite_core_NativeDB_column_1double},
    {"column_long", "(JI)J", _Java_org_sqlite_core_NativeDB_column_1long},
    {"column_int", "(JI)I", _Java_org_sqlite_core_NativeDB_column_1int},
    {"bind_null", "(JI)I", _Java_org_sqlite_core_NativeDB_bind_1null},
    {"bind_int", "(JII)I", _Java_org_sqlite_core_NativeDB_bind_1int},
    {"bind_long", "(JIJ)I", _Java_org_sqlite_core_NativeDB_bind_1long},
    {"bind_double", "(JID)I", _Java_org_sqlite_core_NativeDB_bind_1double},
    {"bind_text_utf8", "(JI[B)I", _Java_org_sqlite_core_NativeDB_bind_1text_1utf8},
    {"bind_blob", "(JI[B)I", _Java_org_sqlite_core_NativeDB_bind_1blob},
    {"result_null", "(J)V", _Java_org_sqlite_core_NativeDB_result_1null},
    {"result_text_utf8", "(J[B)V", _Java_org_sqlite_core_NativeDB_result_1text_1utf8},
    {"result_blob", "(J[B)V", _Java_org_sqlite_core_NativeDB_result_1blob},
    {"result_double", "(JD)V", _Java_org_sqlite_core_NativeDB_result_1double},
    {"result_long", "(JJ)V", _Java_org_sqlite_core_NativeDB_result_1long},
    {"result_int", "(JI)V", _Java_org_sqlite_core_NativeDB_result_1int},
    {"result_error_utf8", "(J[B)V", _Java_org_sqlite_core_NativeDB_result_1error_1utf8},
    {"value_text_utf8", "(Lorg/sqlite/Function;I)[B", _Java_org_sqlite_core_NativeDB_value_1text_1utf8},
    {"value_blob", "(Lorg/sqlite/Function;I)[B", _Java_org_sqlite_core_NativeDB_value_1blob},
    {"value_double", "(Lorg/sqlite/Function;I)D", _Java_org_sqlite_core_NativeDB_value_1double},
    {"value_long", "(Lorg/sqlite/Function;I)J", _Java_org_sqlite_core_NativeDB_value_1long},
    {"value_int", "(Lorg/sqlite/Function;I)I", _Java_org_sqlite_core_NativeDB_value_1int},
    {"value_type", "(Lorg/sqlite/Function;I)I", _Java_org_sqlite_core_NativeDB_value_1type},
    {"create_function_utf8", "([BLorg/sqlite/Function;II)I", _Java_org_sqlite_core_NativeDB_create_1function_1utf8},
    {"destroy_function_utf8", "([BI)I", _Java_org_sqlite_core_NativeDB_destroy_1function_1utf8},
    {"free_functions", "()V", _Java_org_sqlite_core_NativeDB_free_1functions},
    {"limit", "(II)I", _Java_org_sqlite_core_NativeDB_limit},
    {"backup", "([B[BLorg/sqlite/core/DB/ProgressObserver;)I", _Java_org_sqlite_core_NativeDB_backup},
    {"restore", "([B[BLorg/sqlite/core/DB/ProgressObserver;)I", _Java_org_sqlite_core_NativeDB_restore},
    {"column_metadata", "(J)[[Z", _Java_org_sqlite_core_NativeDB_column_1metadata},
    {"set_commit_listener", "(Z)V", _Java_org_sqlite_core_NativeDB_set_1commit_1listener},
    {"set_update_listener", "(Z)V", _Java_org_sqlite_core_NativeDB_set_1update_1listener},
    {"register_progress_handler", "(ILorg/sqlite/ProgressHandler;)V", _Java_org_sqlite_core_NativeDB_register_1progress_1handler},
    {"clear_progress_handler", "()V", _Java_org_sqlite_core_NativeDB_clear_1progress_1handler},

};
static const jint method_table_size = sizeof(method_table) / sizeof(method_table[0]);

static jint JNI_OnLoad_sqlitejdbc0(JavaVM* vm, void* reserved) {
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

  return _Java_org_sqlite_core_NativeDB_JNI_OnLoad(vm, reserved);
}

static void JNI_OnUnload_sqlitejdbc0(JavaVM* vm, void* reserved) {
  JNIEnv* env = NULL;
  if ((*vm)->GetEnv(vm, (void**) &env, SQLITEJDBC_JNI_VERSION) != JNI_OK) {
    // Something is wrong but nothing we can do about this :(
    return;
  }

  _Java_org_sqlite_core_NativeDB_JNI_OnUnload(vm, reserved);
}

// As we build with -fvisibility=hidden we need to ensure we mark the entry load and unload functions used by the
// JVM as visible.

// Invoked by the JVM when statically linked
JNIEXPORT jint JNICALL JNI_OnLoad_sqlitejdbc(JavaVM* vm, void* reserved) {
  return JNI_OnLoad_sqlitejdbc0(vm, reserved);
}

// Invoked by the JVM when statically linked
JNIEXPORT void JNICALL JNI_OnUnload_sqlitejdbc(JavaVM* vm, void* reserved) {
  JNI_OnUnload_sqlitejdbc0(vm, reserved);
}

#ifndef SQLITEJDBC_BUILD_STATIC
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
  return JNI_OnLoad_sqlitejdbc0(vm, reserved);
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
  JNI_OnUnload_sqlitejdbc0(vm, reserved);
}
#endif /* SQLITEJDBC_BUILD_STATIC */
