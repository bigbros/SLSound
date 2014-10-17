package com.madtape.dllbuild;

import com.madtape.dllbuild.R.id;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;


public class MainActivity extends Activity implements OnClickListener{

	private NativeDebug mDebug = null;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mDebug = new NativeDebug();
        Button btnDebug = (Button)findViewById(id.btnDebug);
        btnDebug.setOnClickListener(this);
    }
    
    @Override
    public void onClick(View v) {
    	mDebug.TestMethod("log send");
    }
}
