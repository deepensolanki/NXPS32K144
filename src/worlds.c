/*
   worlds.c
   University of Michigan
   EECS 461, Embedded Control Systems
   Virtual Worlds

   Revision History
     2011-12-01  Jay Patel
     2014-02-24  Scott Kenyon
     2017-03-14  RMC
 */

#include "worlds.h"
#include "gpio.h"

float  k = 17.78;
float J = 0.45;


//====================LAB 4================================
/***************************************************************************
 * Virtual Wall
 ***************************************************************************/
float virtualWall(float angle)
{
    if(angle<WALL_POSITION)
        return -500*angle;
    else return 0.0;
}

/***************************************************************************
 * Virtual Spring
 ***************************************************************************/
float virtualSpring(float angle)
{
    return -10*angle;
}

//====================LAB 6================================
/***************************************************************************
 * Virtual Spring Damper
***************************************************************************/
float virtualSpringDamper(float angle, float velocity)
{
    //float b_m = 0.0012;
    //float b_m = 0.;
    float k = 10.0;
    //float J = 6.4E-4;
    float b;
    //b = 2*sqrt(k*J) - b_m;
    b = 0.4;
    return (-1*k*angle - b*velocity);
}

/***************************************************************************
 * Virtual Wall Damper
***************************************************************************/
float virtualWallDamper(float angle, float velocity)
{
    float b_wall = 0.5;
    if(readGPIO(4,6))
    {
      if(angle>WALL_POSITION)
        return -500*angle - b_wall*velocity;
      else return 0.0;
    }
    else
    {
      if(angle>WALL_POSITION)
              return -500*angle;
          else return 0.0;
    }

}

/***************************************************************************
 * Virtual Spring Mass
***************************************************************************/
float virtualSpringMass(float angle)
{
    float T = TIMESTEP;
    static float x1k1, x1k2;
    static float x2k1, x2k2;
    float  k = 17.78;
    float J = 0.45;
    float u1 = angle;

    x1k2 = x1k1+T*x2k1;
    x2k2 = -(k*T/J)*x1k1 + x2k1 + (k*T/J)*u1;
    x1k1 = x1k2;
    x2k1 = x2k2;

    return k*(x1k2-u1);
}

/***************************************************************************
 * Virtual Spring Mass Damper
***************************************************************************/
float virtualSpringMassDamper(float angle, float velocity)
{
  float T = TIMESTEP;
  static float x1k1, x1k2;
  static float x2k1, x2k2;
  float b = k*T;
  float u1 = angle;

  x1k2 = x1k1+T*x2k1;
  x2k2 = -(k*T/J)*x1k1 -(b*T/J)*x2k1 + (k*T/J)*u1 + x2k1 + (b*T/J)*velocity;
  x1k1 = x1k2;
  x2k1 = x2k2;

  return (k*(x1k2-u1)+b*(x2k2-velocity));

}

/***************************************************************************
 * Virtual Knob
***************************************************************************/
float virtualKnob(float angle, float velocity)
{

  //For 360 movement of knob, I've divided it into 6 positions.

  int r = (int)angle%60;

  if (r > 50)
  {
    return -1.5*virtualSpringDamper(60-r,velocity);
  }
  else if( r < 10)
  {
    return 1.5*virtualSpringDamper(r,velocity);
  }
  /*else
  {
    return virtualSpringMassDamper((int)angle%60,velocity);
      //return damped motion of knob to make it feel like some resistance is being offered
  }
  */
}

