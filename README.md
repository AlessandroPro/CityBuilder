# City Builder
An OpenGL C++ program that allows the user to create buildings and manipulate their form using scaling and extrusions.

<img width="1049" alt="cityscreenshot5" src="https://user-images.githubusercontent.com/15040875/50877259-1ab04c00-139f-11e9-9560-b868b95612af.png">

This program was developed on a Mac using Xcode in C++.
There are two ways to compile and run this program on a Mac:

1) Open a new C++ Xcode project and place all of the files in the project folder. Run it.
2) There is a makefile for this project. To compile with it, cd into the project directory and
run 'make' -> this will compile all of the necessary files and generate an 'assignment2' executable. 
Run './assignment2' to run the program and control-c to terminate it.

NOTE: There are deprecation compiler warnings because of GLUT, but these can be ignored.
NOTE: Although there is code in the makefile for compiling on Windows, it has not been tested.

_____________________________________________

Project Structure:

The new files written by me are:

Vector3D.cpp <br>
Vector3D.hpp 

Polygon.cpp <br>
Polygon.hpp

PrismMesh.cpp <br>
PrismMesh.hpp

Building.cpp <br>
Building.hpp

Camera.cpp <br>
Camera.hpp


Additionally, there is a Spline.cpp file that I did not write myself. The credits are in the comments at the top of the file. 

The program runs from main.cpp, which I modified in multiple places to add controls, change materials, initialize the buildings, etc.

_____________________________________________

### **How to use this program:**

The program is split into three viewports:

1) **World Viewport:** on the left, which is the view of the city <br>
2) **Base Viewport:** on the top right, which shows the base of the currently active building, whose xz cross-sectional shape
can be modified by pulling and dragging on the control points. They can only be dragged diagonally from the centre of the base. <br>
3) **Spline Viewport:** on the bottom right, which shows the poly line representing the vertical side of the currently active building. <br>
By dragging the control points left/right, the user can alter the scale of each floor, thus changing the vertical profile of the building.
This creates smooth, curvy looking buildings.

**Creating buildings:** <br>
Pressing the 'g' key creates a default building. It will be the active building since it's the one the user is currently editing. <br>
The active building has a redder/pink colour tone, while all other buildings and roads have a blue/grey colour tone. <br>
If 'g' is pressed again, a new building is generated and the "current active building' changes to it. <br>

The 'active building' can:

1) Be scaled in the x direction <br>
2) Be scaled in the z direction <br>
3) Be scaled in the y direction <br>
4) Be rotated around the y-axis <br>
5) Be moved forward/backward, left/right with respect to the direction the camera is facing <br>
6) Have the number of sides changed dynamically, the default is 4, minimum is 3 <br>
7) Have its base shape be changed using the control points in the top right viewport <br>
8) Have its floors scaled by a spline function using the control points in the bottom right viewport <br>

**Creating roads:** <br>
Use the same functionality for creating a building, just scale it down until it's flat and scale it out until it's long

**Saving/loading:** <br>
All buildings and roads are saved by pressing the "l" key to a text file called CityMetaData.txt <br>
All buildings and roads can be loaded from the filename assigned to the CityMetaDataFile variable at the top main.cpp. Currently that file is called "CityMetaData.txt" and has a default city loaded. It is duplicated in ExampleCity1.txt since "CityMetaData.txt" can be overwritten.
Loading can be done at any time, even into a new instance to a program. This will not conflict with any 
buildings that are already generated in the scene or buildings that are generated after. 

**Camera:** <br>
The camera can be moved along the hemispheric dome of the city, and always points to the origin. It can zoom in and out. <br>
It can be controlled either by clicking and dragging the mouse across the screen or using the u,h,j,k,i,o keys.


Here is the full list of controls:

-----------------------------------------------------

Action | Control
------------ | -------------
Generate new building <br> (Becomes the active building) |     	g key
<br> | <br>
Move building forward |		w key
Move building backward |		s key
Move building left |		a key
Move building right |		d key
<br> | <br>
Increase building height |	y key
Decrease building height|	t key
Decrease building size in x |	left arrow key
Increase building size in x |	right arrow key
Increase building size in z |	up arrow key
Decrease building size in z |	left arrow key
<br> | <br>
Rotate building cw |		n key
Rotate building ccw |		m key
<br> | <br>
Move camera|			click and drag on world viewport
OR|<br>
Change camera azimuth cw |	h key
Change camera azimuth ccw |	k key
Change camera elevation up |	u key
Change camera elevation down |	j key
Zoom in |				i key
Zoom out |			o key
<br> | <br>
Increase number of sides |        z key
Decrease number of sides |       x key
<br> | <br>
Save city data |			l key
Load city data |			v key
<br> | <br>
Change base control points |	click and drag points in base viewport
Change spline control points |	click and drag points in spline viewport
<br> | <br>
Quit the program |		q key	

-----------------------------------------------------
	
NOTE: These controls are printed to stdout when the 'f' key is pressed while the program is running.

Here are additional screenshots:

<img width="1048" alt="buildingscreenshot1" src="https://user-images.githubusercontent.com/15040875/50877253-1a17b580-139f-11e9-962c-a402e1ffedd4.png">

<img width="1049" alt="buildingscreenshot2" src="https://user-images.githubusercontent.com/15040875/50877254-1a17b580-139f-11e9-9524-5c91d8e64bef.png">

<img width="1050" alt="cityscreenshot1" src="https://user-images.githubusercontent.com/15040875/50877255-1a17b580-139f-11e9-91e3-aa6bbfc700fd.png">

<img width="1050" alt="cityscreenshot2" src="https://user-images.githubusercontent.com/15040875/50877256-1ab04c00-139f-11e9-8d8f-921db7dea0ab.png">

<img width="1049" alt="cityscreenshot3" src="https://user-images.githubusercontent.com/15040875/50877257-1ab04c00-139f-11e9-9efd-1899b825833e.png">

<img width="1049" alt="cityscreenshot4" src="https://user-images.githubusercontent.com/15040875/50877258-1ab04c00-139f-11e9-892e-389423cf7571.png">

