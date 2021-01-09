#include <stdio.h>
//volatile int colour;
int colour;
class Servo
{
public:
	Servo(){;}
	static int setPosition(char *colour);
};
