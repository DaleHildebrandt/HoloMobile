//MainActivity.java
//##Description##
//displays a user interface for switching between the various interactions with 
//the 3D terrain hologram (produced by the accompanying zSpace code), and also 
//displays a replicated (markers placed on the 3D hologram are also shown on the 
//2D map in the corresponding location) top-down view of the hologram.

package com.example.holoterrainandroid;

import java.io.InputStream;

import com.example.holoterrainandroid.ClientSocket;
import com.example.holoterrainandroid.R;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Point;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.ToggleButton;

public class MainActivity extends Activity {
	//Various codes for specifying interaction mode
	//***ANY CHANGE MADE TO THESE VALUES MUST ALSO BE APPLIED TO THE VALUES IN Input_SocketThread.java***
	final int SET_START = 0;
	final int SET_END = 1;
	final int SET_ROI = 2;
	final int ADD_TO_FREEHAND = 3;
	final int CONFIRM_MODE = 4;
	final int SET_HEIGHT = 5;
	final int CLEAR_FREEHAND_PATH_MODE = 6;
	final int SET_POI = 7;
	final int CLEAR_ENDPOINTS_PATH_MODE = 8;
	final int CLEAR_POI_MODE = 9;
	final int CLEAR_ROI_MODE = 10;
	
	final String LOG_TAG = "MainActivity";
	
	//Width(X) & Height(Z) of 2D & 3D map
	final double MAP2D_X = 600.0;
	final double MAP2D_Z = 600.0;
	final double MAP3D_X = 32.0;
	final double MAP3D_Z = 32.0;
	
	boolean modeConfirmed;
	private Handler handler; //Handles incoming messages over socket
	DrawView drawView; //Draws 2D map with overlay items
	
	//Buttons for selecting the various modes
	ToggleButton buttonPlacePath;
	ToggleButton buttonFilterHeight;
	ToggleButton buttonDrawPath;
	ToggleButton buttonPlaceRoi;
	ToggleButton buttonSelectHeight;
	ToggleButton buttonPlacePoi;
	
	//Socket for communicating with accompanying zSpace code
	private ClientSocket socket;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		modeConfirmed = true;
		
		buttonPlacePath = (ToggleButton) findViewById(R.id.buttonPlacePath);
		buttonFilterHeight = (ToggleButton) findViewById(R.id.buttonFilterHeight);
		buttonDrawPath = (ToggleButton) findViewById(R.id.buttonDrawPath);
		buttonPlaceRoi = (ToggleButton) findViewById(R.id.buttonPlaceRoi);
		buttonPlacePoi = (ToggleButton) findViewById(R.id.buttonPlacePoi);
		buttonSelectHeight = (ToggleButton) findViewById(R.id.buttonSelectHeight);
		
		drawView = (DrawView) findViewById(R.id.drawView1);
        
        initSocketHandler();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	
	//##Button Handlers##
	 public void placePath(View view) {
		 socket.setOutputString("placepath");
		 
		 Log.e(LOG_TAG, "Error: Socket Not Initialized");
	
	     modeConfirmed = false;

	     uncheckAllButtons();
	     buttonPlacePath.setChecked(true);
	 }
	 public void drawPath(View view) {
		 socket.setOutputString("drawpath");

		 Log.e(LOG_TAG, "Error: Socket Not Initialized");
	     modeConfirmed = false;

	     uncheckAllButtons();
	     buttonDrawPath.setChecked(true);
	 }
	 
	 public void placeRoi(View view) {
		 socket.setOutputString("placeroi");

		 Log.e(LOG_TAG, "Error: Socket Not Initialized");
	     modeConfirmed = false;

	     uncheckAllButtons();
	     buttonPlaceRoi.setChecked(true);
	 }
	 public void placePoi(View view) {
		 socket.setOutputString("placepoi");

		 Log.e(LOG_TAG, "Error: Socket Not Initialized");
	     modeConfirmed = false;
	     
	     uncheckAllButtons();
	     buttonPlacePoi.setChecked(true);	 
	 }
	 public void filterHeight(View view) {
		 socket.setOutputString("filterheight");

		 Log.e(LOG_TAG, "Error: Socket Not Initialized");
		 
	     modeConfirmed = false;
	     
	     uncheckAllButtons();
	     buttonFilterHeight.setChecked(true);
	 }
	 public void selectHeight(View view){
		 socket.setOutputString("selectheight");
		 
		 Log.e(LOG_TAG, "Error: Socket Not Initialized");
		 
		 modeConfirmed = false;
		 
		 uncheckAllButtons();
		 buttonSelectHeight.setChecked(true);
	 }
	 private void uncheckAllButtons(){
		 buttonPlacePath.setChecked(false);
		 buttonDrawPath.setChecked(false);
		 buttonPlaceRoi.setChecked(false);
		 buttonPlacePoi.setChecked(false);
		 buttonFilterHeight.setChecked(false);
		 buttonSelectHeight.setChecked(false);
	 }
	 
	 private void setButtonVisuals(){
		 if(buttonPlacePath.isChecked())
			 buttonPlacePath.setBackgroundColor(Color.RED);
		 if(buttonDrawPath.isChecked())
			 buttonDrawPath.setBackgroundColor(Color.RED);
		 if(buttonPlaceRoi.isChecked())
			 buttonPlaceRoi.setBackgroundColor(Color.RED);
		 if(buttonPlacePoi.isChecked())
			 buttonPlacePoi.setBackgroundColor(Color.RED);
		 if(buttonFilterHeight.isChecked())
			 buttonFilterHeight.setBackgroundColor(Color.RED);
		 if(buttonSelectHeight.isChecked())
			 buttonSelectHeight.setBackgroundColor(Color.RED);
	 }
	 
	private void initSocketHandler(){
		handler = new Handler(){
			@Override
			public void handleMessage(Message msg) {
				Log.e("Socket Received", "Handling Message");
				String fullCmd; //The entire command string
				String[] tokens; //Tokenized version of the command string
				Double x; //holds the x value from a command
				Double z; //holds the z value from a command
				int displayX; //appropriate x value for display on 2D map
				int displayZ; //appropriate y value for display on 2D map
				double roiRadius; //holds the radius value from a roi command
				double height; //holds the height value from a height command
				
				fullCmd = (String)msg.obj;
				tokens = fullCmd.split(",");
				
				//Determine how the message should be handled based on its code
				switch(msg.what){
					
					case SET_START:
						Log.e("Socket Received", "Case SET_START");

						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						drawView.setStart(createDisplayPoint(x, z));
					break;	
					
					case SET_END:
						Log.e("Socket Received", "Case SET_END");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						
						drawView.setEnd(createDisplayPoint(x, z));
					break;
					
					case SET_POI:
						Log.e("Socket Received", "Case SET_POI");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						
						drawView.setPOI(createDisplayPoint(x, z));
						
					break;
					
					case SET_ROI:
						Log.e("Socket Received", "Case SET_ROI");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						
						roiRadius = (Double.parseDouble(tokens[3])/32.0)*600.0;
						drawView.setROI(createDisplayPoint(x, z), (int)roiRadius);
					break;
					
					case ADD_TO_FREEHAND:
						Log.e("Socket Received", "Case ADD_TO_FREEHAND");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						
						drawView.addToFreehand(createDisplayPoint(x, z));
					break;
					
					case CONFIRM_MODE:
						socket.updateModeConfirmation(fullCmd);
					break;
					
					case SET_HEIGHT:
						Log.e("Socket_Received", "Case SET_HEIGHT");
						int level = 0;
						
						//Determines which image of the map to display based on the currently-selected height
						//resulting images shows a horizontal slice of map close to the selected height
						height = Double.parseDouble(tokens[1]);
						if(height > 0.0 && height < 0.02)
							level = 1;
						else if(height >= 0.02 && height < 0.025)
							level = 2;
						else if(height >= 0.025 && height < 0.03)
							level = 3;
						else if(height >= 0.03 && height < 0.045)
							level = 4;
						else if(height >= 0.045)
							level = 5;
						
						drawView.setImage(level);
					break;
					
					case CLEAR_FREEHAND_PATH_MODE:
						drawView.clearFreehand();
					break;
					
					case CLEAR_ENDPOINTS_PATH_MODE:
						drawView.clearEndpoints();
					break;
					
					case CLEAR_ROI_MODE:
						drawView.clearRoi();
					break;
					
					case CLEAR_POI_MODE:
						drawView.clearPoi();
					break;

				}
			}
		};

		socket = new ClientSocket();
		socket.execute(handler, null, null);

	}
	
	//Given a point in 3D Terrain coordinates, create a corresponding point in the 2D coordinates
	private Point createDisplayPoint(Double x, Double z){
		Point result;
		result = new Point((int)((x/MAP3D_X)*MAP2D_X), (int)((-z/MAP3D_Z)*MAP2D_Z));
		return result;
	}

}
