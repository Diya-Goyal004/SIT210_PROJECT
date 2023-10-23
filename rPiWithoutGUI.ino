from bluepy import btle
import time
import RPi.GPIO as GPIO
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QPushButton, QCheckBox, QHBoxLayout
import sys

# Define GPIO pins for the water pump, LED, and Peltier device
WATER_PUMP_PIN = 17  # Replace with the actual GPIO pin number for the water pump
LED_PIN = 22  # Replace with the actual GPIO pin number for the LED
PELTIER_PIN = 24  # Replace with the actual GPIO pin number for the Peltier device

# Define MAC address for the device
mac_address = "17:ef:77:33:82:44"  # Replace with the MAC address of the device

SERVICE_UUID = "12345678-1234-5678-1234-56789abcdef0"
CHARACTERISTIC_UUID_SOIL_MOISTURE = "12345678-1234-5678-1234-56789abcdef2"
CHARACTERISTIC_UUID_WATER_LEVEL = "12345678-1234-5678-1234-56789abcdef3"
CHARACTERISTIC_UUID_BH1750 = "12345678-1234-5678-1234-56789abcdef4"

SOIL_MOISTURE_THRESHOLD = 500  # Adjust this threshold as needed
LIGHT_INTENSITY_THRESHOLD = 1000  # Adjust this threshold as needed
WATER_LEVEL_THRESHOLD = 30  # Adjust this threshold as needed

LED_ON_HOURS = 6  # Number of hours to keep the LED on
PELTIER_ON_THRESHOLD = 25  # Threshold for turning on the Peltier device

def byte_array_to_int(value):
    value = bytearray(value)
    value = int.from_bytes(value, byteorder="little", signed=True)
    return value

def read_soil_moisture(service):
    soil_moisture_char = service.getCharacteristics(CHARACTERISTIC_UUID_SOIL_MOISTURE)[0]
    soil_moisture = soil_moisture_char.read()
    soil_moisture = byte_array_to_int(soil_moisture)
    print(f"Soil Moisture: {soil_moisture}")
    return soil_moisture

def read_light_intensity(service):
    light_intensity_char = service.getCharacteristics(CHARACTERISTIC_UUID_BH1750)[0]
    light_intensity = light_intensity_char.read()
    light_intensity = byte_array_to_int(light_intensity)
    print(f"Light Intensity: {light_intensity}")
    return light_intensity

def read_water_level(service):
    water_level_char = service.getCharacteristics(CHARACTERISTIC_UUID_WATER_LEVEL)[0]
    water_level = water_level_char.read()
    water_level = byte_array_to_int(water_level)
    print(f"Water Level: {water_level}")
    return water_level

def control_water_pump(soil_moisture):
    if soil_moisture < SOIL_MOISTURE_THRESHOLD:
        print("Water pump triggered")
        GPIO.output(WATER_PUMP_PIN, GPIO.HIGH)  # Turn on the water pump
    else:
        GPIO.output(WATER_PUMP_PIN, GPIO.LOW)  # Turn off the water pump

def control_led_intensity(light_intensity):
    if light_intensity > LIGHT_INTENSITY_THRESHOLD:
        print("Adjusting LED intensity")
        # Calculate a duty cycle (0 to 100) based on light_intensity and set it
        duty_cycle = (light_intensity - LIGHT_INTENSITY_THRESHOLD) / (MAX_LIGHT_INTENSITY - LIGHT_INTENSITY_THRESHOLD) * 100
        pwm_led.start(duty_cycle)  # Start PWM with the calculated duty cycle
    else:
        pwm_led.stop()  # Turn off the LED

def control_peltier(water_level):
    if water_level < WATER_LEVEL_THRESHOLD:
        print("Water level below threshold - Peltier device triggered")
        GPIO.output(PELTIER_PIN, GPIO.HIGH)  # Turn on the Peltier device
    else:
        print("Water level above threshold - Peltier device turned off")
        GPIO.output(PELTIER_PIN, GPIO.LOW)  # Turn off the Peltier device

print("Connecting to the device…")
nano_sense_device = btle.Peripheral(mac_address)

print("Discovering Services for the device…")
_ = nano_sense_device.services
bleService = nano_sense_device.getServiceByUUID(SERVICE_UUID)

print("Discovering Characteristics for the device…")
_ = bleService.getCharacteristics()

class SmartGardenGUI(QWidget):
    def init(self):
        super().init()

        self.setWindowTitle("Smart Vertical Farming")
        self.setGeometry(100, 100, 400, 200)

        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()

        label = QLabel("System Controls:")
        layout.addWidget(label)

        water_pump_checkbox = QCheckBox("Water Pump")
        peltier_device_checkbox = QCheckBox("Peltier Device")
        led_checkbox = QCheckBox("LED")

        layout.addWidget(water_pump_checkbox)
        layout.addWidget(peltier_device_checkbox)
        layout.addWidget(led_checkbox)

        water_pump_checkbox.clicked.connect(lambda: self.control_water_pump(water_pump_checkbox.isChecked()))
        peltier_device_checkbox.clicked.connect(lambda: self.control_peltier(peltier_device_checkbox.isChecked()))
        led_checkbox.clicked.connect(lambda: self.control_led(led_checkbox.isChecked()))

        return layout

    def control_water_pump(self, state):
        GPIO.output(WATER_PUMP_PIN, GPIO.HIGH if state else GPIO.LOW)

    def control_peltier(self, state):
        GPIO.output(PELTIER_PIN, GPIO.HIGH if state else GPIO.LOW)

    def control_led(self, state):
        GPIO.output(LED_PIN, GPIO.HIGH if state else GPIO.LOW)

if name == "main":
    app = QApplication(sys.argv)
    pwm_led = GPIO.PWM(LED_PIN, 100)  # Create PWM object for the LED
    MAX_LIGHT_INTENSITY = 2000  # Maximum light intensity (adjust as needed)

    try:
        led_timer = time.time()

        while True:
            print("\nReading Data:")
            soil_moisture = read_soil_moisture(bleService)
            light_intensity = read_light_intensity(bleService)
            control_water_pump(soil_moisture)
            control_led_intensity(light_intensity)

            water_level = read_water_level(bleService)

            control_peltier(water_level)

            current_time = time.time()

            if current_time - led_timer >= LED_ON_HOURS * 3600:
                print("Turning off LED")
                GPIO.output(LED_PIN, GPIO.LOW)

            if current_time - led_timer >= LED_ON_HOURS * 3600:
                led_timer = current_time

            time.sleep(1)

            gui = SmartGardenGUI()
            gui.show()

            sys.exit(app.exec_())
    finally:
        nano_sense_device.disconnect()
        GPIO.cleanup()
