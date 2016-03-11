# Kalman Filter 1D tracking
Private code developed for pedagogical purposes for use of Kalman filter in simple tracking applications.  
State model is particle moving in 1D -- KF provides an estimator for particle position and velocity.
Equations and example taken directly from: 
http://128.232.0.20/~rmf25/papers/Understanding%20the%20Basis%20of%20the%20Kalman%20Filter.pdf
Smoothing equations are bit more opaque, light explanation is here:
http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.57.1034&rep=rep1&type=pdf

We can make use of equations 10, 11 in 16-18.  

This yields the following:



Initial C_k+1,smooth x_k+1,smooth are the filtered state and covariance at the last layer.  





#How to run the code


This code requires ROOT is enabled on your machine.  It compiles with gcc; so on Mac, this will pick up clang.  

To run this code out-of-the-box, do the following:

1) Compile with gcc: make main -j 2

2) Run the executable: ./main

3) Run validation: root -l -b -q runValidation.C

4) View the output example pngs: x_pull.png, vx_pull.png, track.png

The executable (main) has a number of command line options.  Use ./main --help to view them.  Feel free to mess with the set parameters in Config.h!
Out-of-the-box running use MC generated state as input to KF and has no smoothing after filtering.

