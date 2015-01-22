#ifndef BLEAPP_H
#define BLEAPP_H

void bleInit((void)(*sleepfunc)(void));
void advertisingStart(void);
void bleService();

#endif

