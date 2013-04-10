package com.example.holoterrainviewer;
import java.net.*;
import java.io.*;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

/*
*
*/
public class Input_SocketThread extends Thread 
{

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
	//private Socket socket = null;
	Handler handler;

	String inputLine;
	BufferedReader in;
	InputStreamReader in2;
	DataInputStream dataInputStream;
	
	public Input_SocketThread(Socket socket, Handler handler) 
	{

		super("Accelero_SocketThread");
		//this.socket = socket;
		this.handler = handler;

		try 
		{
			//dataInputStream = new DataInputStream(socket.getInputStream());
			//in = new BufferedReader(new InputStreamReader(new BufferedInputStream(socket.getInputStream())));
			in = new BufferedReader(
					new InputStreamReader(socket.getInputStream()));
			//inputStream = new InputStreamReader(socket.getInputStream());
			//in = new BufferedReader(in, priority);
		} 
		catch (IOException e) 
		{
			Log.e("Socket IOExcpetion", "Unable to set up input stream");
			e.printStackTrace();
		}
		System.out.println("Reader created : ");
	}
		
		
	public void run() 
	{
	
		try 
		{
			
			while ( (inputLine = in.readLine()) != null) 
			{
				int msgCode = -1;
				Log.d("Socket_Input", "reading from input stream");
				Log.d("Socket_Input", inputLine);
				//System.out.println("Receiving " + inputLine);
				String[] tokens = inputLine.split(",");
				//handler.handleMessage(inputLine);
				Message msg = new Message();
				if(tokens.length == 3 && tokens[0].equals("startpoint"))
					msgCode = SET_START;
				else if(tokens.length == 3 && tokens[0].equals("endpoint"))
					msgCode = SET_END;
				else if(tokens.length == 4 && tokens[0].equals("roi"))
					msgCode = SET_ROI;
				else if(tokens.length == 3 && tokens[0].equals("poi"))
					msgCode = SET_POI;
				else if(tokens.length == 3 && tokens[0].equals("freehand")){
					msgCode = ADD_TO_FREEHAND;
				}
				else if(tokens.length == 2 && tokens[0].equals("clear") && tokens[1].equals("freehand")){
					msgCode = CLEAR_FREEHAND_PATH_MODE;
				}
				else if(tokens.length == 2 && tokens[0].equals("clear") && tokens[1].equals("endpoints")){
					msgCode = CLEAR_ENDPOINTS_PATH_MODE;
				}
				else if(tokens.length == 2 && tokens[0].equals("clear") && tokens[1].equals("poi")){
					msgCode = CLEAR_POI_MODE;
				}
				else if(tokens.length == 2 && tokens[0].equals("clear") && tokens[1].equals("roi")){
					msgCode = CLEAR_ROI_MODE;
				}
				else if(tokens.length == 1){
					msgCode = CONFIRM_MODE;
				}
				else if(tokens.length == 2 && tokens[0].equals("height")){
					msgCode = SET_HEIGHT;
				}
				else{
					Log.e("Socket_Received", "Error: Unknown Case");
				}
				msg.what = msgCode;
				msg.obj = inputLine;
				handler.sendMessage(msg);
				
				Log.d("Input", "Input Received");
				
			}

			
		} 
		catch (IOException e) 
		{
			Log.d("Socket_Input_Exception", e.toString());
			e.printStackTrace();
		}
	}
}