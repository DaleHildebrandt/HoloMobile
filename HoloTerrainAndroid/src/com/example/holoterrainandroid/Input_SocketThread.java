//Input_SocketThread.java
//##Description##
//Creates a thread for listening for messages coming in over socket. 
//Interprets the messages received and forwards them to a handler.

package com.example.holoterrainandroid;
import java.net.*;
import java.io.*;
import android.os.Handler;
import android.os.Message;
import android.util.Log;


public class Input_SocketThread extends Thread 
{

	//Various codes for specifying interaction mode
	//***ANY CHANGE MADE TO THESE VALUES MUST ALSO BE APPLIED TO THE VALUES IN MainActivity.java***
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

	Handler handler; //Handler to send received messages to after they have been interpreted

	String inputLine; //Holds most recently read line from socket
	BufferedReader in; //Reads from socket
	boolean isSetup = false;

	
	public Input_SocketThread(Socket socket, Handler handler) 
	{

		super("Accelero_SocketThread");
		this.handler = handler;
		
		//Attaches reader to socket for reading.
		try 
		{
			in = new BufferedReader(
					new InputStreamReader(socket.getInputStream()));
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
		isSetup = true;
		try 
		{
			
			while ( (inputLine = in.readLine()) != null) 
			{
				int msgCode = -1;
				Log.d("Socket_Input", "reading from input stream");
				Log.d("Socket_Input", inputLine);

				String[] tokens = inputLine.split(",");

				Message msg = new Message();
				
				//Interpret commands based on their format
				
				//Commands involving placing a single point
				//Format: command-name,x-coord,y-coord (ex. startpoint,14.2,16.7)
				if(tokens.length == 3 && tokens[0].equals("startpoint"))
					msgCode = SET_START;
				else if(tokens.length == 3 && tokens[0].equals("endpoint"))
					msgCode = SET_END;
				else if(tokens.length == 3 && tokens[0].equals("poi"))
					msgCode = SET_POI;
				else if(tokens.length == 3 && tokens[0].equals("freehand")){
					msgCode = ADD_TO_FREEHAND;
				}
				//Commands involving placing a point with a radius around it
				//Format: command-name, x-coord,y-coord,radius (ex. roi,12.3, 7.2,6)
				else if(tokens.length == 4 && tokens[0].equals("roi"))
					msgCode = SET_ROI;
				//Commands involving clearing information for a particular graphical item
				//Format: clear,command-name (ex. clear,endpoints)
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
				//Command confirmation messages
				else if(tokens.length == 1){
					msgCode = CONFIRM_MODE;
				}
				//Command setting height
				//Format: height, height-value (ex. height,3.6)
				else if(tokens.length == 2 && tokens[0].equals("height")){
					msgCode = SET_HEIGHT;
				}
				//Unknown command
				else{
					Log.e("Socket_Received", "Error: Unknown Case");
				}
				
				//Compose the message and send it off to the handler
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