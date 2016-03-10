# kalman1D
Private code developed for pedagogical purposes for use of Kalman filter in simple tracking applications.
Equations and example taken directly from: 
http://128.232.0.20/~rmf25/papers/Understanding%20the%20Basis%20of%20the%20Kalman%20Filter.pdf

This code requires ROOT is enabled on your machine.  It compiles with gcc; so on Mac, this will pick up clang.  

To run this code, do the following:

1) Compile with gcc: make main -j 2

2) Run the executable: ./main

3) Run validation: root -l -b -q runValidation.C

4) View the output example pngs: x_pull.png, vx_pull.png, track.png
