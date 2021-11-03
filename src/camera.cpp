/***************************************************************************
 *   Copyright (C) 2009-2013 by Veselin Georgiev, Slavomir Kaslev et al    *
 *   admin@raytracing-bg.net                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "camera.h"
#include "matrix.h"
#include "util.h"
#include "sdl.h"
#include <iostream>
using namespace std;

void Camera::beginFrame(void)
{
    //initial camera-grid parameters
	double x = -aspect;
	cout << "x=" << x << "\n";
	double y = +1;
	cout << "y=" << y << "\n";

	Vector corner = Vector(x, y, 1); //the up-left corner of the camera-grid with depth=1
	cout << "corner(x,y,z)=" << corner.x << ","<< corner.y << "," << corner.z << "\n";

	Vector center = Vector(0, 0, 1); //the center point of the camera-grid with depth=1
    cout << "center(x,y,z)=" << center.x << ","<< center.y << "," << center.z << "\n";

	double lenXY = (corner - center).length(); //the distance between corner and the center of the grid
	cout << "lenXY=" << lenXY << "\n";

	double wantedLength = tan(toRadians(fov / 2));
    cout << "fov=" << fov << "\n";
    cout << "wantedLength=" << wantedLength << "\n";

	double scaling = wantedLength / lenXY;
	cout << "scaling=" << scaling << "\n";

    //rescale x,y vectors to get the desired grid-size
	x *= scaling;
	cout << "x(scaling)=" << x << "\n";
	y *= scaling;
	cout << "y(scaling)=" << y << "\n";

    //calculate camera-grid corners
	this->upLeft = Vector(x, y, 1);
	cout << "this->upLeft(x,y,z)=" << this->upLeft.x << ","<< this->upLeft.y << "," << this->upLeft.z << "\n";
	this->upRight = Vector(-x, y, 1);
	cout << "this->upRight(x,y,z)=" << this->upRight.x << ","<< this->upRight.y << "," << this->upRight.z << "\n";
	this->downLeft = Vector(x, -y, 1);
	cout << "this->downLeft(x,y,z)=" << this->downLeft.x << ","<< this->downLeft.y << "," << this->downLeft.z << "\n";


    //rotate camera-grid to the desired location
	Matrix rotation = rotationAroundZ(toRadians(roll))
	                * rotationAroundX(toRadians(pitch))
	                * rotationAroundY(toRadians(yaw));
	upLeft *= rotation;
	cout << "upLeft(rotation)(x,y,z)=" << upLeft.x << ","<< upLeft.y << "," << upLeft.z << "\n";
	upRight *= rotation;
	cout << "upRight(rotation)(x,y,z)=" << upRight.x << ","<< upRight.y << "," << upRight.z << "\n";
	downLeft *= rotation;
	cout << "downLeft(rotation)(x,y,z)=" << downLeft.x << ","<< downLeft.y << "," << downLeft.z << "\n";
}

Ray Camera::getScreenRay(double x, double y)
{
	Ray result; // A, B -     C = A + (B - A) * x
	result.start = this->pos;
	Vector target0 = upLeft +
		(upRight - upLeft) * (x / (double) frameWidth()) +
		(downLeft - upLeft) * (y / (double) frameHeight());
	Vector target = target0 + this->pos;


	// A - camera; B = target
	result.dir = target - result.start;

	result.dir.normalize();
	return result;
}


//void Camera::beginFrame(void)
//{
//	//initial camera-grid parameters
//	double x = -aspect;
//	cout << "x=" << x << "\n";
//	double y = +1;
//	cout << "y=" << y << "\n";
//
//	Vector corner = Vector(x, y, 1); //the up-left corner of the camera-grid with depth=1
//	cout << "corner(x,y,z)=" << corner.x << "," << corner.y << "," << corner.z << "\n";
//
//	Vector center = Vector(0, 0, 1); //the center point of the camera-grid with depth=1
//	cout << "center(x,y,z)=" << center.x << "," << center.y << "," << center.z << "\n";
//
//	double lenXY = (corner - center).length(); //the distance between corner and the center of the grid
//	cout << "lenXY=" << lenXY << "\n";
//
//	double wantedLength = tan(toRadians(fov / 2));
//	cout << "fov=" << fov << "\n";
//	cout << "wantedLength=" << wantedLength << "\n";
//
//	double scaling = wantedLength / lenXY;
//	cout << "scaling=" << scaling << "\n";
//
//	//rescale x,y vectors to get the desired grid-size
//	x *= scaling;
//	cout << "x(scaling)=" << x << "\n";
//	y *= scaling;
//	cout << "y(scaling)=" << y << "\n";
//
//	//calculate camera-grid corners
//	this->upLeft = Vector(x, y, 1);
//	cout << "this->upLeft(x,y,z)=" << this->upLeft.x << "," << this->upLeft.y << "," << this->upLeft.z << "\n";
//	this->upRight = Vector(-x, y, 1);
//	cout << "this->upRight(x,y,z)=" << this->upRight.x << "," << this->upRight.y << "," << this->upRight.z << "\n";
//	this->downLeft = Vector(x, -y, 1);
//	cout << "this->downLeft(x,y,z)=" << this->downLeft.x << "," << this->downLeft.y << "," << this->downLeft.z << "\n";
//
//
//	//rotate camera-grid to the desired location
//	Matrix rotation = rotationAroundZ(toRadians(roll))
//		* rotationAroundX(toRadians(pitch))
//		* rotationAroundY(toRadians(yaw));
//	upLeft *= rotation;
//	cout << "upLeft(rotation)(x,y,z)=" << upLeft.x << "," << upLeft.y << "," << upLeft.z << "\n";
//	upRight *= rotation;
//	cout << "upRight(rotation)(x,y,z)=" << upRight.x << "," << upRight.y << "," << upRight.z << "\n";
//	downLeft *= rotation;
//	cout << "downLeft(rotation)(x,y,z)=" << downLeft.x << "," << downLeft.y << "," << downLeft.z << "\n";
//
//	//move the camera to the desired position
//	upLeft += pos;
//	cout << "upLeft(move)(x,y,z)=" << upLeft.x << "," << upLeft.y << "," << upLeft.z << "\n";
//	upRight += pos;
//	cout << "upRight(move)(x,y,z)=" << upRight.x << "," << upRight.y << "," << upRight.z << "\n";
//	downLeft += pos;
//	cout << "downLeft(move)(x,y,z)=" << downLeft.x << "," << downLeft.y << "," << downLeft.z << "\n";
//}

//Ray Camera::getScreenRay(double x, double y)
//{
//	Ray result; // A, B -     C = A + (B - A) * x
//	result.start = this->pos;
//	Vector target = upLeft +
//		(upRight - upLeft) * (x / (double)frameWidth()) +
//		(downLeft - upLeft) * (y / (double)frameHeight());
//
//
//	// A - camera; B = target
//	result.dir = target - this->pos;
//
//	result.dir.normalize();
//	return result;
//}
