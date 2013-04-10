//Output_SocketThread
//##Description##
//Creates a seperate thread for sending information from Android over socket
//Used to indicate change of interaction mode to zSpace code

package com.example.holoterrainandroid;

import java.net.*;
import java.io.*;
import android.os.Handler;


public class Output_SocketThread extends Thread {

	Handler handler; //unnecessary?
	String prevOutputLine = null; //unnecessary?
	String outputLine= "marcos;123;12;8;23;23;23;23;23;90"; //Holds the string to output (gibberish by default)
	PrintWriter out ; //Used to write output to socket
	boolean newCommand; //Has the current message (ie. outputLine) changed since the last send?


	public Output_SocketThread(Socket sock, Handler handler) {
		super("Accelero_Thread");

		newCommand = true;
		this.handler = handler;
		
		//Attaches writer to socket for writing to.
		try 
		{
			out = new PrintWriter
			(sock.getOutputStream(), true);
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
		System.out.println("Writer created : ");
	}
			
			
	public void run() {
		while (true){
			
			//Only send if a new string has been sent, or a previously sent string hasn't received a confirmation message back.
			if(newCommand) 
			{
				out.println(outputLine); //Send message
				
				try
				{
					Thread.sleep(10);//Wait for 10 ms
				} 
				catch(Exception e)
				{
					System.out.println("Exception in Thread Sleeping : " + e);
				}
				
				prevOutputLine = outputLine; //unnecessary?
				newCommand = false; //Prevent continuously sending same string repeatedly
			}
		}
	}
	
	
}