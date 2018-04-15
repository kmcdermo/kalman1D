# Kalman Filter 1D tracking
Private code developed for pedagogical purposes for use of the Kalman filter (KF) in simple tracking applications.  The Kalman filter is ubiqitious in High Energy Physics (HEP) for the use of measuring the trajectory of particles moving through detectors.  In the context of HEP, the KF provides position and momentum estimates along the path of the particle. 

I would be remiss if I did not include the seminal paper on the use of the KF in HEP, by Rudi Fruhwirth: https://inspirehep.net/record/259509?ln=en.  The paper itself is fairly dense, so I have included a small set of resources that went into implementing this code, as well extra references on the KF itself.

The state model in this code is particle moving in 1D, and here the KF provides an estimator for particle position and velocity.
An intuitive example and explanation of the Kalman filter specific to this model is derived here: 
http://128.232.0.20/~rmf25/papers/Understanding%20the%20Basis%20of%20the%20Kalman%20Filter.pdf

I in fact use the same state vector and transition matrix as the intuitive derivation. The first set in F_k-1 is the top row, the second set is the bottom row.  
x_k   =  {position, velocity}

F_k-1 =  {{1, delT} , {0, 1}}

It is important to keep in mind that the KF is recursive.  So when it is actually implemented, it needs an input of the form of some initial guess of the KF state and covariance.  As such, from the paper above the state and covariance of x_k-1 and C_k-1 must first be provided before performing the KF propagation and update.  

In the case of tracking a rail car, we can say that it started at the origin with max velocity, and we assign a large uncertainty to the each value.  After the first KF propagation + update, this initial guess will be weighted down.  The output of the first propagation + update, x_k and C_k now become x_k-1 and C_k-1 in the next propagation + update.  

The above paper does not include two important aspects of extensions of the Kalman filter: non-linear state model extensions of the KF and Kalman smoothing.  As this code is for the simple example of a particle moving in 1D, the state model is linear and will not make use of the non-linear extension.  It is important to note, though, that in HEP, typically particles are subjected to a solenoidal magnetic field, which yields the non-linear helix equations.  Therefore, in HEP, one would then utilize the Extended Kalman Filter (EKF).  

Kalman smoothing, however, is applicable to this example. Smoothing allows us to reuse all the information gained by the forward propagation that the inner layers did not have access to, as a weighted average. So by definition, the first smoothed state is in fact the last filtered state, as this layer has already acquired all information possible about the system from measurement updates. Then the second smoothed state is the second-to-last filtered state.  The smoothing equations themselves are bit more opaque, although a light explanation is here in addition to more info on how the KF is used in HEP: https://www.dropbox.com/s/ofxkgan8fpqp8eo/glast_kf_for_physicists.pdf?dl=0

We can make use of equations 10, 11 in 16-18. This yields the following:

* A_k = C_k * F_k^T * (F_k * C_k * F_k^T  + Q_k)^-1
* x_k,smooth = x_k + A_k * (x_k+1,smooth - F_k * x_k) 
* C_k,smooth = C_k + A_k * (C_k+1,smooth - F_k * C_k * F_k^T + Q_k)

As shown in equation 19, A_k in the presence of no process noise (i.e. the noise that can kink the track in its propagation) reduces to the back propagator: (F_k)^-1. From the transition matrix defined above:

(F_k)^-1 =  {{1, -delT} , {0, 1}}

which then takes a state from x_k+1 to x_k (backwards in time)!  

When actually implementing, we start smoothing on the second to last filtered state, which is labeled k. By definition, the initial C_k+1,smooth x_k+1,smooth are the filtered state and covariance at the last layer (as explained above).  x_k and C_k are then the filtered state and covariance on the layer we are smoothing (so you have to save this information from the forward propagation + update!).  At the start of smoothing, then, x_k and C_k are the filtered values on the second-to-last layer.  After smoothing the second-to-last layer, x_k,smooth and C_k,smooth become C_k+1,smooth and x_k+1,smooth for the next smoothing step for the third-to-last layer, and x_k and C_k are now the filtered state and covariance for the third-to-last layer. 

The smoothing proceeeds over all layers. We can actually perform a smoothing to the production point of the track. This means that after smoothing the innermost layer, the filtered state x_k is the initial guess we made about the production point. 

Additional resources:
* A nice interactive tutorial of KF with a plane landing: http://home.wlu.edu/~levys/kalman_tutorial/
* Wikipedia: https://en.wikipedia.org/wiki/Kalman_filter
* Blog: http://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/
* Blog: http://bilgin.esme.org/BitsAndBytes/KalmanFilterforDummies

More technical resources:
* A little more rigorous derivation of KF with linear algebra: http://web.mit.edu/kirtley/kirtley/binlustuff/literature/control/Kalman%20filter.pdf
* Derivations of KF as the optimal linear estimator: http://webee.technion.ac.il/people/shimkin/Estimation09/ch4_KFderiv.pdf
* A little more mathy derivation: http://www.aticourses.com/kalman_filter.pdf
* Another mathy derivation: https://users.ece.cmu.edu/~byronyu/papers/derive_ks.pdf

## How to run the code

This code requires that ROOT6 is enabled on your machine.  It compiles with gcc; so on Mac, this will pick up clang.  This code also contains a port of SMatrix, a vectorizable template matrix operations library.  The SMatrix port clocks in at roughly 10k lines of code (under the Math directory)! 

ROOT can be downloaded here: https://root.cern.ch/downloading-root.
The nice thing about the PRO releases (recommended) is that they contain binaries for given OS's.  For Mac, you can simply download the .dmg file for ROOT6 and run ROOT out-of-the-box after setting the path.

To run this code out-of-the-box (with ROOT previously installed), do the following:

1. Compile with gcc: make main -j 4
2. Run the executable: ./main
3. Run validation: ./scripts/runValidation.sh
4. View the output example pngs in the output directory ouput/: x_pull_filter.png, vx_pull_filter.png, x_pull_smooth.png, vx_pull_smooth.png, track.png

The executable (main) has a number of command line options.  Use ./main --help to view them.  Feel free to mess with the filtering parameters in Config.hh through the command line!  In particular, messing with the number of hits, the associated size of the parameters and variances, and also the fudge factors, help reveal the strength of the KF when tweaking these parameters.  Out-of-the-box running uses an estimate of the vertex position and velocity off a simple fit to a line.  It also performs the smoothing immediately following filtering.
