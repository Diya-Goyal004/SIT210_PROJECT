from bluepy import btle
import time

mac_address = "17:ef:77:33:82:44"  # Put in your own Mac address here
SERVICE_UUID = "12345678-1234-5678-1234-56789abcdef0"
CHARACTERISTIC_UUID_SOIL_MOISTURE = "12345678-1234-5678-1234-56789abcdef2"
CHARACTERISTIC_UUID_WATER_LEVEL = "12345678-1234-5678-1234-56789abcdef3"
CHARACTERISTIC_UUID_BH1750 = "12345678-1234-5678-1234-56789abcdef4"

def byte_array_to_int(value):
    value = bytearray(value)
    value = int.from_bytes(value, byteorder="little", signed=True)
    return value

def read_soil_moisture(service):
    soil_moisture_char = service.getCharacteristics(CHARACTERISTIC_UUID_SOIL_MOISTURE)[0]
    soil_moisture = soil_moisture_char.read()
    soil_moisture = byte_array_to_int(soil_moisture)
    print(f"Soil Moisture: {soil_moisture}")

def read_water_level(service):
    water_level_char = service.getCharacteristics(CHARACTERISTIC_UUID_WATER_LEVEL)[0]
    water_level = water_level_char.read()
    water_level = byte_array_to_int(water_level)
    print(f"Water Level: {water_level}")

def read_bh1750_light(service):
    bh1750_char = service.getCharacteristics(CHARACTERISTIC_UUID_BH1750)[0]
    light_intensity = bh1750_char.read()
    light_intensity = byte_array_to_int(light_intensity)
    print(f"Light Intensity: {light_intensity}")

print("Connecting…")
nano_sense = btle.Peripheral(mac_address)

print("Discovering Services…")
_ = nano_sense.services
bleService = nano_sense.getServiceByUUID(SERVICE_UUID)

print("Discovering Characteristics…")
_ = bleService.getCharacteristics()

try:
    while True:
        print("\n")
        read_soil_moisture(bleService)
        read_water_level(bleService)
        read_bh1750_light(bleService)
        time.sleep(1)
finally:
    nano_sense.disconnect()
