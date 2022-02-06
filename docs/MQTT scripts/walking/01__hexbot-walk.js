var Thread = Java.type("java.lang.Thread");

function execute() {
    out("hexbot-walk.js");
	
    myTWIPe = "Hexbot3C61054ADD98/commands"     // Doug's bot
//	myTWIPe = "calServoCC50E394F048/commands"     // Andrew's bot

// math background
// consider a step of size D to the robot's left, or +y directon in global coordinates
// for the corner legs, the local cordinate equivalent involves changes to both local x and local z
// Consider leg 4:
// The step is at a 45 degree angle to local x axis, so to achieve sideways motion D,
//   we need to add D*cos(45) to local x coord, and D*sin(45) to local Z coord
// cos(45) = sin(45) = .7071, so we get these x and y offsets:
// D = 1, X offset = .7071, y offset = .7071
// D = 2,           1.4142            1.4142
// D = 3,           2.1213            2.1213
// D = 4,           2.8284            2.8284
//
// for the other corners, the same offsets are used, with different signs
// for the middle legs, 2 and 5, the step can be added directly to the local axis that is aligned with step

    out(" ");
	send("fg,0,0")	// clear any previous flow
// go to the  home position
//0 since this is the first flow row, we jump to it directly, then pause for 340 msec
	send("fl,1000,4,10,0,0,0,    0,0,0,      0,0,0,    0,0,0,     0,0,0,    0,0,0,     0,0,0")
//1 make row 1 long enough (240 = 6 frames = 6 legs home positions) so chart will show home locations	
	send("fl, 240,4,10,0,0,0,    0,0,0,      0,0,0,    0,0,0,     0,0,0,    0,0,0,     0,0,0")
		
//===== ripple reposition each leg 4 cm closer to body =======================================
//2 using local coordinates & offsets, raise legs 1 and 6, which are opposites
	send("fl,240,4,10,0,0,0,     0,2,0,     0,0,0,    0,0,0,     0,0,0,    0,0,0,     0,2,0")
//3 put them back down, but 4 cm closer to the chassis (local X direction)
	send("fl,200,4,10,0,0,0,    -4,0,0,     0,0,0,    0,0,0,     0,0,0,    0,0,0,    -4,0,0")
//4 do same thing for opposite legs 2 and 5. up..
	send("fl,200,4,10,0,0,0,    -4,0,0,      0,2,0,   0,0,0,     0,0,0,    0,2,0,    -4,0,0")
//5  and down a bit closer to center of bot
	send("fl,200,4,10,0,0,0,    -4,0,0,     -4,0,0,   0,0,0,     0,0,0,   -4,0,0,    -4,0,0")
//6 and lastly do same for opposite legs 3 and 6. up...
	send("fl,200,4,10,0,0,0,    -4,0,0,     -4,0,0,    0,2,0,    0,2,0,   -4,0,0,    -4,0,0")
//7  and down
	send("fl,200,4,10,0,0,0,    -4,0,0,     -4,0,0,   -4,0,0,    -4,0,0,   -4,0,0,    -4,0,0")
//8 stay in that position, with legs displaced 4 cm "inward" for 1 second		
	send("fl,1000,4,10,0,0,0,    -4,0,0,     -4,0,0,   -4,0,0,    -4,0,0,   -4,0,0,   -4,0,0")
	
//====== walking gait ==========================================================================
//9 we're going to repeat this section, so mark the beginning, and call it cycle #6 for reference
	send("fl, 0, 9, 6, 0, 0, 0")

// use local offsets from the home position, to make the 4 cm offset easier to handle
// and use a 2 cm step to robot's left
// start with 
//   - odd legs down touching floor at modified home position 4 cm in (call it home4)	
//10   - even legs up in the air extended out to the end point of the step, to robot's left
	send("fl, 500, 4,10,0,0,0, -4,0,0, -2.59,2,0, -4,0,0, -2.59,2,1.41, -4,0,0, -2.59,2,-1.41 ")
//11 lower evens, and once they're supporting the bot, raise odds
    send("fl, 500, 4,10,0,0,0, -4,0,0, -2.59,0,0, -4,0,0, -2.59,0,1.41, -4,0,0, -2.59,0,-1.41 ")
//12 now it's safe to lift odd legs
    send("fl, 500, 4,10,0,0,0, -4,2,0, -2.59,0,0, -4,2,0, -2.59,0,1.41, -4,2,0, -2.59,0,-1.41 ")
// use even legs on ground to push floor to the left, moving bot to the right
//13 at same time, position odds up and extended to be ready for their next step
    send("fl, 500, 4,10,0,0,0, -5.41,2,1.41, -4,0,0, -5.41,2,-1.41, -4,0,0, -2.59,2,0, -4,0,0")
//14 lower odds, and once they're supporting the bot, raise evens
    send("fl, 500, 4,10,0,0,0, -5.41,0,1.41, -4,0,0, -5.41,0,-1.41, -4,0,0, -2.59,0,0, -4,0,0")
//15 now it's safe to lift the evens
    send("fl, 500, 4,10,0,0,0, -5.41,0,1.41, -4,2,0, -5.41,0,-1.41, -4,2,0, -2.59,0,0, -4,2,0")
// use odd legs on ground to push floor to the left, moving bot to the right
// at same time, position evens up and extended to be ready for their next step
   // above step is done at beginning of cycle if we're doing that.
//16 so mark end of cycle being here
   	send("fl, 0, 10, 6, 0, 0, 0")

//17 for now, we're not repeating the cycle we've defined, just shutting down	
    send("fl, 500, 4,10,0,0,0, -4,0,0, -2.59,2,0, -4,0,0, -2.59,2,1.41, -4,0,0, -2.59,2,-1.41")

//===== back to neutral, then back to belly ====================================================
//18 bring all legs back to home position, but pulled in 4 cm
	send("fl, 500,4,10,0,0,0,    -4, 0,0,     -4, 0,0,   -4, 0,0,    -4, 0,0,   -4, 0,0,    -4, 0,0")
//19 do it again, just to be sure	
	send("fl, 500,4,10,0,0,0,    -4, 0,0,     -4, 0,0,   -4, 0,0,    -4, 0,0,   -4, 0,0,    -4, 0,0")
//20 raise opposite legs 1 and 6 by 3 cm, and push the out to normal home position, undoing the 4 cm pull in	
	send("fl,200,4,10,0,0,0,     0,2,0,    -4,0,0,   -4,0,0,    -4,0,0,   -4,0,0,     0,2,0")
//21 then put legs 1 and 6 down in the true home position
	send("fl,200,4,10,0,0,0,     0,0,0,    -4,0,0,   -4,0,0,    -4,0,0,   -4,0,0,     0,0,0")
//22 do the same for legs 2 and 5. up...
	send("fl,200,4,10,0,0,0,     0,0,0,      0,2,0,  -4,0,0,    -4,0,0,    0,2,0,     0,0,0")
//23  ..and down
	send("fl,200,4,10,0,0,0,     0,0,0,      0,0,0,  -4,0,0,    -4,0,0,    0,0,0,     0,0,0")
//24 lastly, do the same for legs 3 and 4. up...
	send("fl,200,4,10,0,0,0,     0,0,0,      0,0,0,    0,2,0,    0,2,0,    0,0,0,     0,0,0")
//25  ..and down
	send("fl,200,4,10,0,0,0,     0,0,0,      0,0,0,    0,0,0,    0,0,0,    0,0,0,     0,0,0")
//26 and sit there for a bit		
	send("fl, 500,4,10,0,0,0,    0,0,0,      0,0,0,    0,0,0,    0,0,0,    0,0,0,     0,0,0")

//27 leave bot in belly-on-the-floor position, to ease next start up
	send("fl,1000,4,10,0,0,0,   0,3.0,0,   0,3.0,0,   0,3.0,0,   0,3.0,0,   0,3.0,0,   0,3.0,0,")
	
	// now that we've defined the flow, execute it
	send("fg,1,50,49")          // start er up, 50 msec per frame,
								// toeMoveAction:
								// 1 - move toes (altho I sometimes test with 5V power offset)
								// 16 - print global X and Y coords for Excel graphing
								// 32 - at flow startup, print a nicely formatted, numbered flow listing

    action.setExitCode(0);
    action.setResultText("done.");
    out("Parameter parameter set up complete");
    return action;
}
function send(cmd){
	out(cmd);
	mqttManager.publish(myTWIPe, cmd);
	Thread.sleep(200);  // occasionally lose a line if it's reduced to 100
}
function out(message){
     output.print(message);
}
