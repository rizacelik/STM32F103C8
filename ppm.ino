#include <libmaple/scb.h>
/*
The code I used to obtain PPM signals for STM32F103C8. This code works with https://github.com/rogerclarkmelbourne/Arduino_STM32. You have to install this when you want to work with Arduino.
*/

uint8_t int_flag, channel = 1;
uint32_t previous_time;
volatile int16_t ch1, ch2, ch3, ch4, ch5, ch6, ch7;


void setup()
{
  pinMode(PB9, INPUT); // input ppm pin
  attachInterrupt(digitalPinToInterrupt(PB9), PPM_calc, RISING);
  Serial.begin(9600);
}


void loop()
{
  Serial.print(ch1);
  Serial.print("\t");
  Serial.print(ch2);
  Serial.print("\t");
  Serial.print(ch3);
  Serial.print("\t");
  Serial.print(ch4);
  Serial.println("\t");
  delay(10);
}

void PPM_calc()
{
  // Reads PPM I/P &calculates PWM signal for up to 7  channels.
  uint32_t systick_counter = SYSTICK_BASE->CNT;  //Read SysTick counter
  if (0b1 & SCB_BASE->ICSR >> 26)
  {
    //If SysTick interrupt pending flag is set
    int_flag = 1; //Set interrupt flag
    systick_counter = SYSTICK_BASE->CNT;  //Re-read the SysTick counter
  } else {
    int_flag = 0; //SysTick interrupt flag is not set during reading
  }

  uint32_t start_time = (systick_uptime_millis * 1000) + (SYSTICK_RELOAD_VAL + 1 - systick_counter) / CYCLES_PER_MICROSECOND; //Calculate  total microseconds

  if (int_flag){
    start_time += 1000; //If the SysTick interrupt is set 1000us have to added
  }

  uint32_t PPM_delay_time = start_time - previous_time;  //to get the correct microseconds result

  if (channel == 1) {
    ch1 = PPM_delay_time;
  }

  if (channel == 2) {
    ch2 = PPM_delay_time;
  }

  if (channel == 3){
    ch3 = PPM_delay_time;
  }

  if (channel == 4) {
    ch4 = PPM_delay_time;
  }

  if (channel == 5){
    ch5 = PPM_delay_time;
  }

  if (channel == 6){
    ch6 = PPM_delay_time;
  }

  if (channel == 7) {
    ch7 = PPM_delay_time;
  }
  if (PPM_delay_time > 3020) {
    channel = 1;
  } else {
    channel++;
  }
  previous_time = start_time;
}
