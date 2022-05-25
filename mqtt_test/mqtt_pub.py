# importing the client library
from http import client
import paho.mqtt.client as mqtt
import time

client = mqtt.Client("test_client_pub")
client.connect("localhost")

# send temperature data for every 0.1 seconds
prev_time = time.time()
while True:
    if (time.time() - prev_time) > 0.1:
        client.publish("test_topic", 25)
        prev_time = time.time()
