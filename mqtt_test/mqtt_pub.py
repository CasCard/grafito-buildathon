# importing the client library
from http import client
import paho.mqtt.client as mqtt
import time
import random

client = mqtt.Client("test_client_pub")
client.connect("192.168.199.142")

def publish_metrics(Location):
    # publish a message
    client.publish(f"temperature/{Location}", random.randint(0,100))
    client.publish(f"humidity/{Location}", random.randint(0,100))
    client.publish(f"pressure/{Location}", random.randint(0,100))
    client.publish(f"moisture/{Location}", random.randint(0,100))

# send temperature data for every 0.1 seconds
prev_time = time.time()
while True:
    if (time.time() - prev_time) > 1:
        # client.publish("test_topic", random.randint(25, 35))
        publish_metrics("Kalamassery")
        publish_metrics("Cheranallur")
        publish_metrics("Kottayam")
        publish_metrics("Aluva")
        
        prev_time = time.time()
