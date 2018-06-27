import RPi.GPIO as GPIO
import time
PIN = 11
GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN, GPIO.OUT)
for i in range (0,10):
	print("ON: ", i)
	GPIO.output(PIN, True)
	time.sleep(1)
	print("OFF ", i)
	GPIO.output(PIN, False)
	time.sleep(1)

GPIO.cleanup()
