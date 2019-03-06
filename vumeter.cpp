// Always start with std.h
#include <precomp.h>
#include "../../bfc/std.h"
//
#include "vumeter.h"
#include "resource.h"
#include "vumeterwnd.h"
//
#include "../../common/xlatstr.h"
// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"


static WACNAME wac;
WACPARENT *the = &wac;                     

// {555AA803-78AD-4bdb-AE11-8ECAD692D4FB}
static const GUID guid =
{ 0x555aa803, 0x78ad, 0x4bdb, { 0xae, 0x11, 0x8e, 0xca, 0xd6, 0x92, 0xd4, 0xfb } };



WACNAME::WACNAME() : WACPARENT("DeJaVU Meter")
{
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<VUMeterWnd> >);
  registerService(new WndCreateCreatorSingle< CreateBucketItem<VUMeterWnd> >);
  registerAutoPopup(getGUID(), getName());

  registerService( new XuiObjectCreator<VUMeterWndXuiSvc> );
 // registerSkinFile( "Wacs/xml/VUMeter/base.xml" ); 
}


WACNAME::~WACNAME() {}

GUID WACNAME::getGUID() { return guid; }

void WACNAME::onCreate() {}

void WACNAME::onDestroy() {}

