#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#define  RoAPin    0
#define  RoBPin    1

#define PinA 0
#define PinB 1

unsigned char flag;
unsigned char Last_RoB_Status;
unsigned char Current_RoB_Status;

void readPins(int *lastStateA, int *lastStateB)
{
  int stateA = digitalRead(PinA);
  int stateB = digitalRead(PinB);

  if (*lastStateA != stateA) {
    printf("State A %d -> %d\n", *lastStateA, stateA);
  }
  if (*lastStateB != stateB) {
    printf("State B %d -> %d\n", *lastStateB, stateB);
  }

  *lastStateA = stateA;
  *lastStateB = stateB;
}

void readRun()
{
  int lastStateA = 0;
  int lastStateB = 0;

  while (1) {
    readPins(&lastStateA, &lastStateB);
  }
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

  while(1){
    int thisCount = rotaryDeal(lastCount);
    if (thisCount != lastCount) {
      printf("count: %d\n", thisCount);
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
