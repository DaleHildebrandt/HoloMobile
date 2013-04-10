package com.example.holoterrainviewer;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

import android.os.AsyncTask;
import android.os.Handler;
import android.util.Log;

public class ClientSocket extends AsyncTask<Handler, Void, Void>{
	Output_SocketThread out_Thread;
	Input_SocketThread in_Thread;
	String modeConfirmation;
	boolean modeConfirmed;
	
	protected Void doInBackground(Handler... msgs){
		//Logcat tags
		final String NETWORK_GENERAL_TAG = "Socket";
		final String NETWORK_CONTENT_TAG = "SocketMsg";
		final String CONTEXT_TAG = "ClientSocket";
		modeConfirmation = null;
		modeConfirmed = true;
		Log.d(CONTEXT_TAG, "***Starting AsyncTask***");
		
		Socket clientSocket = null ;
		// Accept a client for the server
		try
		{
			//hci_lab_1-PC
			//clientSocket = new Socket("130.179.30.170", 60010);
			//Z_Space-PC
			clientSocket = new Socket("130.179.30.127", 60010);
			//clientSocket.setTcpNoDelay(true);
		} 
		catch (IOException e) 
		{
			System.out.println("IOException: "+ e);
			System.out.println("Accept failed: 2000");
			Log.e("IOException", "Accept failed");
			System.exit(-1);
		}
		Log.d(NETWORK_GENERAL_TAG, "Client Socket Created");


		Log.d(NETWORK_GENERAL_TAG, "Creating Input Socket Thread");
		in_Thread = new Input_SocketThread(clientSocket, msgs[0]);
		in_Thread.start();

		Log.d(NETWORK_GENERAL_TAG, "Creating Output Socket Thread");
		out_Thread = new Output_SocketThread(clientSocket, msgs[0]);
		out_Thread.start();
		
		confirm();


		//clientSocket.close();
		return null;
		
	
	
		
		//return null;
	}
	public void setOutputString(String line){
		out_Thread.outputLine = line;
		modeConfirmation = line;
		out_Thread.newCommand = true;
		modeConfirmed = false;
	}
	public void updateModeConfirmation(String line){

		if(line.equals(modeConfirmation)){
			modeConfirmed = true;
		}
	}
	public void confirm(){
		while(true){
			if(!modeConfirmed){
				out_Thread.newCommand = true;
			}
			
			try {
				Thread.sleep(200);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		
	}

	public void setSendValue(boolean _send){
		out_Thread.send = _send;
	}
}


