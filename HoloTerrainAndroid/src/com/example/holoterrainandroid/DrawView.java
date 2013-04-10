//DrawView.java
//##Description##
//Handles drawing of 2D map content (2D map & various overlay information, such as points of interest, paths, etc.)
//##NOTES##
//Provided images must rotated 180 degrees, in order to maintain their original orientation (due to the view matrix manipulations used)
package com.example.holoterrainandroid;

import java.io.InputStream;
import java.util.ArrayList;

import com.example.holoterrainandroid.R;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Point;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;

public class DrawView extends View {
	
	//##General Drawing##
    Paint paint = new Paint(); //allows for drawing
    Matrix viewMatrix; //Converts the display coordinate system that is used for drawing (to un-invert Y-axis)
    
    //##Background Image##
    int currBitmapID; //ID for the current background image (2D map in this case)
    Bitmap[] images; //all background images
    int[] bitmapID; //IDs of each background image
    
    //##Overlay Items##
    //path via endpoints
    Point start;
    Rect startRect;
    Point end;
    Rect endRect;
    //ROI
    ROI roi;
    Rect roiRect;
    //POI
    Point poi;
    Rect poiRect;
    //freehand path
    ArrayList<Point> polyLine;
    
    final float CIRCLE_RADIUS = 10.0f;

    
    public DrawView(Context context, AttributeSet attrs) {
        super(context, attrs);
        
        //Store the IDs for the different versions of the 2D map
        bitmapID = new int[6];
		bitmapID[0] = R.drawable.terrain_2d_square_flipped_vertically;
		bitmapID[1] = R.drawable.level_1_crop;
		bitmapID[2] = R.drawable.level_2_crop;
		bitmapID[3] = R.drawable.level_3_crop;
		bitmapID[4] = R.drawable.level_4_crop;
		bitmapID[5] = R.drawable.level_5_crop;


		InputStream is;
		images = new Bitmap[6];
		//Load all the images
		for(int i = 0; i < bitmapID.length; i++){
			is = this.getResources().openRawResource(bitmapID[i]);
			images[i] = BitmapFactory.decodeStream(is);
		}
		//Set the current image
		currBitmapID = 0;

		viewMatrix = new Matrix();
        start = null;
        startRect = null;
        end = null;
        endRect = null;
        roi = null;
        roiRect = null;
        poi = null;
        poiRect = null;

       

        polyLine = new ArrayList<Point>();
        
    	setUpViewMatrix();
    }
    //##Set a particular overlay##
    public void setStart(Point newStart){
    	if(start == null)
    		start = new Point(newStart.x, newStart.y);
    	else 
    		start.set(newStart.x, newStart.y);
    		
		startRect = createRect(start, (int)CIRCLE_RADIUS);
    	this.invalidate();
    }
    
    public void setEnd(Point newEnd){
    	if(end == null)
    		end = new Point(newEnd.x, newEnd.y);
    	else 
    		end.set(newEnd.x, newEnd.y);
    	
    	endRect = createRect(end, (int)CIRCLE_RADIUS);
    	this.invalidate();
    }
    
    public void addToFreehand(Point next){
    	polyLine.add(next);
    	this.invalidate();
    }
    
    public void setROI(Point poi, int radius){
    	if(roi == null)
    		roi = new ROI(poi, radius);
    	else
    		roi.setROI(poi, radius);
    	
    	roiRect = createRect(roi.getPOI(), (int)CIRCLE_RADIUS);
    	this.invalidate();
    }
    public void setPOI(Point newPoi) {
    	if(poi == null)
    		poi = new Point(newPoi.x, newPoi.y);
    	else
    		poi.set(newPoi.x, newPoi.y);
    	
    	poiRect = createRect(poi, (int)CIRCLE_RADIUS);
    	this.invalidate();
    }
    
    //sets the currently-displayed background image
    public void setImage(int id){
    	currBitmapID = id;
    	this.invalidate();
    }
    
    //##Clear a particular overlay##
    public void clearFreehand(){
    	polyLine.clear();
    	this.invalidate();
    }
    
    public void clearEndpoints(){
    	start = null;
    	end = null;
    	this.invalidate();
    }
    
    public void clearRoi(){
    	roi = null;
    	this.invalidate();
    }
    
    public void clearPoi(){
    	poi = null;
    	this.invalidate();
    }
    
    //##Drawing##
    @Override
    public void onDraw(Canvas canvas) {
    	
    	canvas.setMatrix(viewMatrix); //Transform drawing coordinate system
    	canvas.drawBitmap(images[currBitmapID], null, new Rect(0, 0, 600, 600), paint); //Draws the background map image

        
        //Draw Endpoints path
    	if(start != null && end != null){
	        paint.setStrokeWidth(6);
            paint.setColor(Color.CYAN);
            canvas.drawLine(start.x, start.y, end.x, end.y, paint);
	        paint.setStrokeWidth(1);
    	}

        
        //Draw Startpoint of Endpoints path
        if(start != null){
	        paint.setColor(Color.RED);
	        canvas.drawRect(startRect, paint);
        }
        
        //Draw Endpoint of Endpoints path
        if(end != null){
            paint.setColor(Color.BLUE);
            canvas.drawRect(endRect, paint);
        }

    	//Draw POI
        if(poi != null){
            paint.setColor(Color.YELLOW);
            canvas.drawRect(poiRect, paint);
        }
        
        //Draw freehand path
        paint.setColor(Color.MAGENTA);
        paint.setStrokeWidth(6);
        for(int i = 0; i < polyLine.size()-1; i++){
        	canvas.drawLine(polyLine.get(i).x, polyLine.get(i).y, polyLine.get(i+1).x, polyLine.get(i+1).y, paint);
        }
        paint.setStrokeWidth(1);

        //Draw ROI
        if(roi != null){
	        paint.setARGB(255, 255, 255, 0);
	        paint.setStyle(Paint.Style.STROKE); //Radius Stroke
	        canvas.drawCircle(roi.getPOI().x, roi.getPOI().y, roi.getRadius(), paint);
	        
	        
	        paint.setStyle(Paint.Style.FILL); //Radius fill
	        paint.setARGB(30, 255, 255, 0);
	        canvas.drawCircle(roi.getPOI().x, roi.getPOI().y, roi.getRadius(), paint);
	        
	        paint.setARGB(255, 255, 0, 255); //ROI's POI
	        //canvas.drawCircle(roi.getPOI().x, roi.getPOI().y, CIRCLE_RADIUS, paint);
	        //canvas.drawRect(new Rect((int)(roi.getPOI().x-CIRCLE_RADIUS), (int)(roi.getPOI().y+CIRCLE_RADIUS), (int)(roi.getPOI().x+CIRCLE_RADIUS), (int)(roi.getPOI().y-CIRCLE_RADIUS)), paint);
	        canvas.drawRect(roiRect, paint);
        }
        
        
        paint.reset(); //Reset drawing state vars
    }
    
    //Creates a rectangle used for drawing a specific point on the 2D map
    private Rect createRect(Point point, int radius) {
    	Rect result;
    	result = new Rect((int)(point.x-radius), (int)(point.y+radius), (int)(point.x+radius), (int)(point.y-radius));
    	return result;	
    }
    
    //Converts the display coordinate system so that the Y-axis is no longer inverted (ie. higher y-value => appearing higher on-screen)
    private void setUpViewMatrix() {
    	
    	viewMatrix.preTranslate(0.0f, 600.0f);
    	viewMatrix.preScale(1.0f, -1.0f);

    }


}