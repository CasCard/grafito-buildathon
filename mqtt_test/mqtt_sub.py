# importing the client library
from http import client
import paho.mqtt.client as mqtt
import time


def on_connect(client, userdata, message):
    print("message received ", str(message.payload.decode("utf-8")))
    print("message topic=", message.topic)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)
    print("")


client = mqtt.Client("test_client_sub")
client.on_message = on_connect
client.connect("localhost")

client.loop_start()
client.subscribe('test_topic')

time.sleep(5)
client.loop_stop()
