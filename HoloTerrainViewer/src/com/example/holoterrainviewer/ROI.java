package com.example.holoterrainviewer;

import android.graphics.Point;

public class ROI {
	Point poi;
	int radius;
	public ROI(Point poi, int radius){
		this.poi = poi;
		this.radius = radius;
	}
	
	public ROI(){
		poi = null;
		radius = 1;
	}
	
	public Point getPOI(){
		return poi;
	}
	public int getRadius(){
		return radius;
	}
	
	public void setPOI(Point poi){
		this.poi = poi;
	}
	
	public void setRadius(int radius){
		this.radius = radius;
	}
}
