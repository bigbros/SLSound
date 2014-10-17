package com.madtape.dllbuild;

public class NativeDebug {
	public NativeDebug() {		
		System.loadLibrary("ogg");
		System.loadLibrary("vorbis");
		System.loadLibrary("mp3lame");
		System.loadLibrary("slsound");
	}
	public native void TestMethod(String msg);
}
