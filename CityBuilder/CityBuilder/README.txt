README

Alessandro Profenna
CPS511 - Assignment 2
City Builder

This program was developed on a Mac using Xcode in C++.
There are two ways to compile and run this program on a Mac:

1) Open a new C++ Xcode project and place all of the files in the project folder. Run it.
2) There is a makefile for this project. To compile with it, cd into the project directory and
run 'make' -> this will compile all of the necessary files and generate an 'assignment2' executable. 
Run './assignment2' to run the program and control-c to terminate it.

NOTE: There are deprecation compiler warnings because of GLUT, but these can be ignored.
NOTE: Although there is code in the makefile for compiling on Windows, it has not been tested.
Therefore this assignment should be compiled, run, and marked on a Mac. If the TA does not have access
to a Mac, I can showcase the assignment running on my Mac to the TA at a designated time.

_____________________________________________


Project Structure:

The new files written by me are:

Vector3D.cpp
Vector3D.hpp

Polygon.cpp 
Polygon.hpp

PrismMesh.cpp (Intead of using cubeMesh.cpp)
PrismMesh.hpp

Building.cpp
Building.hpp

Camera.cpp
Camera.hpp


Additionally, there is a Spline.cpp file that I did not write myself. The credits are in the comments at the top of the file. I asked Dr. McInerney for approval and he said it was okay to use. Basically I use it to generate a 3D cubic spline function. I integrated this with a spline control point interface that can be altered by the user. Each floor is used as input to the spline function to determine their xz-plane scales.

The program runs from main.cpp, which I modified in multiple places to add controls, change materials, initialize the buildings, etc.



_____________________________________________


Fulfilled Requirements:

All required sections are complete.
Bonus b) was also implemented (worth 1.5 marks).

_____________________________________________


***How to use this program:***

The program is split into three viewports:

1) World Viewport: on the left, which is the view of the city
2) Base Viewport: on the top right, which shows the base of the currently active building, whose xz cross-sectional shape 
can be modified by pulling and dragging on the control points. They can only be dragged diagonally from the centre of the base.
3) Spline Viewport: on the bottom right, which shows the poly line representing the vertical side of the currently active building. 
By dragging the control points left/right, the user can alter the scale of each floor, thus changing the vertical profile of the building.
This creates smooth, curvy looking buildings.

Creating buildings:
Pressing the 'g' key creates a default building. It will be the active building since it's the one the user is currently editing.
The active building has a redder/pink colour tone, while all other buildings and roads have a blue/grey colour tone.
If 'g' is pressed again, a new building is generated and the "current active building' changes to it.

The 'active building' can:

1) Be scaled in the x direction
2) Be scaled in the z direction
3) Be scaled in the y direction
4) Be rotated around the y-axis
5) Be moved forward/backward, left/right with respect to the direction the camera is facing
6) Have the number of sides changed dynamically, the default is 4, minimum is 3
7) Have its base shape be changed using the control points in the top right viewport
8) Have its floors scaled by a spline function using the control points in the bottom right viewport

Creating roads:
Use the same functionality for creating a building, just scale it down until it's flat and scale it out until it's long

Saving/loading:
All buildings and roads are saved by pressing the "l" key to a text file called CityMetaData.txt
All buildings and roads can be loaded from the filename assigned to the CityMetaDataFile variable at the top main.cpp. Currently that file is called "CityMetaData.txt" and has a default city loaded. It is duplicated in ExampleCity1.txt since "CityMetaData.txt" can be overwritten.
Loading can be done at any time, even into a new instance to a program. This will not conflict with any 
buildings that are already generated in the scene or buildings that are generated after. 

Camera:
The camera can be moved along the hemispheric dome of the city, and always points to the origin. It can zoom in and out.
It can be controlled either by clicking and dragging the mouse across the screen or using the u,h,j,k,i,o keys.


Here is the full list of controls:

-----------------------------------------------------
Generate new building:     	g key
(Becomes the active building)

Move building forward:		w key
Move building backward:		s key
Move building left:		a key
Move building right:		d key

Increase building height:	y key
Decrease building height:	t key
Decrease building size in x:	left arrow key
Increase building size in x:	right arrow key
Increase building size in z:	up arrow key
Decrease building size in z:	left arrow key
		
Rotate building cw		n key
Rotate building ccw		m key

Move camera			click and drag on world viewport
OR
Change camera azimuth cw	h key
Change camera azimuth ccw	k key
Change camera elevation up	u key
Change camera elevation down	j key
Zoom in				i key
Zoom out			o key

Increase number of sides        z key
Decrease number of sides        x key

Save city data			l key
Load city data			v key

Change base control points	click and drag points in base viewport
Change spline control points	click and drag points in spline viewport

Quit the program		q key	
-----------------------------------------------------
	



NOTE: These controls are printed to stdout when the 'f' key is pressed while the program is running.


Also, the project folder includes screenshots of what the program should look like when it is running.