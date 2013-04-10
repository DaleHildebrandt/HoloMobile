package com.example.holoterrainviewer;

import java.io.InputStream;

import com.example.holoterrainviewer.ClientSocket;

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
	
	final double MAP2D_X = 600.0;
	final double MAP2D_Z = 600.0;
	final double MAP3D_X = 32.0;
	final double MAP3D_Z = 32.0;
	boolean modeConfirmed;
	private Handler handler;
	DrawView drawView;
	
	ToggleButton buttonPlacePath;
	ToggleButton buttonFilterHeight;
	ToggleButton buttonDrawPath;
	ToggleButton buttonPlaceRoi;
	ToggleButton buttonSelectHeight;
	ToggleButton buttonPlacePoi;
	
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
		
		//pathButton.bringToFront();
		InputStream is = this.getResources().openRawResource(R.drawable.terrain_2d_square);
		Bitmap originalBitmap = BitmapFactory.decodeStream(is);
		
		drawView = (DrawView) findViewById(R.id.drawView1);
        //drawView = new DrawView(this);
        //drawView.setBackgroundColor(Color.WHITE);
        //setContentView(drawView);
        
        initSocketHandler();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	
	 public void placePath(View view) {
	     socket.setOutputString("placepath");
	     modeConfirmed = false;
	     //buttonPlacePath.setBackgroundColor(Color.RED);
	     uncheckAllButtons();
	     buttonPlacePath.setChecked(true);
	     //view.setSelected(true);
	     //setButtonVisuals();
	 }
	 public void drawPath(View view) {
	     socket.setOutputString("drawpath");
	     modeConfirmed = false;
	     //buttonDrawPath.setBackgroundColor(Color.RED);
	     uncheckAllButtons();
	     buttonDrawPath.setChecked(true);
	     //setButtonVisuals();
	 }
	 
	 public void placeRoi(View view) {
	     socket.setOutputString("placeroi");
	     modeConfirmed = false;
	     //buttonPlaceRoi.setBackgroundColor(Color.RED);
	     uncheckAllButtons();
	     buttonPlaceRoi.setChecked(true);
	     //setButtonVisuals();
	 }
	 public void placePoi(View view) {
	     socket.setOutputString("placepoi");
	     modeConfirmed = false;
	     uncheckAllButtons();
	     buttonPlacePoi.setChecked(true);
	     //setButtonVisuals();
		 
	 }
	 public void filterHeight(View view) {
	     socket.setOutputString("filterheight");
	     modeConfirmed = false;
	     //buttonFilterHeight.setBackgroundColor(Color.RED);
	     uncheckAllButtons();
	     buttonFilterHeight.setChecked(true);
	     //setButtonVisuals();
	 }
	 public void selectHeight(View view){
		 socket.setOutputString("selectheight");
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
				String startCmd;
				String[] tokens;
				Double x;
				Double z;
				int displayX;
				int displayZ;
				double roiRadius;
				double height;
				
				switch(msg.what){
					case 69: // Update tablet log infos
					socket.setOutputString("Hello world");	
					break;
					case 100: // Next Trial
					socket.setOutputString("Case 100");	
					break;
					
					case SET_START:
					//socket.setOutputString("Case 0");	
						Log.e("Socket Received", "Case SET_START");
						startCmd = (String)msg.obj;
						tokens = startCmd.split(",");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						displayX = (int)((x/MAP3D_X)*MAP2D_X);
						displayZ = (int)((-z/MAP3D_Z)*MAP2D_Z);
						drawView.setStart(new Point(displayX, displayZ));
					break;	
					
					case SET_END:
						Log.e("Socket Received", "Case SET_END");
						startCmd = (String)msg.obj;
						tokens = startCmd.split(",");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						displayX = (int)((x/MAP3D_X)*MAP2D_X);
						displayZ = (int)((-z/MAP3D_Z)*MAP2D_Z);
						//textBox.setText((String)msg.obj);
						drawView.setEnd(new Point(displayX, displayZ));
						//textBox.setText((String)msg.obj);
					break;
					
					case SET_POI:
						Log.e("Socket Received", "Case SET_POI");
						startCmd = (String)msg.obj;
						tokens = startCmd.split(",");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						displayX = (int)((x/MAP3D_X)*MAP2D_X);
						displayZ = (int)((-z/MAP3D_Z)*MAP2D_Z);
						//textBox.setText((String)msg.obj);
						drawView.setPOI(new Point(displayX, displayZ));
						//textBox.setText((String)msg.obj);
						
					break;
					
					case SET_ROI:
						Log.e("Socket Received", "Case SET_ROI");
						startCmd = (String)msg.obj;
						tokens = startCmd.split(",");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						displayX = (int)((x/MAP3D_X)*MAP2D_X);
						displayZ = (int)((-z/MAP3D_Z)*MAP2D_Z);
						roiRadius = (Double.parseDouble(tokens[3])/32.0)*600.0;
						//textBox.setText((String)msg.obj);
						drawView.setROI(new Point(displayX, displayZ), (int)roiRadius);
						//textBox.setText((String)msg.obj);
					break;
					
					case ADD_TO_FREEHAND:
						Log.e("Socket Received", "Case ADD_TO_FREEHAND");
						startCmd = (String)msg.obj;
						tokens = startCmd.split(",");
						x = Double.parseDouble(tokens[1]);
						z = Double.parseDouble(tokens[2]);
						displayX = (int)((x/MAP3D_X)*MAP2D_X);
						displayZ = (int)((-z/MAP3D_Z)*MAP2D_Z);
						//textBox.setText((String)msg.obj);
						//drawView.setEnd(new Point(displayX, displayZ));
						drawView.addToFreehand(new Point(displayX, displayZ));
						//drawView.invalidate();
						//textBox.setText((String)msg.obj);
						
						
					break;
					
					case CONFIRM_MODE:
						startCmd = (String)msg.obj;
						socket.updateModeConfirmation(startCmd);
					break;
					
					case SET_HEIGHT:
						Log.e("Socket_Received", "Case SET_HEIGHT");
						int level = 0;
						startCmd = (String)msg.obj;
						tokens = startCmd.split(",");
						height = Double.parseDouble(tokens[1]);
						if(height > 0.0 && height < 0.02){
							level = 1;
							drawView.setImage(level);
						}
						else if(height >= 0.02 && height < 0.025){
							level = 2;
							drawView.setImage(level);
						}
						else if(height >= 0.025 && height < 0.03){
							level = 3;
							drawView.setImage(level);
						}
						else if(height >= 0.03 && height < 0.045){
							level = 4;
							drawView.setImage(level);
						}
						else if(height >= 0.045){
							level = 5;
							drawView.setImage(level);
						}
						

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
//		handler = new ComHandler("null");
//		handler.setObserver(new Observer(){
//			public void callback(String msg){
//				
//				textBox.setText(msg);
//				Log.d("Socket_Recieve", "In callback");
//			}
//		});

		//if (!test){
		socket = new ClientSocket();
		socket.execute(handler, null, null);
		//}
	}


}
