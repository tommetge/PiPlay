all:
	gcc rotaryEncoder.c -o rotaryEncoder -lwiringPi
	gcc servoController.c -o servoController -lwiringPi
