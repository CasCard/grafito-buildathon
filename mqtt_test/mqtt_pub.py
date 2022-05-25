# importing the client library
from http import client
import paho.mqtt.client as mqtt
import time
import random

client = mqtt.Client("test_client_pub")
client.connect("192.168.199.142")

# send temperature data for every 0.1 seconds
prev_time = time.time()
while True:
    if (time.time() - prev_time) > 1:
        # client.publish("test_topic", random.randint(25, 35))
        client.publish("temperature/location_one", 22)
        client.publish("temperature/location_two", 23)

        client.publish("humidity/location_one", random.randint(25, 35))
        client.publish("humidity/location_two", random.randint(25, 35))

        client.publish("humidity/location_one", random.randint(25, 35))
        client.publish("humidity/location_two", random.randint(25, 35))
        
        prev_time = time.time()
