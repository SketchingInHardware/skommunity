# DoomScroller Profile

## Technical Profile

| II 	| IO 	| LI 	| LO 	| NI 	| NO 	|
|----	|----	|----	|----	|----	|----	|
| 1  	| 1  	| 1  	| 1  	| 0  	| 1  	|

A class 61 object. (Possibly a class 45^*)

### General Behavior Description
Compares two variables, the number of bytes received and a tolerance based on local environment. When the number of bytes received exceeds tolerance the system resets. As the number of bytes received increases the frequency with which the device polls the network environment decreases based on values in a look up table.

### Internal Input
Has a look table to scale its exhaustion level in a non-linear fashion.

### Internal Output
Debugging via Serial is on.  ^*(Is Serial a _local_ output or an _internal_ one.)

### Local Input
Analog input to vary the size of the tolerance variable.
Digital read set up, but does not effect the behavior of the device.

### Local Output
LED blinking. Rate is based on a constant, but is incidentally impacted by the delay() function decreasing the rate of network polling.

### Network Input
Device periodically polls MQTT server for ALL the message it has received.

### Network Output
None.

## Personality Profile

The DoomScroller loads up a feed and keeps refreshing the stream until its brain forces a reboot from being overwhelmed. The DoomScroller becomes less able to keep up the rate of in take as its the quantity of information it has consumed increases. It barely notices the content, but it must keep asking for more.

External inputs can enhance or curtail the DoomScrollers ability to continue on.

Observers do not see much but may overtime notice the DoomScroller seems to not really blink very often.
