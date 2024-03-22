// *****************************************************************************************************
// This MQTTfx script for Hexbot robot created by hexbotScriptCompiler on 10/8/2023 at 12:13:36.2061510
// NOTE: Doug's robot = Hexbot3C61054ADD98, Andrew's robot = Hexbot94B97E5F4A40.
// *****************************************************************************************************
var Thread = Java.type("java.lang.Thread");
function execute() {
mybot = "hfr48B8/commands"

send("NEW_FLOW")
send("Flow, 1000,MLRH,10,0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0")
send("flow,1,bom")


send("Flow,0,SM,h")
// above just resets flow row start point for next macro


send("Flow, 1000,MLRH,10,0,0,0, 5.9,10,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0")
send("Flow, 1000,MLRH,10,0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0")
send("Flow,0,SM,a")

send("Flow, 1000,MLRH,10,0,0,0, 0,0,0, 5.9,10,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0")
send("Flow, 1000,MLRH,10,0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0")
send("Flow,0,SM,b")

send("Flow, 1000,MLRH,10,0,0,0, 0,0,0, 0,0,0, 5.9,10,0, 0,0,0, 0,0,0, 0,0,0")
send("Flow, 1000,MLRH,10,0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0")
send("Flow,0,SM,c")


send("flow,1,eom")

send("flow,1,dm,a")

send("flow,1,dm,b")

send("flow,1,dm,c")

send("flow,1,dm,b")

send("flow,1,dm,a")

//send("flow,1,dm,a")

send("DO_FLOW,1,25")
action.setExitCode(0);
action.setResultText("done.");
out("Parameter parameter set up complete");
return action;
}
function send(cmd){
out(cmd);      // display command in MQTT console as its downloaded to bot
mqttManager.publish(mybot, cmd);
Thread.sleep(200);
}
function out(message){
output.print(message);
}
