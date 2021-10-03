
#ifndef MQTTDEFS
#define MQTTDEFS

// define a new type that is a function pointer
typedef String (*messsage_function)(void);

struct MQTT_Object {
  long lastTimeSent;
  int interval;
  messsage_function getMessage;
  char topic[];
};

//TODO: For run time updates, but maybe doesn't really work
//char* topicAssembler(char* topic, char* subtag) {
//    char myConcatenation[strlen(topic) + 1 + strlen(subtag)];
//    sprintf(myConcatenation,"%s/%s",topic,subtag);
//    return myConcatenation;
//}

#endif
