package com.example.holoterrainviewer;

import java.io.InputStream;
import java.util.ArrayList;

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
	final int OFFSCREEN_X = -100;
	final int OFFSCREEN_Y = -100;
    Paint paint = new Paint();
    Bitmap currBitmap;
    int currBitmapID;
    Bitmap[] images;
    int[] bitmapID;
    Matrix viewMatrix;
    Point start;
    Rect startRect;
    Point end;
    Rect endRect;
    ROI roi;
    Rect roiRect;
    Point poi;
    Rect poiRect;
    ArrayList<Point> polyLine;
    Path path;
    int inc;
    final float CIRCLE_RADIUS = 10.0f;

//    public DrawView(Context context) {
//        super(context);    
//		bitmapID = new int[6];
//		bitmapID[0] = R.drawable.terrain_2d_square_flipped_vertically;
//		bitmapID[1] = R.drawable.level_1_crop;
//		bitmapID[2] = R.drawable.level_2_crop;
//		bitmapID[3] = R.drawable.level_3_crop;
//		bitmapID[4] = R.drawable.level_4_crop;
//		bitmapID[5] = R.drawable.level_5_crop;
//
//        //currBitmapID = bitmapID[0];
//		InputStream is;
//		
//		//Load all the images
//		for(int i = 0; i < bitmapID.length; i++){
//			is = this.getResources().openRawResource(bitmapID[i]);
//			images[i] = BitmapFactory.decodeStream(is);
//		}
//		//currBitmap = BitmapFactory.decodeStream(is);
//		
//		viewMatrix = new Matrix();
//        start = new Point(OFFSCREEN_X, OFFSCREEN_Y);
//        end = new Point(OFFSCREEN_X, OFFSCREEN_Y);
//        poi = new Point(OFFSCREEN_X, OFFSCREEN_Y);
//        //roi = new ROI(new Point(OFFSCREEN_X, OFFSCREEN_Y), 1);
//        roi = new ROI(new Point(200, 200), 200);
//        path = new Path();
//        inc = 0;
//
//        polyLine = new ArrayList<Point>();
//        
//    	setUpViewMatrix();
//    }
    
    public DrawView(Context context, AttributeSet attrs) {
        super(context, attrs);
        
        bitmapID = new int[6];
		bitmapID[0] = R.drawable.terrain_2d_square_flipped_vertically;
		bitmapID[1] = R.drawable.level_1_crop;
		bitmapID[2] = R.drawable.level_2_crop;
		bitmapID[3] = R.drawable.level_3_crop;
		bitmapID[4] = R.drawable.level_4_crop;
		bitmapID[5] = R.drawable.level_5_crop;

        //currBitmapID = bitmapID[0];
		InputStream is;
		images = new Bitmap[6];
		//Load all the images
		for(int i = 0; i < bitmapID.length; i++){
			is = this.getResources().openRawResource(bitmapID[i]);
			images[i] = BitmapFactory.decodeStream(is);
		}
		currBitmapID = 0;


		viewMatrix = new Matrix();
//        start = new Point(OFFSCREEN_X, OFFSCREEN_Y);
//        end = new Point(OFFSCREEN_X, OFFSCREEN_Y);
//        roi = new ROI(new Point(OFFSCREEN_X, OFFSCREEN_Y), 1);
        //poi = new Point(OFFSCREEN_X, OFFSCREEN_Y);
        start = null;
        startRect = null;
        end = null;
        endRect = null;
        roi = null;
        roiRect = null;
        poi = null;
        poiRect = null;
        //roi = new ROI(new Point(200, 200), 200);
        path = new Path();
        inc = 0;

        polyLine = new ArrayList<Point>();
        
    	setUpViewMatrix();
        //init(context);
    }
    public void setStart(Point newStart){
    	if(start == null){
    		start = new Point(newStart.x, newStart.y);
    		startRect = new Rect((int)(start.x-CIRCLE_RADIUS), (int)(start.y+CIRCLE_RADIUS), (int)(start.x+CIRCLE_RADIUS), (int)(start.y-CIRCLE_RADIUS));
    	}else {
    		start.set(newStart.x, newStart.y);
    		startRect.set((int)(start.x-CIRCLE_RADIUS), (int)(start.y+CIRCLE_RADIUS), (int)(start.x+CIRCLE_RADIUS), (int)(start.y-CIRCLE_RADIUS));
    	}
    	
    	
    	this.invalidate();
    }
    
    public void setEnd(Point newEnd){
    	if(end == null){
    		end = new Point(newEnd.x, newEnd.y);
    		endRect = new Rect((int)(end.x-CIRCLE_RADIUS), (int)(end.y+CIRCLE_RADIUS), (int)(end.x+CIRCLE_RADIUS), (int)(end.y-CIRCLE_RADIUS));
    	} else {
    		end.set(newEnd.x, newEnd.y);
    		endRect.set((int)(end.x-CIRCLE_RADIUS), (int)(end.y+CIRCLE_RADIUS), (int)(end.x+CIRCLE_RADIUS), (int)(end.y-CIRCLE_RADIUS));
    	}
    	this.invalidate();
    }
    
    public void addToFreehand(Point next){
    	polyLine.add(next);
    	this.invalidate();
    }
    
    public void setROI(Point poi, int radius){
    	if(roi == null){
    		roi = new ROI(poi, radius);
    		roiRect = new Rect((int)(roi.getPOI().x-CIRCLE_RADIUS), (int)(roi.getPOI().y+CIRCLE_RADIUS), (int)(roi.getPOI().x+CIRCLE_RADIUS), (int)(roi.getPOI().y-CIRCLE_RADIUS));
    	}
    	else{
        	roi.setPOI(poi);
        	roi.setRadius(radius);
        	roiRect.set((int)(roi.getPOI().x-CIRCLE_RADIUS), (int)(roi.getPOI().y+CIRCLE_RADIUS), (int)(roi.getPOI().x+CIRCLE_RADIUS), (int)(roi.getPOI().y-CIRCLE_RADIUS));
    	}
    	this.invalidate();
    }
    public void setPOI(Point newPoi) {
    	if(poi == null){
    		poi = new Point(newPoi.x, newPoi.y);
    		poiRect = new Rect((int)(poi.x-CIRCLE_RADIUS), (int)(poi.y+CIRCLE_RADIUS), (int)(poi.x+CIRCLE_RADIUS), (int)(poi.y-CIRCLE_RADIUS));
    	}
    	else {
    		poi.set(newPoi.x, newPoi.y);
    		poiRect.set((int)(poi.x-CIRCLE_RADIUS), (int)(poi.y+CIRCLE_RADIUS), (int)(poi.x+CIRCLE_RADIUS), (int)(poi.y-CIRCLE_RADIUS));
    	}
    	this.invalidate();
    }
    
    public void setImage(int id){
    	//currBitmapID = bitmapID[id];
    	currBitmapID = id;
    	this.invalidate();
		//InputStream is = this.getResources().openRawResource(currBitmapID);
		//currBitmap = BitmapFactory.decodeStream(is);
    }
    
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
    @Override
    public void onDraw(Canvas canvas) {
    	
    	canvas.setMatrix(viewMatrix);
    	canvas.drawBitmap(images[currBitmapID], null, new Rect(0, 0, 600, 600), paint);
    	//canvas.DrawBitmap(bitmap, null, new Rect(100, 100, 200, 150), null);

        
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

	        //canvas.drawCircle(start.x, start.y, CIRCLE_RADIUS, paint);
	        //canvas.drawRect(new Rect((int)(start.x-CIRCLE_RADIUS), (int)(start.y+CIRCLE_RADIUS), (int)(start.x+CIRCLE_RADIUS), (int)(start.y-CIRCLE_RADIUS)), paint);
	        canvas.drawRect(startRect, paint);
	        //canvas.drawRect(start.x, start.y, CIRCLE_RADIUS, paint);

        }
        
        //Draw Endpoint of Endpoints path
        if(end != null){
            paint.setColor(Color.BLUE);
            //canvas.drawCircle(end.x, end.y, CIRCLE_RADIUS, paint);
            //canvas.drawRect(new Rect((int)(end.x-CIRCLE_RADIUS), (int)(end.y+CIRCLE_RADIUS), (int)(end.x+CIRCLE_RADIUS), (int)(end.y-CIRCLE_RADIUS)), paint);
            canvas.drawRect(endRect, paint);
        }

        
    	//Draw POI
        if(poi != null){
            paint.setColor(Color.YELLOW);
            //canvas.drawCircle(poi.x, poi.y, CIRCLE_RADIUS, paint);
            //canvas.drawRect(new Rect((int)(poi.x-CIRCLE_RADIUS), (int)(poi.y+CIRCLE_RADIUS), (int)(poi.x+CIRCLE_RADIUS), (int)(poi.y-CIRCLE_RADIUS)), paint);
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
        
        
        paint.reset(); //Reset state vars
//        canvas.drawRect(30, 30, 80, 80, paint);
//        paint.setStrokeWidth(0);
//        paint.setColor(Color.CYAN);
//        canvas.drawRect(33, 60, 77, 77, paint );
//        paint.setColor(Color.YELLOW);
//        canvas.drawRect(33, 33, 77, 60, paint );

    }
    private void setUpViewMatrix() {
    	
    	//viewMatrix.setTranslate(0.0f, 96.0f);
    	viewMatrix.preTranslate(0.0f, 600.0f);
    	viewMatrix.preScale(1.0f, -1.0f);


    	
    }


}