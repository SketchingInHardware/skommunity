/*
    p5.js MQTT Random Number Client

  Baed on example code: https://github.com/tigoe/mqtt-examples
  Modified range and server
  by Carlyn Maw Sept. 2021
  see also https://editor.p5js.org/carlynorama/sketches/csV7YH2nle
*/

// MQTT client details:
let broker = {
    hostname: 'public.cloud.shiftr.io',
    port: 443
};
// MQTT client:
let client;
// client credentials:
let creds = {
    clientID: 'skommunity',
    userName: 'public',
    password: 'public'
}
// topic to subscribe to when you connect:
let topic = 'generators';

let interval = 5000;
let counter = 0;
let counterMin = 0;
let counterMax = 6;
let counterGoingUp = true;

// a pushbutton to send messages
let sendButton;
let localDiv;
let remoteDiv;

// intensity of the circle in the middle
let intensity = 255;

function setup() {
    createCanvas(400, 400);
    // Create an MQTT client:
    client = new Paho.MQTT.Client(broker.hostname, Number(broker.port), creds.clientID);
    // set callback handlers for the client:
    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;
    // connect to the MQTT broker:
    client.connect(
        {
            onSuccess: onConnect,       // callback function for when you connect
            userName: creds.userName,   // username
            password: creds.password,   // password
            useSSL: true                // use SSL
        }
    );
    // create the send button:
    sendButton = createButton('send a message');
    sendButton.position(20, 20);
    sendButton.mousePressed(sendRandom);
    // create a div for local messages:
    localDiv = createDiv('local messages will go here');
    localDiv.position(20, 50);
    localDiv.style('color', '#fff');
    // create a div for the response:
    remoteDiv = createDiv('waiting for messages');
    remoteDiv.position(20, 80);
    remoteDiv.style('color', '#fff');

  setInterval(sendNext, interval);
}

function draw() {
    background(50);
    // draw a circle whose brightness changes when a message is received:
    fill(intensity);
    circle(width/2, height/2, width/2);
// subtract one from the brightness of the circle:
    if (intensity > 0) {
        intensity--;
    }
}

// called when the client connects
function onConnect() {
    localDiv.html('client is connected');
    client.subscribe(topic);
}

// called when the client loses its connection
function onConnectionLost(response) {
    if (response.errorCode !== 0) {
        localDiv.html('onConnectionLost:' + response.errorMessage);
    }
}

// called when a message arrives
function onMessageArrived(message) {
    remoteDiv.html('I got a message:' + message.payloadString);
    let  incomingNumber = parseInt(message.payloadString);
    if (incomingNumber > 0) {
        intensity = 255;
    }
}

function sendRandom() {
  let range
  let msg = String(round(random(counterMin, counterMax)));
  sendMqttMessage(msg);
}

function sendNext() {
  sendMqttMessage(String(counter));
  updateCounter();
}

function updateCounter() {
  if (counter == counterMax) {
    counterGoingUp = false;
  } else if (counter == counterMin) {
    counterGoingUp = true;
  }
  if (counterGoingUp == true) {
    counter ++;
  } else {
    counter --;
  }

}

// called when you want to send a message:
function sendMqttMessage(msg) {
    // if the client is connected to the MQTT broker:
    if (client.isConnected()) {
        // make a string with a random number form 0 to 15:

        // start an MQTT message:
        message = new Paho.MQTT.Message(msg);
        // choose the destination topic:
        message.destinationName = topic;
        // send it:
        client.send(message);
        // print what you sent:
        localDiv.html('I sent: ' + message.payloadString);
    }
}
