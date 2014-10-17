LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_PLATFORM	:=	android-14
APP_ABI			:=	all
APP_STL			:=	stlport_static

DEBUG_MODE	:=	on
SUPPORT_OGG	:=	off
SUPPORT_MP3	:=	off

LOCAL_MODULE := libslsnd
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../include
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../include/mp3lame
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../include/mp3lame/mpglib
LOCAL_CFLAGS += -I$(LOCAL_PATH)/.
LOCAL_CFLAGS += -ffast-math -fsigned-char
ifeq ($(TARGET_ARCH),arm)
	LOCAL_CFLAGS += -march=armv6 -marm -mfloat-abi=softfp -mfpu=vfp
endif
ifeq ($(DEBUG_MODE),on)
	LOCAL_CFLAGS += -DDEBUG
endif

LOCAL_SHARED_LIBRARIES :=
FILEIO_SRC_FILES	:=	CSLSoundFile.cpp CSLWavFile.cpp

ifeq ($(SUPPORT_OGG),on)
	LOCAL_SHARED_LIBRARIES	+=	libogg libvorbis
	FILEIO_SRC_FILES		+=	CSLOggFile.cpp
endif

ifeq ($(SUPPORT_MP3),on)
	LOCAL_SHARED_LIBRARIES	+=	libmp3lame
	FILEIO_SRC_FILES		+=	CSLMp3File.cpp
endif

LOCAL_SRC_FILES := \
	$(FILEIO_SRC_FILES) \
	JNIinterface.cpp


include $(BUILD_SHARED_LIBRARY)