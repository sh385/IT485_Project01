#include "light.h"

Light* createLight()
{
	Light* light = (Light*)malloc(sizeof(Light));
	return light;
}