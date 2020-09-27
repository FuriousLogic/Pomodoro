#include <Arduino.h>

enum class State {neutral, pomodoro, shortBreak, longBreak, pomodoroOver, breakOver, error};
const byte interruptPin = 2 ;
const int ledRed = 5;
const int ledGreen = 7;
const int ledGreen2 = 11;
const int ledYellow = 9;
const int shortBreakMax = 3;
const int buzzer = 12;

// Live Vals
const long lenPomodoro = 25L*60L*1000L;
const long lenShortBreak = 10L*60L*1000L;
const long lenLongBreak = 15L*60L*1000L;

// Test vals
// const long lenPomodoro = 25*1000;
// const long lenShortBreak = 10*1000;
// const long lenLongBreak = 15*1000;

static unsigned long last_interrupt_time = 0;
State currentState = State::neutral;
unsigned long start;
int shortBreakCount = 0;

void buttonPush(){
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200){
    switch (currentState){
      case State::neutral:
        currentState = State::pomodoro;
        break;
      case State::pomodoro:
        currentState = State::neutral;
        break;  
      case State::shortBreak:
        currentState = State::neutral;
        break;  
      case State::longBreak:
        currentState = State::neutral;
        break;  
      case State::pomodoroOver:
        if(shortBreakCount >= shortBreakMax){
          currentState = State::longBreak;
          shortBreakCount=0;
        }else{
          currentState = State::shortBreak;
          shortBreakCount++;
        }
        break;  
      case State::breakOver:
        currentState = State::neutral;
        break;  
      case State::error:
        currentState = State::neutral;
        break;  
      default:
        currentState = State::error;
        break;
    }
    start = millis();
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin),buttonPush,RISING);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledGreen2, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(buzzer, OUTPUT);

  //test
  digitalWrite(ledRed,HIGH);
  delay(1000);
  digitalWrite(ledGreen,HIGH);
  delay(1000);
  digitalWrite(ledGreen2,HIGH);
  delay(1000);
  digitalWrite(ledYellow,HIGH);
  delay(1000);
  digitalWrite(ledRed,LOW);
  digitalWrite(ledGreen,LOW);
  digitalWrite(ledGreen2,LOW);
  digitalWrite(ledYellow,LOW);
  tone(buzzer, 261);
  delay(500);
  tone(buzzer, 277);
  delay(500);
  tone(buzzer, 294);
  delay(500);
  noTone(buzzer);
}

void loop() {
  unsigned long elapsed = millis() - start;
  char mystr[40];
  sprintf(mystr,"Elapsed: %ld State: %d", elapsed, currentState);
  Serial.println(mystr);

  switch (currentState)
  {
  case State::neutral:
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledYellow,HIGH);
    break;
  case State::pomodoro:
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledYellow,LOW);
    if(elapsed > lenPomodoro){
      currentState = State::pomodoroOver;
      tone(buzzer, 294);
      delay(500);
      tone(buzzer, 261);
      delay(500);
      noTone(buzzer);
    }
    break;  
  case State::shortBreak:
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledYellow,LOW);
    if(elapsed > lenShortBreak){
      currentState = State::breakOver;
      tone(buzzer, 261);
      delay(500);
      tone(buzzer, 294);
      delay(500);
      noTone(buzzer);
    }
    break;  
  case State::longBreak:
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledGreen2,HIGH);
    digitalWrite(ledYellow,LOW);
    if(elapsed > lenLongBreak){
      currentState = State::breakOver;
      tone(buzzer, 261);
      delay(500);
      tone(buzzer, 294);
      delay(500);
      noTone(buzzer);
    }
    break;  
  case State::pomodoroOver:
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledYellow,HIGH);
    break;  
  case State::breakOver:
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledYellow,HIGH);
    break;  
  case State::error:
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledGreen2,HIGH);
    digitalWrite(ledYellow,HIGH);
    break;  
  default:
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledYellow,LOW);
    break;
  }
}

