# oversharer_code.py -- Sketching in Hardware 2021 MQTT bounce emitter
# 5 Oct 2021 -  @todbot 
# Part of the Skommunity project
#
# It overshares its internal thoughts, posting them to the Net,
# not caring what others think, slowly going crazy.
#
# Hardware:
# - LILLYGO ESP32-S2 wifi board w/ built-in LCD display
#   (or similarly-capable CircuitPython board)
#
# To install:
# - Install CircuitPython 7 on board
# - Install libraries, resources, and code.py
# -- circup adafruit_imageload adafruit_minimqtt
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
ouch_feed = "skommunity/feeds/ouch"
feeds = (
    'skommunity/feeds/ouchU',
    'skommunity/feeds/ouchD',
    'skommunity/feeds/ouchL',
    'skommunity/feeds/ouchR',
)

msgs = (
    (  # ball 0 - pain
        'Ouch my head hurts.',
        'Ouch my back hurts.',
        'Ouch my throat hurts.',
        'Ouch my everything hurts.'
    ),
    (  # ball 1 - covid
        'Is this a cough?',
        'Leave the house?',
        'Where is my mask?',
        'Where are my pants?'
    ),
    (  # ball 2 - media
        'Wow this streaming show in bingeable.',
        'I have binged all the shows.',
        'Where are more shows to binge?',
        'I am rewatching Star Trek.'
    )
)

ball_count = 3
ball_img_fname = "/images/jpm30a.bmp"

display = board.DISPLAY  # CP already sets up display for us, 240x135
display.brightness = 0.05

dw,dh = display.width, display.height # convenience for later

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
    def update(self):
        # wrap around
        #self.x = (self.x + self.vx) % dw  # wrap around top-bottom
        #self.y = (self.y + self.vy) % dh  # and left-right
        # bounce
        self.x = self.x + self.vx
        self.y = self.y + self.vy
        if self.x - self.w//2 <= 0 or self.x >= dw - self.w//2: 
            self.vx = -self.vx
            self.action( self, left= self.x < dw//2, top= self.y < dh//2 )
        if self.y - self.h//2 <= 0 or self.y >= dh - self.h//2: 
            self.vy = -self.vy
            self.action( self, left= self.x < dw//2, top= self.y < dh//2 )
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


def connected(client, userdata, flags, rc):
    feed = ouch_feed
    # Called when the client is connected successfully to the broker
    print("Connected to MQTT broker! Listening for topic changes on %s" % feed)
    # Subscribe to all changes on the onoff_feed.
    client.subscribe(feed)

def disconnected(client, userdata, rc):
    # Called when the client is disconnected
    print("Disconnected from MQTT broker!")

def message(client, topic, message):
    # Called when a topic the client is subscribed to has a new message
    print("New message on topic {0}: {1}".format(topic, message))


# socketpool for mqtt_client connections
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

# Setup the callback methods above
mqtt_client.on_connect = connected
mqtt_client.on_disconnect = disconnected
mqtt_client.on_message = message

# Connect the client to the MQTT broker.
print("Connecting to MQTT broker...")
mqtt_client.connect()

#########################################
# Display & ball setup

screen = displayio.Group()  # a main group that holds everything
display.show(screen) # add main group to display
name_label = label.Label(font=terminalio.FONT, x=dw//2-20, y=dh-20, color=0x999999, text="oversharer")
screen.append(name_label)

balls = []
ball_img, ball_pal = adafruit_imageload.load(ball_img_fname)
ball_pal.make_transparent(0)
img_w = ball_img.width
img_h = ball_img.height

def send_mqtt_msg(topic,message):
    try:
        mqtt_client.publish(topic,message)
    except OSError:
        print("ERROR sending to",topic)
        
def bounce_action(ball=None, left=False, top=False):
    print("bounce_action:",left,top)
    ball_num = 0
    if ball == balls[0]: ball_num = 0
    if ball == balls[1]: ball_num = 1
    if ball == balls[2]: ball_num = 2
        
    val = 0
    if left and top: val = 1
    if left and not top: val = 2
    if not left and top: val = 3
    if not left and not top: val = 4

    # Send a message
    print(time.monotonic(), "Sending to ",ouch_feed, ":", val)

    feed = feeds[val-1]
    msg = msgs[ball_num][val-1]
    msg = msg + ' ' + str(time.monotonic())
    
    print(time.monotonic(), "Sending to ",feed, ":",msg)
    send_mqtt_msg( feed, msg)

    send_mqtt_msg( ouch_feed, msg)


# construct our balls, each with its own TileGrid holding a bitmap
for i in range(ball_count):
    imgtg = displayio.TileGrid(ball_img, pixel_shader=ball_pal)
    # random velocity vector
    vx,vy = random.random() - 0.75, random.random() - 0.5
    ball = Ball( dw//2, dh//2, img_w, img_h, vx,vy, tilegrid=imgtg, action=bounce_action)
    screen.append(imgtg)
    balls.append(ball)

######################
# main

while True:
    # this kills the framerate, since defaults to timeout=1.0 second
    # but not needed for sending
    # Poll the message queue 
    #mqtt_client.loop()

    if not mqtt_client.is_connected():
        print("disconnected! attempting to reconnect")
        time.sleep(1)
        mqtt_client.reconnect()
        
    for b in balls:
        b.update()

    time.sleep(0.02)

    
