#ifndef _VUMETER_H
#define _VUMETER_H

#include "../../studio/wac.h"

class VUMeterWnd;

#define WACNAME WACGeneric
#define WACPARENT WAComponentClient

class WACNAME : public WACPARENT {
public:
  WACNAME();
  virtual ~WACNAME();
  virtual GUID getGUID();
  virtual void onCreate();
  virtual void onDestroy();
};

extern WACPARENT *the;

#endif
