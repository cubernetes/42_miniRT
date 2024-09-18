# Simple Raytracer in C
# How to Build & Run (Linux, X11, cc)
```sh
make -j re
./miniRT <.rt scene file>
```
# Show possible targets
```sh
make h
```
# Interactive Controls
## Menu Mode
- Left Click - Enter Camera Mode
- Q or Escape - Exit
## Camera Mode
- W, A, S, D - Movement on the X-Z plane (view-direction dependent)
- Space, L-Shift - Movement along vertical (Y) axis
- Adding L-Control will make you go 4x faster
- F - Increase FOV
- LCTRL-F - Decrease FOV
- Left Click - If pointing at an object, select it and enter Object Mode
- Mouse Movement - Orient the camera
- Q - Enter Menu Mode
- Escape - Exit
## Object Mode
- W, A, S, D - Move the select object on the X-Z plane (direction independent)
- Space, L-Shift - Move object along the vertical (Y) axis
- Adding L-Control will move the object 4x faster
- F - Increase FOV
- LCTRL-F - Decrease FOV
- Left Click - If pointing at an object, select it and stay in Object Mode
- Mouse Movement - Orient the camera
- Q - Enter Camera Mode
- Escape - Exit

# Notes
- The resolution during movement is adaptive, but it only updates on a per second basis. This means you have to wait some seconds until the resolution reaches point where 30fps can be maintained.
- The ability to manipulate objects at runtime, or even to move around, was not a requirement for the completion of the assignment. Bugs are therefore to be expected.
