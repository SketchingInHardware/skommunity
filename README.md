# skommunity
Sketching21 group installation

Notes
----

TODO: Define "in the room"

## The Technical Profile

Each device is characterized by the presence or absence of inputs and outputs operating in a given domain. The domains represent the ability to provide information at a distance from the object. The modality of their behavior does not matter.

For example think of a light based emitter receiver pair:

- Internal Domain: Fully encased optoisolator
- Local Domain: One device has a LED the other a Photodiode
- Network Domain: TCP/IP over fiberoptic cabling.

#### Internal Input

Sensors/Inputs placed to primarily receive from paired “Internal Outputs” or “the void.”

Currently lookup tables, stored data and algorithms being applied to inputs and outputs are being classified as internal inputs.

#### Internal Output

Positioned to address an internal input or “the void.”

#### Local Input

Sensors/receivers placed such that they could receive information from a person or device "in the room."

#### Local Output

Outputs placed such that they could transmit information to a person or device "in the room."

#### Network Input

Information received from an emitter "outside the room" that is largely undetectable by humans "in the room" with the device.  

#### Network Output

Information sent "outside the room" in a manner undetectable by humans "in the room."


### Technical Classification

Each device can be classified as one of 64 device types by using a 6 bit number based on the presence or absence of IO in the different domains.

| II 	| IO 	| LI 	| LO 	| NI 	| NO 	|
|----	|----	|----	|----	|----	|----	|
| 0  	| 0  	| 0  	| 0  	| 0  	| 0  	|

The examples below represent the current classification schema. Opinions can and will change without notification.

Example A: a device that reads information on one MQTT channel and repeats it on another MQTT channel would be a Class 3 device.

| II 	| IO 	| LI 	| LO 	| NI 	| NO 	| CLASS |
|----	|----	|----	|----	|----	|----	|----   |
| 0  	| 0  	| 0  	| 0  	| 1  	| 1  	|3  	  |



Example B: A device that a takes a sensor reading and transmits the value unaltered except for a type conversion.  

| II 	| IO 	| LI 	| LO 	| NI 	| NO 	| CLASS |
|----	|----	|----	|----	|----	|----	|----   |
| 0  	| 0  	| 1  	| 0  	| 0  	| 1  	|9  	  |



Example C: A device that has a temperature sensor. It _maps_^* the brightness of an LED to the temperature.

| II 	| IO 	| LI 	| LO 	| NI 	| NO 	| CLASS |
|----	|----	|----	|----	|----	|----	|----   |
| 1  	| 0  	| 1  	| 1  	| 0  	| 0  	|88  	  |

^* whether or not this represents "internal input" or not has not been resolved by the Authors.

Example D: A device that has a light sensor and a temperature sensor. It uses the light value to change the rate that it transmits the temperature data.

| II 	| IO 	| LI 	| LO 	| NI 	| NO 	| CLASS |
|----	|----	|----	|----	|----	|----	|----   |
| 1  	| 0  	| 1  	| 0  	| 0  	| 1  	|41  	  |
