#ifndef __LCDDISPLAY__H
#define __LCDDISPLAY__H

void initLCD();
void showDisplay(float temp, float humid);
void errorDisplay(const char * message_1, const char * message_2);

#endif
