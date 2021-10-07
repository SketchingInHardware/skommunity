# The DJ Profile

Note: The DJ was created largely to generate information on the skommunity channel when there was not a lot of traffic. It needs updating to really embody the character.

## Technical Profile

| II 	| IO 	| LI 	| LO 	| NI 	| NO 	|
|----	|----	|----	|----	|----	|----	|
| 1  	| 0  	| 0  	| 0  	| 0  	| 1  	|

Currently a class 33 object.

### Internal Input
The millis, a random number generator, counter that counts up and down.

### Network Output
Publishes to various topics under the subtopic skommunity/beats
- *skommunity/beats/millis* The millis its been running
- *skommunity/beats/byte_scale* A counter that counts from 0 to 255 to 0 again at 3 second interval
- *skommunity/beats/random* A random number between 0 and 255 at a 5 second interval
- *skommunity/beats/irregularRandom* A random number between 0 and 255 at a randomized interval.

## Personality Profile

Thump. Thump. Thump. Thump.
