# Kalman Filter 1D tracking
Private code developed for pedagogical purposes for use of the Kalman filter in simple tracking applications.  The Kalman filter is ubiqitious in High Energy Physics for the use measuring the trajectory of particles moving through detectors.  In the context of HEP, the KF provides position and momentum estimates along the path of the particle. 

I would be remiss if I did not include the seminal paper on the use of the KF in HEP, by Rudi Fruhwirth: https://inspirehep.net/record/259509?ln=en
The paper itself is fairly dense, so I have included a set of some resources that went into implemented this code.

The state model in this code is particle moving in 1D, and here the KF provides an estimator for particle position and velocity.
An intuitive example and explanation of the Kalman filter specific to this model is derived here: 
http://128.232.0.20/~rmf25/papers/Understanding%20the%20Basis%20of%20the%20Kalman%20Filter.pdf

It is important to keep in mind that the KF is recursive.  So when it is actually implemented, it needs an input of the form of some initial guess of the KF state and covariance.  As such, from the paper above the state and covariance of x_k-1 and C_k-1 must first be provided before performing the KF propagation and update.  

In the case of tracking a rail car, we can say that it started at the origin with max velocity, and we assign a large uncertainty to the each value.  After the first KF propagation + update, this initial guess will be weighted down.  The output of the first propagation + update, x_k and C_k now become x_k-1 and C_k-1 in the next propagation + update.  

The above paper does include two inmportant aspects of extensions of the Kalman filter: non-linear state model extensions of the KF and Kalman smoothing.  As this code is for the simple example of a particle moving in 1D, the state model is linear and will not make use of the non-linear extension.  It is important to note, though, that in HEP, typically particles are subjected to a solenoidal magnetic field, which yields the non-linear helix equations.  Therefore, in HEP, one would then utilize the Extended Kalman Filter (EKF).  

Kalman smoothing, however, is applicable to this example. The mmoothing equations are bit more opaque, although a light explanation is here in addition to more info on how the KF is used in HEP:
http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.57.1034&rep=rep1&type=pdf

We can make use of equations 10, 11 in 16-18. This yields the following:

A_k = C_k * F_k^T * (F_k * C_k * F_k^T  + Q_k)^-1 [eq.16]
x_k,smooth = x_k + A_k * (x_k+1,smooth - F_k * x_k) [eq.17] 
C_k,smooth = C_k + A_k * (C_k+1,smooth - F_k * C_k * F_k^T + Q_k) [eq.18]

When actually implementing, we start on the last filtered state, which is now label k+1.  We then move from k+1 to k. When starting at the last layer, we then end up on the second-to-last layer.

So by definition, the initial C_k+1,smooth x_k+1,smooth are the filtered state and covariance at the last layer.  In other words, the filtered state is equal to the smooth state only for the last layer we ended up on after the forward filtering.  x_k and C_k are the filtered state and covariance on the layer we are moving to (so you have to save this information from the forward propagation + update!).  At the start of smoothing, then, x_k and C_k are the filtered values on the second-to-last layer.  After smoothing the second-to-last layer, x_k,smooth and C_k,smooth become C_k+1,smooth and x_k+1,smooth for the next iteration.


##How to run the code

This code requires ROOT is enabled on your machine.  It compiles with gcc; so on Mac, this will pick up clang.  This code also contains a port of SMatrix, a vectorizable template matrix operations library.  The SMatrix clocks in at roughly 10k lines of code! 

To run this code out-of-the-box, do the following:

1) Compile with gcc: make main -j 2

2) Run the executable: ./main

3) Run validation: root -l -b -q runValidation.C

4) View the output example pngs in the output director validation/: x_pull.png, vx_pull.png, track.png

The executable (main) has a number of command line options.  Use ./main --help to view them.  Feel free to mess with the set parameters in Config.h!  Out-of-the-box running use MC generated state as input to KF and has no smoothing after filtering.
