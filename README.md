# 3D-Virtual-Roaming-System
A comprehensive demonstration project of 3D graphics, which covers 3D graphics observation and transformation, realistic display, lighting and texture and other parts of realistic graphics display algorithm.

### 1 Basic Information:
#### 1.1 Scene:
A cube Sky Box is used to form the background to enhance the reality, and switchable between four scenes.
White balls are drawn by a simple particle system to simulate snowy weather; specifically, scene's bottom size is 1024x1024, with 16x16 as a unit grid; the total number of particles generated is 500, and controled not be generated in the housing area.

#### 1.2 Roaming:
Users interact with the mouse and keyboard. W and S keys are used to control the movement of Z axis of the camera, A and D keys are used to control the movement of X axis. When the user drags the mouse, the offset between the mouse position and the center of the screen is mapped to the rotation of the camera's observation angle.

#### 1.3 Objects:
The walls and the bottom of the house are quadrilateral, and the roof is surrounded by four triangles; there is a wooden table at the entrance of the house with a Brass Teapot and a crystal ball with environmental texture effect; two bookshelves are on the left side of the house and three rings on the right side of the table for demonstration of different materials and lighting effects, which can be photographed in two sets of materials. The light source of all objects in the scene comes from a point light source, positioned directly above the camera's initial left side (512, 512).

#### 1.4 Operation:
Roaming operation is controlled by the mouse keyboard, as described above; other functions are as follows: F1~F4 is used to switch the background of four sets of sky boxes; G key is used to open and close the ground grid; M key is used to switch the material of the circle in the house.

### 2 Design Details:
#### 2.1 Class:
Camera class controls the operation of camera and realizes the core part of roaming function; CBMPLoader class is used to load BMP bitmap files to prepare for adding texture; House class is responsible for drawing the main parts of the scene, including houses and indoor objects; material class defines the parameters of different materials; particle class is used to achieve particle effects and simulate snow weather; skybox class is responsible for creating. Build Sky Box; Vector class realizes the operation of three-dimensional vectors, which provides the basis for vector operation in camera class.

#### 2.2 Texture:
For quadrilateral, triangle and other objects, the loaded BMP images can be directly mapped as texture; for quadratic geometry (sphere, cylinder, cone, etc.), it is necessary to create quadratic geometry GLUquadric Obj*quadratic first, and then OPENGL will use built-in rules to map texture binding. In addition, for the environment texture, we need to use the function glCopyTex Image 2D to copy the content of the screen from the frame buffer to memory, and then bind it to the object.

#### 2.3 Particle:
For the snowflake particle effect, the radius of the initial sphere is 0.4-0.55, the acceleration of gravity is 0.01-0.06, and the initial velocity is 0.1-0.6. After the random generation of the full map position (except for the house), the particles will fall faster according to the above attributes until the height y is less than 0, and then the particles will be regenerated randomly. If you need to modify the color and shape of snowflakes, just modify the createDL function in the particle class.

### Samples:
<img src="https://github.com/xqzlgy2/3D-Virtual-Roaming-System/blob/master/Pictures/sample1.png"  width="500" height="300">
<img src="https://github.com/xqzlgy2/3D-Virtual-Roaming-System/blob/master/Pictures/sample2.png"  width="500" height="300">
<img src="https://github.com/xqzlgy2/3D-Virtual-Roaming-System/blob/master/Pictures/sample3.png"  width="500" height="300">
<img src="https://github.com/xqzlgy2/3D-Virtual-Roaming-System/blob/master/Pictures/sample4.png"  width="500" height="300">
<img src="https://github.com/xqzlgy2/3D-Virtual-Roaming-System/blob/master/Pictures/sample5.png"  width="500" height="300">
