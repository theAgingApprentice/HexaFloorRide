<h1>Kinematic Model</h1>

We use our own custom inverse kinematic (ik) model to manipulate the legs of HexaFloorRide robot. Figure 1 and 2 below provide the dimentions used in our model. An explanation of our approach to the ik model problem is on our "to do" list  and will appear here when ready. 

<figure>
  <img src="https://github.com/theAgingApprentice/HexaFloorRide/blob/main/physicalDesign/fusionExports/hfrTopLegDimensions.png" alt="Model" style="width:100%">
  
  <figcaption>Fig.1 - HexaFloorRide top leg dimension view.</figcaption>
</figure> 

<figure>
  <img src="./physicalDesign/fusionExports/hfrFrontLegDimentions.png" alt="Model" style="width:100%">
  <figcaption>Fig.2 - HexaFloorRide front leg dimension view.</figcaption>
</figure> 

<h1>Reference Material</h1> 
This 
<a href="https://www.mathworks.com/help/fuzzy/modeling-inverse-kinematics-in-a-robotic-arm.html">matlab article</a>
discusses a fuzzy logic approach to manipulating a leg. 
<p>
There is an inverted kinematic Ardiuno library called <a href="https://github.com/cgxeiji/CGx-InverseK">CGx-InverseK</a> that could be used to manipulate your robot. We have elected to use our own logic. 

