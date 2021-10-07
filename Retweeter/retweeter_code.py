# retweeter_code.py -- Sketching in Hardware 2021 MQTT topic relay
# 5 Oct 2021 -  @todbot 
# Part of the Skommunity project
#
# It 'retweets' anything from one topic to another topic.
# And vice-versa.
# But with a delay as the visualization of a message ball
# moving across the screen
#
# Hardware:
# - LILLYGO ESP32-S2 wifi board w/ built-in LCD display
#   (or similarly-capable CircuitPython board)
#
# To install:
# - Install CircuitPython 7 on board
# - Install libraries, resources, and code.py
# -- circup adafruit_imageload adafruit_display_text adafruit_minimqtt
# -- cp -X images /Volumes/CIRCUITPY
# -- cp -X oversharer_code.py /Volumes/CIRCUITPY/code.py
# - Create a 'secrets.py' with WiFi & MQTT credentials (see 'secrets_exmaple.py')
#

import time, math, random
import board, busio
import displayio, terminalio
import adafruit_imageload
from adafruit_display_text import label

import ssl, socketpool, wifi
import adafruit_minimqtt.adafruit_minimqtt as MQTT

# MQTT feeds
feeds = [
    "todbot/feeds/ouch",
    "todbot/feeds/dinger",
    ]
    
ball_count = 3
ball_img_fname = "/images/jpm30a.bmp"

display = board.DISPLAY  # CP already sets up display for us, 240x135
dw,dh = display.width, display.height

#########################################
# Display & ball setup

print("display size:", display.width, display.height)
screen = displayio.Group()  # a main group that holds everything
display.show(screen) # add main group to display
name_label = label.Label(font=terminalio.FONT, x=dw//2-20, y=dh-20, color=0x999999, text="retweeter")
screen.append(name_label)

balls = []
ball_img, ball_pal = adafruit_imageload.load(ball_img_fname)
ball_pal.make_transparent(0)

last_msg = None

class Ball:
    def __init__(self, x,y, w, h, vx=0,vy=0,tilegrid=None, action=lambda *args:None):
        self.x, self.y = x, y  # x,y pos
        self.w, self.h = w, h  # w,h size
        self.vx,self.vy = vx,vy # initial x,y velocity
        self.tg = tilegrid     # holds the bitmap
        self.action = action
    def accelerate(self, angle, magnitude):
        self.vx = max(min(self.vx + (math.sin(angle) * amount), self.vmax),-self.vmax)
        self.vy = max(min(self.vy - (math.cos(angle) * amount), self.vmax),-self.vmax)
    def off(self):
        # turn off ball
        self.tg.hidden = True
        self.vx = 0
        self.vy = 0
    def on(self, x,y, vx,vy):
        self.tg.hidden = False
        self.x, self.y = x,y
        self.vx, self.vy = vx,vy
    def apply_force():  # to be done
        pass
    def update(self):
        # wrap around
        #self.x = (self.x + self.vx) % dw  # wrap around top-bottom
        #self.y = (self.y + self.vy) % dh  # and left-right
        if self.tg.hidden: return
        # bounce
        self.x = self.x + self.vx
        self.y = self.y + self.vy
        if self.x - self.w//2 <= 0 or self.x >= dw - self.w//2: 
            self.action( self, left= self.x < dw//2, top= self.y < dh//2 )
            # self.vx = -self.vx  # disable bounce for this app
        if self.y - self.h//2 <= 0 or self.y >= dh - self.h//2: 
            self.action( self, left= self.x < dw//2, top= self.y < dh//2 )
            # self.vy = -self.vy 
        # update bitmap position
        self.tg.x = int(self.x) - self.w//2 # we think in zero-centered things
        self.tg.y = int(self.y) - self.h//2 # but tilegrids are top-left zero'd


#############################################
# WiFi & MQTT setup

try:
    from secrets import secrets
except ImportError:
    print("WiFi secrets are kept in secrets.py, please add them there!")
    raise

print("Connecting to %s" % secrets["ssid"])
wifi.radio.connect(secrets["ssid"], secrets["password"])
print("Connected to %s!" % secrets["ssid"])

      
# Create a socket pool
pool = socketpool.SocketPool(wifi.radio)

# Set up a MiniMQTT Client
mqtt_client = MQTT.MQTT(
    broker=secrets["mqtt_broker"],
    port=secrets["mqtt_port"],
    username=secrets["mqtt_username"],
    password=secrets["mqtt_password"],
    socket_pool=pool,
    ssl_context=ssl.create_default_context(),
)

# on mqtt connect
def connected(client, userdata, flags, rc):
    print("Connected to MQTT broker!")
    for feed in feeds:
        print("Listening for topic changes on",feed)
        client.subscribe(feed)

# on mqtt disconnect
def disconnected(client, userdata, rc):
    print("Disconnected from MQTT broker!")


# on mqtt message receive
def message(client, topic, message):
    # This method is called when a topic the client is subscribed to has a new message.
    print("Received message {0}: {1}".format(topic, message))
    print("last_msg", last_msg)
    this_msg = "%s:%s" % (topic,str(message))
    if this_msg == last_msg:
        print("not echoing myself")
        return
    
    # launch ball
    if topic == feeds[0]:  # left to right
        print("feed0: ",topic)
        ball = balls[0]
        ball.on( 0+30, dh//2, 1,0)
    if topic == feeds[1]:  # right to left
        print("feed1: ",topic)
        ball = balls[1]
        ball.on( dw-30, dh//2, -1,0)
    
# when ball hits edge
def ball_action(ball=None, left=False, top=False):
    global last_msg
    print("do_something:",left,top, ball)
    ball.off()
    
    val = 1 if left else 2
    val += 10 if top else 20  # such a hack
    feed = feeds[0] if left else feeds[1]
    
    print(time.monotonic(), "Sending msg w/ val: %d..." % val)
    try: 
        mqtt_client.publish(feed, val)
        last_msg = "%s:%s" % (feed,str(val))
        print(time.monotonic(), "Sent!")
    except OSError:
        print("ERROR! trying to reconnect...")
        time.sleep(1)
        #mqtt_client.disconnect()
        time.sleep(1)
        mqtt_client.reconnect()

    
# construct our balls, each with its own TileGrid holding a bitmap
for i in range(ball_count):
    imgtg = displayio.TileGrid(ball_img, pixel_shader=ball_pal)
    vx,vy = random.random() - 0.5, random.random() - 0.5   # random velocity 
    ball = Ball( dw//2, dh//2, ball_img.width, ball_img.height, vx,vy,
                 tilegrid=imgtg, action=ball_action)
    ball.off()
    screen.append(imgtg)
    balls.append( ball )

######################
# main

# Setup the callback methods above
mqtt_client.on_connect = connected
mqtt_client.on_disconnect = disconnected
mqtt_client.on_message = message

# Connect the client to the MQTT broker.
print("Connecting to MQTT broker...")
mqtt_client.connect()

while True:

    for b in balls:
        b.update()

    # this kills the framerate, not needed for sending, apparently
    # Poll the message queue 
    #mqtt_client.loop()
    mqtt_client.loop( timeout=0.03 ) # timeout=0.01 gives ETIMEOUT, unfortuanately
    #time.sleep(0.01)  # don't use this if doing mqtt_client.loop() because timeout

    
