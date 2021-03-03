#pragma once
#include "stddef.h"
#include "vga.h"

void initPhysics();

struct molecule
{
    int X;
    int Y;
    int color;
};

class physics{
private:
	int gravityCenterX;
	int gravityCenterY;
	bool planeGravity;
	int gravityForce;
	molecule mol[320*200];
	int molnum;
	molecule Emptymol;
public:
	physics(bool gravityMode, int Force)// 1 = plane mode, 0 = spheric mode (like earth)
	{
		if (gravityMode){
			planeGravity=1;
			gravityCenterX=160;
			gravityCenterY=200;
		}
		molnum=0;
		gravityForce=Force;
	}
	
	void SetGravity(int X,int Y, int Force){
		gravityCenterX = X;
		gravityCenterY = Y;
		gravityForce=Force;
	}
	
	void setWater(int X,int Y){
		mol[molnum].color=1;
		//mol[molnum].Type="Water";
		mol[molnum].X=X;
		mol[molnum].Y=Y;
		molnum++;
	}
    
	
	bool iterate(int number){
		if (number > 1){
			return 0;
		}
        DeskColor(15);
		for (int ii = 0; ii < molnum; ii++)
    	{
            putPixel(mol[ii].X,mol[ii].Y,mol[ii].color);
            if (mol[ii].X!=160) mol[ii].X = gravityForce/160-mol[ii].X;
            if (mol[ii].Y!=100) mol[ii].Y = gravityForce/100-mol[ii].Y;
    	}
		Screen1.swap();
		
	}
};