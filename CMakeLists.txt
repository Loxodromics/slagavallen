cmake_minimum_required(VERSION 3.14)

project(slagavallen LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools REQUIRED)
set(SOURCES
	src/main.cpp
	src/game/game.cpp
	src/graphic/backgroundnode.cpp
	src/graphic/gameworlditem.cpp
	src/graphic/terrainmaterial.cpp
	src/graphic/terrainnode.cpp
	src/graphic/textureatlas.cpp
	src/logic/hex.cpp
	src/logic/map.cpp
	src/logic/mapgenerator.cpp
	src/logic/tile.cpp
	src/thirdparty/simplexnoise.cpp
	qml/qml.qrc)

set(TS_FILES resources/slagavallen_en_001.ts)
qt_add_resources(SOURCES resources.qrc)

if(ANDROID)
	add_library(slagavallen SHARED
		${SOURCES}
		${TS_FILES}
	)
else()
	add_executable(slagavallen
		${SOURCES}
		${TS_FILES}
	)
endif()

target_compile_definitions(slagavallen
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(slagavallen
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
