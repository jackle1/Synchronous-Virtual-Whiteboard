pin=3
gpio.mode(pin, gpio.OUTPUT)
gpio.write(3, gpio.LOW)
print(gpio.read(pin))