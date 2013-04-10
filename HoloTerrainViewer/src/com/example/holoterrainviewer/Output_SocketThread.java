package com.example.holoterrainviewer;

import java.net.*;
import java.io.*;
import android.os.Handler;


public class Output_SocketThread extends Thread {

	//private Socket sock = null;
	Handler handler;
	String prevOutputLine = null;
	String inputLine, outputLine= "marcos;123;12;8;23;23;23;23;23;90";
	PrintWriter out ;
	boolean newCommand;
	boolean send = false;
	
	public Output_SocketThread(Socket sock, Handler handler) {
		super("Accelero_Thread");
		//this.sock = socket;
		newCommand = true;
		this.handler = handler;
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
			
			//if(prevOutputLine == null || (prevOutputLine != null && !prevOutputLine.equals(outputLine))){
				//System.out.println("Printing : " + outputLine);
				//handler.sendEmptyMessage(69); // update outputLine
			if(newCommand){
				out.println(outputLine);
				//System.out.println("Printing : Printed");
				try
				{
					Thread.sleep(10);
				} 
				catch(Exception e)
				{
					System.out.println("Exception in Thread Sleeping : " + e);
				}
				prevOutputLine = outputLine;
				newCommand = false;
			}

				
			//}
			
			
		}
	}
}