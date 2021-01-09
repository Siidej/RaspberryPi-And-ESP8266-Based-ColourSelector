#include "static/Servo.h"
#include <bcm2835.h>
#include <iostream>


// PWM output on RPi Plug P1 pin 12 (which is GPIO pin 18)
// in alt fun 5.
// Note that this is the _only_ PWM pin available on the RPi IO headers
#define PIN RPI_GPIO_P1_12
// and it is controlled by PWM channel 0
#define PWM_CHANNEL 0
// This controls the max range of the PWM signal
#define RANGE 12000

int Servo::setPosition(char *colour)
{
  if(!bcm2835_init())
	  return 1;
  bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_ALT5);	
  bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_32);	// Raspberry PI 3b+ basic PWM clock's frequency -> 19.2MHz 
							  // 19.2MHz / 32 = 600000Hz | 60000Hz / RANGE(12000) = 50Hz -> for our servo moter
  bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);		// Mark:Space mode.
  /* 2 Modes of PWM existe:
  Traditional PWM uses Mark:Space mode.
  The time the voltage is high is varied. (Modulated)
  it looks like this:
  ________
		  _________________________  
  <-------------------------------> 25% duty cycle
   One cycle of the base frequency
   ________________
				   ________________
   <------------------------------> 50% duty cycle

  But the RaspberryPi uses balanced mode PWM as it's default.
  Balanced mode PWM looks like this:
   _   _   _   _   _   _   _   _
    ___ ___ ___ ___ ___ ___ ___ ___
   <------------------------------> 25% duty cycle
   _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
    _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
   <------------------------------> 50% duty cycle
   */
  bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);
  
  // data = alpha * RANGE; // alpha -> output ratio
  /* alpha | degre	   
  *   5%  |  100	   // bcm2835_pwm_set_data(PWM_CHANNEL, 600);
  *   20% |   0 	   // bcm2835_pwm_set_data(PWM_CHANNEL, 2525);
  *   30% | -100	   // bcm2835_pwm_set_data(PWM_CHANNEL, 4450);
  */
  //std::cout << colour <<std::endl;
  switch(*colour)
  {
    case '0' :		// Red
      bcm2835_pwm_set_data(PWM_CHANNEL, 1500);
      printf("set position to Red\n");
      break;
    case '1' :		// Green
      bcm2835_pwm_set_data(PWM_CHANNEL, 2600);
      printf("set position to Green\n");
      break;
    case '2' :		// Blue
      bcm2835_pwm_set_data(PWM_CHANNEL, 3500);
      printf("set position to Blue\n");
      break;
    default : 
      printf("non case\n");
    
  }
  bcm2835_close();
  return 0;
}
