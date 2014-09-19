#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#define  RoAPin    0
#define  RoBPin    1

#define PinA 0
#define PinB 1

#define ServoMin 65
#define ServoMax 255
#define ServoPin 0
#define ServoDev "/dev/servoblaster"

unsigned char flag;
unsigned char Last_RoB_Status;
unsigned char Current_RoB_Status;

int setServoPosition(int lastPosition, int increment)
{
  if (increment == 0) return;

  int position = lastPosition;

  // Set the relative position in increments of 5
  position = increment > 0 ? position + 5 : position - 5;

  if (position > ServoMax) position = ServoMax;
  if (position < ServoMin) position = ServoMin;

  char command[64];
  if (sprintf(command, "echo %d=%d > %s", ServoPin, position, ServoDev) > 0) {
    system(command);
  }

  return position;
}

int rotaryDeal(int lastCount)
{
    Last_RoB_Status = digitalRead(RoBPin);

    while(!digitalRead(RoAPin)){
        Current_RoB_Status = digitalRead(RoBPin);
        flag = 1;
    }

    if(flag == 1){
        flag = 0;
        if((Last_RoB_Status == 0)&&(Current_RoB_Status == 1)){
            lastCount++;
        }
        if((Last_RoB_Status == 1)&&(Current_RoB_Status == 0)){
            lastCount--;
        }
    }

    return lastCount;
}

void rotaryRun()
{
  int lastCount = 0;
  int servoPosition = setServoPosition(ServoMin, 1);

  while(1){
    int thisCount = rotaryDeal(lastCount);
    if (thisCount != lastCount) {
      int increment = thisCount > lastCount ? 1 : -1;
      servoPosition = setServoPosition(servoPosition, increment);
    }
    lastCount = thisCount;
  }
}

int main(void)
{
  if(wiringPiSetup() < 0) {
    fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
    return 1;
  }

  pinMode(RoAPin, INPUT);
  pinMode(RoBPin, INPUT);

  rotaryRun();

  return 0;
}
