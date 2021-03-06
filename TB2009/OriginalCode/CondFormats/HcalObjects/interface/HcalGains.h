#ifndef HcalGains_h
#define HcalGains_h

/** 
\class HcalGains
\author Radek Ofierzynski
POOL container to store Gain values 4xCapId
*/

#include "CondFormats/HcalObjects/interface/HcalCondObjectContainer.h"
#include "CondFormats/HcalObjects/interface/HcalGain.h"

typedef HcalCondObjectContainer<HcalGain> HcalGains;

#endif
