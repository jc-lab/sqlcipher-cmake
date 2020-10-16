cmake_minimum_required(VERSION 3.8)
project(project-zlib)

include(FetchContent)
set(SQLCIPHER_JDBC_TARGET_NAME sqlcipherjdbc CACHE STRING "sqlcipher-jdbc jni library name")
set(SQLCIPHER_JDBC_JNI         ON            CACHE BOOL "Build sqlcipher-jdbc jni library")

#set(SQLCIPHER_FETCH_INFO
#        URL      https://github.com/sqlcipher/sqlcipher/archive/v4.4.0.tar.gz
#        URL_HASH SHA256=0924b2ae1079717954498bda78a30de20ce2a6083076b16214a711567821d148
#        )
#
#set(FINDJNI_JAVA_HOME_X86_32 "C:/Program Files (x86)/ojdkbuild/java-1.8.0-openjdk-1.8.0.265-1")
#set(FINDJNI_JAVA_HOME_X86_64 "C:/Program Files/ojdkbuild/java-1.8.0-openjdk-1.8.0.265-1")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenSSL REQUIRED)

if (DEFINED X64)
    set(_BUILD_X64 ${X64})
else()
    set(_BUILD_X64 0)
    if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
        set(_BUILD_X64 1)
    endif()

    if(CMAKE_CL_64)
        set(_BUILD_X64 1)
    endif()
endif()

if (SQLCIPHER_JDBC_JNI)
    if (NOT FINDJNI_JAVA_HOME)
        if (_BUILD_X64)
            if (FINDJNI_JAVA_HOME_X86_64)
                set(FINDJNI_JAVA_HOME ${FINDJNI_JAVA_HOME_X86_64})
            endif()
        else()
            if (FINDJNI_JAVA_HOME_X86_32)
                set(FINDJNI_JAVA_HOME ${FINDJNI_JAVA_HOME_X86_32})
            endif()
        endif()
    endif()
    message("FINDJNI_JAVA_HOME: ${FINDJNI_JAVA_HOME}")

    set(JAVA_AWT_LIBRARY NotNeeded)
    set(JAVA_AWT_INCLUDE_PATH NotNeeded)
    find_package(JNI REQUIRED)
    list(REMOVE_ITEM JNI_LIBRARIES "NotNeeded")
    list(REMOVE_ITEM JNI_INCLUDE_DIRS "NotNeeded")
    message("JNI_LIBRARIES: ${JNI_LIBRARIES}")
    message("JNI_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")
endif(SQLCIPHER_JDBC_JNI)

FetchContent_Declare(
        fetch
        ${SQLCIPHER_FETCH_INFO}
)
FetchContent_GetProperties(fetch)
FetchContent_Populate(fetch)

if (MSVC)
    file(RELATIVE_PATH RELATIVE_SRC_PATH ${CMAKE_CURRENT_BINARY_DIR} ${fetch_SOURCE_DIR})

    message(INFO "sqlcipher: RELATIVE_SRC_PATH => ${RELATIVE_SRC_PATH}")
    string(REPLACE "/" "\\" _WINSTYLE_RELATIVE_SRC_PATH "${RELATIVE_SRC_PATH}")
    message(INFO "sqlcipher: _WINSTYLE_RELATIVE_SRC_PATH => ${_WINSTYLE_RELATIVE_SRC_PATH}")

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.c
            COMMAND nmake /f ${_WINSTYLE_RELATIVE_SRC_PATH}\\Makefile.msc sqlite3.c TOP=${_WINSTYLE_RELATIVE_SRC_PATH} OPT_FEATURE_FLAGS="-DSQLITE_HAS_CODEC"
            DEPENDS OpenSSL::Crypto
    )

    set(SQLITE_COMPILE_DEFINITIONS
            INCLUDE_MSVC_H=1
            SQLITE_OS_WIN=1
            _CRT_SECURE_NO_DEPRECATE
            _CRT_SECURE_NO_WARNINGS
            _CRT_NONSTDC_NO_DEPRECATE
            _CRT_NONSTDC_NO_WARNINGS
            SQLITE_THREADSAFE=1
            SQLITE_THREAD_OVERRIDE_LOCK=-1
            SQLITE_TEMP_STORE=1
            SQLITE_MAX_TRIGGER_DEPTH=100
            SQLITE_HAS_CODEC
            SQLITE_ENABLE_COLUMN_METADATA=1
            )
    if ("${BUILDING_LIBRARY_OPENSSL_LINK_TYPE}" MATCHES "dynamic")
        list(APPEND SQLITE_COMPILE_DEFINITIONS -DOPENSSL_OPT_WINDLL)
    endif()

    add_library(generate_sqlite_c ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.c)
    target_link_libraries(generate_sqlite_c
            PRIVATE
            OpenSSL::Crypto
            )
    target_include_directories(
            generate_sqlite_c
            PUBLIC
            ${fetch_SOURCE_DIR}
            ${fetch_SOURCE_DIR}/src
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    target_compile_definitions(generate_sqlite_c
            PUBLIC
            ${SQLITE_COMPILE_DEFINITIONS}
            )

    set(OPT_FEATURE_FLAGS)
    string(JOIN " -D" OPT_FEATURE_FLAGS ${SQLITE_COMPILE_DEFINITIONS})
    string(CONCAT OPT_FEATURE_FLAGS "-D" ${OPT_FEATURE_FLAGS})
    string(CONCAT OPT_FEATURE_FLAGS ${OPT_FEATURE_FLAGS} " -I$<TARGET_PROPERTY:OpenSSL::Crypto,INTERFACE_INCLUDE_DIRECTORIES>")

    message("OPT_FEATURE_FLAGS: ${OPT_FEATURE_FLAGS}")

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.dll
            COMMAND
                nmake /f ${_WINSTYLE_RELATIVE_SRC_PATH}\\Makefile.msc
                sqlite3.dll
                TOP=${_WINSTYLE_RELATIVE_SRC_PATH}
                OPT_FEATURE_FLAGS="${OPT_FEATURE_FLAGS}"
                LDOPTS="$<TARGET_PROPERTY:OpenSSL::Crypto,IMPORTED_LOCATION>"
            DEPENDS OpenSSL::Crypto
    )

    add_custom_target(
            sqlite3_shared
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.dll
    )

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.exe
            COMMAND
            nmake /f ${_WINSTYLE_RELATIVE_SRC_PATH}\\Makefile.msc
            sqlite3.exe
            TOP=${_WINSTYLE_RELATIVE_SRC_PATH}
            OPT_FEATURE_FLAGS="${OPT_FEATURE_FLAGS}"
            LDOPTS="$<TARGET_PROPERTY:OpenSSL::Crypto,IMPORTED_LOCATION>"
            DEPENDS OpenSSL::Crypto
    )

    add_custom_target(
            sqlite3_exe
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.exe
    )

    if (SQLCIPHER_JDBC_JNI)
        set(SQLITE_JDBC_SRC_FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/jdbc/NativeDB.h
                ${CMAKE_CURRENT_SOURCE_DIR}/jdbc/NativeDB.c
                ${CMAKE_CURRENT_SOURCE_DIR}/jdbc/jni_module.c
                )
        add_library(${SQLCIPHER_JDBC_TARGET_NAME} SHARED
                ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.c
                ${SQLITE_JDBC_SRC_FILES}
                )
        add_compile_definitions(${SQLCIPHER_JDBC_TARGET_NAME}
                PRIVATE
                ${SQLITE_COMPILE_DEFINITIONS}
                )
        target_link_libraries(${SQLCIPHER_JDBC_TARGET_NAME}
                PRIVATE
                ${JNI_LIBRARIES}
                OpenSSL::Crypto
                )
        target_include_directories(${SQLCIPHER_JDBC_TARGET_NAME}
                PRIVATE
                ${JNI_INCLUDE_DIRS}
                ${fetch_SOURCE_DIR}
                ${fetch_SOURCE_DIR}/src
                ${CMAKE_CURRENT_BINARY_DIR}
                )

        install(TARGETS ${SQLCIPHER_JDBC_TARGET_NAME})
    endif(SQLCIPHER_JDBC_JNI)

    add_library(sqlite3_build_dummy ${CMAKE_CURRENT_SOURCE_DIR}/dummy.cc)
    add_dependencies(sqlite3_build_dummy
            sqlite3_shared
            sqlite3_exe)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.exe DESTINATION "bin")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sqlite3.dll DESTINATION "lib")
else()

endif()
