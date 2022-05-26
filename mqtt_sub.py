# importing the client library
from http import client
import paho.mqtt.client as mqtt
import time
from fuzzy_logic import FuzzyLogic


class MQTTSub:
    def __init__(self):
        self.fuzzy = FuzzyLogic()
        self.client = mqtt.Client("MQTTSub")
        self.temp = 0
        self.humidity = 0
        self.Soil_moisture = 0
        self.client.message_callback_add('temperature/#', self.on_message_temp)
        self.client.message_callback_add(
            'humidity/#', self.on_message_humidity)
        self.client.message_callback_add('#', self.on_message_all)
        self.client.message_callback_add(
            'moisture/Kalamassery', self.on_message_moisture)
        self.client.connect("localhost")
        self.client.subscribe("#")
        self.client.loop_forever()

    def on_message_moisture(self, client, userdata, message):
        self.Soil_moisture = float(message.payload.decode("utf-8"))

    def on_message_all(self, client, userdata, message):
        out_flowrate = self.fuzzy.get_output(
            self.Soil_moisture/6, self.humidity/4, self.temp/2, 5)
        self.client.publish("out_flowrate", out_flowrate)

    def on_message_temp(self, client, userdata, message):
        new_temp = float(message.payload.decode("utf-8"))
        self.temp = new_temp + self.temp * 0.1*(new_temp - self.temp)
        self.temp = new_temp

    def on_message_humidity(self, client, userdata, message):
        new_humidity = float(message.payload.decode(
            "utf-8"))
        self.humidity = new_humidity + (new_humidity - self.humidity)*0.1


if __name__ == "__main__":
    mqtt_sub = MQTTSub()
