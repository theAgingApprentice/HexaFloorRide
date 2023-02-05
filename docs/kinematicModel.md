<h1>Kinematic Model</h1>

We use our own custom inverse kinematic (ik) model to manipulate the legs of HexaFloorRide robot. This page explains our approach to inverse kinematics.  

## Definition of Kinematics

* The branch of mechanics concerned with the motion of objects without reference to the forces which cause the motion.
* Kinematics is the study of the motion of mechanical points, bodies and systems without consideration of their associated physical properties and the forces acting on them. The study is often referred to as the geometry of motion, and it models these motions mathematically using algebra.

In our context, it concerns the control functions for the 3 servo motors that control leg positioning, and the resulting position of the end point of the leg, which we call the toe.

## Two basic tasks in kinematics:

Forward Kinematics, where the settings for your position controls are known, and you want to calculate the resulting position of the endpoint. In our case the positions of the 3 servos for a given leg are known, and we want to figure out where the toe will end up. This is relatively easy, and consists of apply basic trigonometry to the displacement angles of the servos, and the fixed lengths of the interconnecting body parts

Inverse Kinematics, where the desired location of the end point is known, and you need to work backwards (thus "inverse") to determine what position control settings are needed to achieve this positioning. For us this would mean that the toe position is known, and we want to figure out the angle each servo should be set to in order for the toe to end up in the desired position. In general, this is a more challenging mathematical task, and in some designs, can only be done by iterative methods. In our case, we are able produce the required angles analytically using 3D analytic geometry. This is fortunate, because doing complex iterative calculations for 6 legs at least 20 times a second might not be feasible.

## The mathematical model of a leg

* origin above hip
* mechanical coupling is equivalent to 3 line segments with connections that rotate around a fixed axis.
* one end is origin, other is toe (round toe is ignored in this discussion)

## Real world constraints

* servos can only rotate +/- 90 degrees
* don't want legs to collide with robot body
* don't want legs to collide with each other
* changes in motion should be smoothed, and not abrupt
* robot movements cause momentum which can affect subsequent movement
* need at least 3 legs on floor for static balancing
* the available torque in the servos may not be enough for some manoeuvres

## Coordinate systems

* global frame of reference: X,Y, & Z
* local leg frame of reference: S, T, U ?
* converting between coordinate systems
* maintaining position information in both systems

## Body parts, sizes and connectivity

* relative locations
* axis rotations, which way is positive
* leg part names and lengths
* overhead view of full mathematical model

## Applied Forward Kinematics

* go through the trig to calculate leg position, one component at a time, with diagrams

## Applied Inverse Kinematics

* use swinging door analogy to reduce problem to 2 dimensions
* explain how there are circles of possible positions centered on toe and from knee
* in general, these circles intersect in 2 places, but only one satisfies real world constraints above
* start from basic equation for a circle
* derive equations for the 2 circles at toe and knee
* derive the equation for the 2 intersection points of these circles
* how to select the right one of the 2 possible locations for the ankle?
* use resulting coordinates for ankle to calculate angles for all servos

## Dangers:

* a mathematical discontinuity that gets in the way of continuous smooth motion
* servo positions that are mathematically valid, but don't obey real world constraints above, possible causing damage
* bugs in complex math coding that are hard to detect, and hard to fix

## Dimensions

Figure 1 and 2 below provide the dimentions used in our model.

<figure>
  <img src="/physicalDesign/fusionExports/hfrTopLegDimensions.png" alt="Model" style="width:100%">
  
  <figcaption>Fig.1 - HexaFloorRide top leg dimension view.</figcaption>
</figure> 

<figure>
  <img src="/physicalDesign/fusionExports/hfrFrontLegDimentions.png" alt="Model" style="width:100%">
  <figcaption>Fig.2 - HexaFloorRide front leg dimension view.</figcaption>
</figure> 

<h1>Reference Material</h1> 
<ul>
  <li>
    This 
<a href="https://www.mathworks.com/help/fuzzy/modeling-inverse-kinematics-in-a-robotic-arm.html">matlab article</a>
discusses a fuzzy logic approach to manipulating a leg. 
  </li>
  <li>
  There is an inverted kinematic Ardiuno library called <a href="https://github.com/cgxeiji/CGx-InverseK">CGx-InverseK</a> that could be used to manipulate your robot. We have elected to use our own logic. 
  </li>
</ul>
