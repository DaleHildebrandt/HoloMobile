//ROI.java
//##Description##
//Represents a region of interest, for display on the 2D map (handled in DrawView).

package com.example.holoterrainandroid;

import android.graphics.Point;

public class ROI {
	Point poi; //2D point; the center of the region of interest
	int radius; //max. distance from 2D point that the region covers
	
	//##Constructors##
	public ROI(Point poi, int radius){
		this.poi = poi;
		this.radius = radius;
	}
	
	public ROI(){
		poi = null;
		radius = 1;
	}
	
	
	//##Getters##
	public Point getPOI(){
		return poi;
	}
	public int getRadius(){
		return radius;
	}
	
	//##Setters##
	public void setROI(Point poi, int radius){
		this.poi = poi;
		this.radius = radius;
	}
	public void setPOI(Point poi){
		this.poi = poi;
	}
	
	public void setRadius(int radius){
		this.radius = radius;
	}
}
