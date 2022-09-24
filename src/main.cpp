/*
Arrows and Enter Keyboard emulation for FNF game.

This is for Rpi Pico board.

Work in progeress...

inspired on:
https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/

*/

#include "Keyboard.h"

#define DEBOUNCE 10 // 10ms debounce time

byte buttons[] = {0, 1, 2, 3, 4, 5}; // Buttons ports in order right, down, up, left, enter.

byte led = 25;

#define NUMBUTTONS sizeof(buttons)

byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS]; // buttons states

char codeButton[] = {KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_RETURN}; // keyCodes

void setup()
{
  byte i;

  // make the pushButtons pin an input:
  for (i = 0; i < NUMBUTTONS; i++)
  {
    pinMode(buttons[i], INPUT_PULLUP);
    // digitalWrite(buttons[i], HIGH); //don know why yet.
  }

  // built in led
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  // initialize control over the keyboard:
  Keyboard.begin();
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis() < lasttime)
  {
    // we wrapped around, lets just try again
    lasttime = millis();
  }

  if ((lasttime + DEBOUNCE) > millis())
  {
    // not enough time has passed to debounce
    return;
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();

  for (index = 0; index < NUMBUTTONS; index++)
  {
    justpressed[index] = 0; // when we start, we clear out the "just" indicators
    justreleased[index] = 0;

    currentstate[index] = digitalRead(buttons[index]); // read the button

    if (currentstate[index] == previousstate[index])
    {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW))
      {
        // just pressed
        justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH))
      {
        // just released
        justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index]; // remember, digital HIGH means NOT pressed
    }
    // Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index]; // keep a running tally of the buttons
  }
}

void loop()
{
  check_switches(); // when we check the switches we'll get the current state

  for (byte i = 0; i < NUMBUTTONS; i++)
  {
    if (justpressed[i])
    {
      Keyboard.press(codeButton[i]);
      digitalWrite(led, HIGH);
    }
    if (justreleased[i])
    {
      Keyboard.release(codeButton[i]);
      digitalWrite(led, LOW);
    }
    /*if (pressed[i]) {
      Serial.print(i, DEC);
      Serial.println(" pressed");*/
    // is the button pressed down at this moment
  }
}
