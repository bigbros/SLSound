LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libmp3lame
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../include
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../include/mp3lame
LOCAL_CFLAGS +=	-I$(LOCAL_PATH)/../include/mp3lame/mpglib -I$(LOCAL_PATH)/
LOCAL_CFLAGS +=	-ffast-math -fsigned-char -DSTDC_HEADERS -DHAVE_MPGLIB
ifeq ($(TARGET_ARCH),arm)
	LOCAL_CFLAGS += -march=armv6 -marm -mfloat-abi=softfp -mfpu=vfp
endif

MPGLIB_SRC_FILES	:=	\
	mpglib/common.c \
	mpglib/dct64_i386.c \
	mpglib/decode_i386.c \
	mpglib/interface.c \
	mpglib/layer1.c \
	mpglib/layer2.c \
	mpglib/layer3.c \
	mpglib/tabinit.c

LOCAL_SRC_FILES := \
	bitstream.c \
	encoder.c \
	fft.c \
	gain_analysis.c \
	id3tag.c \
	lame.c \
	mpglib_interface.c \
	newmdct.c \
	presets.c \
	psymodel.c \
	quantize_pvt.c \
	quantize.c \
	reservoir.c \
	set_get.c \
	tables.c \
	takehiro.c \
	util.c \
	vbrquantize.c \
	VbrTag.c \
	version.c \
	$(MPGLIB_SRC_FILES)

include $(BUILD_SHARED_LIBRARY)