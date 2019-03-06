#include <precomp.h>
#include "../../bfc/std.h"
#include "../../core/corehandle.h"

#include "vumeter.h"
#include "vumeterwnd.h"
#include "resource.h"

#include "../../bfc/canvas.h"
#include "../../common/xlatstr.h"
//#include "../../common/corehandle.h"

#include "../../bfc/wnds/combobox.h"

char VUMeterWndXuiObjectStr[] = "VUMeter.Window";
char VUMeterWndXuiSvcName[]   = "VUMeter:Window XuiObject Service";


//  Methods required by Window Creation Services
const char *VUMeterWnd::getWindowTypeName() { return "DeJaVU Meter"; }
GUID VUMeterWnd::getWindowTypeGuid() { return the->getGUID(); }
void VUMeterWnd::setIconBitmaps(ButtonWnd *button)
{ button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED); }


//init all of our main variables
VUMeterWnd::VUMeterWnd() {
	setName("DeJaVU Meter");
	setVirtual(FALSE);
	colourz = NULL;
	text = true;
	VUDir = 0;

	int i;
	for (i=0; i<3; i++) {
		drawStyle[i] = 0;
		colourStyle[i] = 0;
	}
	PaintStyle = 0;

	hPort = NULL;

	ZeroMemory(sendData, sizeof(sendData));

	timerclient_setTimer(326, 1);
}

VUMeterWnd::~VUMeterWnd() {
	timerclient_killTimer(326);
	CloseHandle(hPort);
	hPort = NULL;
}


void VUMeterWnd::timerclient_timerCallback(int id) {
	VUMETERWND_PARENT::timerclient_timerCallback(id);

	if (id == 326) {
		DWORD read = 0;
		char readData[256];
		ReadFile (hPort, readData, sizeof(readData), &read, 0);
		if (read == 0) return;
		if (readData[0] != 1) return;

		VisData data;
		CoreHandle core;
//		int rt = api->core_getVisData(0, &data, sizeof(VisData));
		int rt = core.getVisData(&data, sizeof(VisData));

		int i;
		DWORD write;
//		memset(sendData, 1, sizeof(sendData));

		for (i=1; i<31; i++) {
//			int j = i<<1;
//			int tmp = (data.spectrumData[0][j] + data.spectrumData[1][j] + data.spectrumData[0][j+1] + data.spectrumData[1][j+1])/3;
			int tmp = 0;
			int amt = 4;
			for (int j=0; j<amt; j++) {
				int tmp2 = ((data.spectrumData[0][((i-1)*amt)+j] + data.spectrumData[1][((i-1)*amt)+j]));
				tmp2 /= 24;
				tmp2 = min(20, tmp2);
				tmp = max(tmp, tmp2);
			}

			sendData[i] = max(tmp, sendData[i]-2);
			if (sendData[i] == 0xFF) sendData[i] = (char)0xFE;
//			if (sendData[i+1] == 0x00) sendData[i+1] = 1;
		}
		

		//preform all calculations here
//		for (int i=0; i<sizeof(sendData); i++) {
//			sendData[i] = i;
//			sendData[i] = -i+160;
//		}

		sendData[0] = 1;
		sendData[sizeof(sendData)-1] = (char)0xFF;
		WriteFile(hPort, sendData, sizeof(sendData), &write, NULL);
	}
}

/*
	VisData myVisData;
	int rt = api->core_getVisData(0, &myVisData, sizeof(VisData));
				for (i=0; i<bars; i++)
				{
					rect.left = crect.left + (i*6);
					rect.right = rect.left + 5;
					rect.bottom = crect.bottom;
					Data = ((myVisData.spectrumData[0][576*i/bars] + myVisData.spectrumData[1][576*i/bars]) >> 1)*2;
					if (Data > 255) Data = 255;
					int barHeight = (wHeight*Data/255);
					
					switch (drawStyle[1])
					{
					case 0:
						for (i2=0; i2<barHeight; i2++)
						{
							int barLine = crect.bottom - i2;
							canvas->pushPen(colourz[i2]);
							canvas->lineDraw(rect.left, barLine, rect.right, barLine);
							canvas->popPen();
						}
						break;

					case 1:
						for (i2=0; i2<barHeight; i2++)
						{
							int barLine = crect.bottom - i2;
							canvas->pushPen( colourz[wHeight+(i2-barHeight)] );
							canvas->lineDraw(rect.left, barLine, rect.right, barLine);
							canvas->popPen();
						}
						break;

					case 2:
						rect.top = crect.bottom - barHeight;
						canvas->fillRect(&rect, colourz[barHeight]);
						break;
					}
				}
*/

int VUMeterWnd::getPreferences(int what)  {
/*   switch (what) 
   {
      case SUGGESTED_W: return 500;
      case SUGGESTED_H: return 200;
      case SUGGESTED_X: return -1;
      case SUGGESTED_Y: return -1;
      case MINIMUM_W:   return 500;
      case MINIMUM_H:   return 200;
      case MAXIMUM_W:   return -1;
      case MAXIMUM_H:   return -1;
   }
*/
   return -1;
}


//set internal variables to match those within the XML files
int VUMeterWnd::setXmlParam(const char *name, const char *strvalue) {
	int ret = VUMETERWND_PARENT::setXmlParam(name, strvalue);

	if ( stricmp(name, "DisplayText") == 0 ) {
		if (strvalue[0] == '0') text = false;
	}

	else if ( stricmp(name, "DefaultVis") == 0 ) {
		int tmp;
		SSCANF(strvalue, "%d", &tmp);
		if ((tmp>-1) & (tmp<3)) PaintStyle = tmp;
	}

	else if ( stricmp(name, "Color0") == 0 ) {
		int tmp;
		SSCANF(strvalue, "%d", &tmp);
		if ((tmp>-1) & (tmp<6)) colourStyle[0] = tmp;
	}

	else if ( stricmp(name, "Color1") == 0 ) {
		int tmp;
		SSCANF(strvalue, "%d", &tmp);
		if ((tmp>-1) & (tmp<6)) colourStyle[1] = tmp;
	}

	else if ( stricmp(name, "Color2") == 0 ) {
		int tmp;
		SSCANF(strvalue, "%d", &tmp);
		if ((tmp>-1) & (tmp<6)) colourStyle[2] = tmp;
	}

	else if ( stricmp(name, "Style0") == 0 ) {
		int tmp;
		SSCANF(strvalue, "%d", &tmp);
		if ((tmp>-1) & (tmp<3)) drawStyle[0] = tmp;
	}

	else if ( stricmp(name, "Style1") == 0 ) {
		int tmp;
		SSCANF(strvalue, "%d", &tmp);
		if ((tmp>-1) & (tmp<3)) drawStyle[1] = tmp;
	}

	else if ( stricmp(name, "Style2") == 0 ) {
		int tmp;
		SSCANF(strvalue, "%d", &tmp);
		if ((tmp>-1) & (tmp<3)) drawStyle[2] = tmp;
	}

	else if ( stricmp(name, "VUDir") == 0 ) {
		int tmp;
		SSCANF(strvalue, "%d", &tmp);
		if ((tmp>-1) & (tmp<3)) VUDir = tmp;
	}

	return ret;
}

/*
//currently, the only timer we have is for repainting
//soon, there will be one for "peak falloffs"
void VUMeterWnd::timerCallback(int id)
{
	switch (id)
	{
	case 325:
		invalidate();
		break;
	}
}
*/

//whenever we resize the window, we need
//to recreate things like our colour data
int VUMeterWnd::onResize()
{
	//get new client rect, and calculate width and height
	crect = clientRect();
	wWidth = crect.right - crect.left;
	wHeight = crect.bottom - crect.top;

	//if colourz exist already, lets free them!
	if (colourz) FREE(colourz);
	
	int i;
	int inc;
	int vis;

	//different paint styles repuire different
	//amouts of colours to be displayed
	switch (PaintStyle)
	{
	case 0:  inc=wWidth+1;		break;
	case 1:  inc=wHeight;			break;
	case 2:  inc=wHeight/2;		break;
	}

	//allocate new colourz
	colourz = (COLORREF*)MALLOC(inc*4);
	
	//create colourz
	switch (colourStyle[PaintStyle])
	{
	case 0: //red
		for (i=0; i<inc; i++)
		{
			vis=(255*i/inc);
			int red = (vis < 128) ? -vis+128 : 0;
			int green = (vis < 128) ? 0 : vis-128;
			int blue = (vis < 128) ? 128 : vis;
			colourz[i] = RGB(blue, green, red);
		}
		break;

	case 1: //green
		for (i=0; i<inc; i++)
		{
			vis=(255*i/inc);
			int red = (vis < 128) ? -vis+128 : 0;
			int green = (vis < 128) ? 0 : vis-128;
			int blue = (vis < 128) ? 128 : vis;
			colourz[i] = RGB(green, blue, red);
		}
		break;

	case 2: //blue
		for (i=0; i<inc; i++)
		{
			vis=(255*i/inc);
			int red = (vis < 128) ? -vis+128 : 0;
			int green = (vis < 128) ? 0 : vis-128;
			int blue = (vis < 128) ? 128 : vis;
			colourz[i] = RGB(red, green, blue);
		}
		break;

	case 3: //flame
		for (i=0; i<inc; i++)
		{
			vis=(255*i/inc);
			int red = (vis < 128) ? -vis+128 : 0;
			int green = (vis < 128) ? 0 : vis-128;
			int blue = (vis < 128) ? 128 : vis;
			colourz[i] = RGB(0xff, 0xff, 0xff);
		}
		break;

	case 4: //b/w
		for (i=0; i<inc; i++)
		{
			colourz[i] = RGB(0xff, 0xff, 0xff);
		}
		break;

	case 5: //classic
		for (i=0; i<inc; i++)
		{
			vis=(255*i/inc);
			int red = (vis < 128) ? ((vis) * 2) - 255  : 255;
			int green = (vis < 128) ? 255 : 255 - ((vis-128) * 2);
			colourz[i] = RGB(red, green, 0x00);
		}
		break;
	}

	return 1;
}


//we use onInit to activate our time
//and create our menu
int VUMeterWnd::onInit() {
  int retval = VUMETERWND_PARENT::onInit();

	GuiObject *guiObj = getGuiObject();
	ASSERT(guiObj != NULL);
	GuiObject *guiTxt = guiObj->guiobject_findObject("wasabi.statusbar.text");
	if (guiTxt) guiTxt->guiobject_setXmlParam("align", "center");
	guiObj->guiobject_setStatusText("DeJaVU Meter for Wasabi Media Player", 0);

//  setTimer(325, 10);
//	setSpeed(10);

	//main menu
	menu = new PopupMenu(this);
	menu->addCommand("Display Type", 0, 0, 1);
	menu->addCommand("DeJaVU Meter", 1, (PaintStyle==0));
	menu->addCommand("Spectrum AnalyzARRR", 2, (PaintStyle==1));
	menu->addCommand("Oscilliscope", 3, (PaintStyle==2));
	menu->addSeparator();
	menu->addCommand("Configuration", 0, 0, 1);

	//VU Meter menu
	PopupMenu *sub0 = new PopupMenu();
	sub0->addCommand("Normal", 101, (drawStyle[0]==0));
	sub0->addCommand("Fire", 102, (drawStyle[0]==1));
	sub0->addCommand("Solid Bars", 103, (drawStyle[0]==2));
	sub0->addSeparator();
	sub0->addCommand("From Left", 150, (VUDir==0));
	sub0->addCommand("Cross", 151, (VUDir==1));
	sub0->addCommand("From Right", 152, (VUDir==2));
	sub0->addSeparator();
	sub0->addCommand("Peaks", 140);
	sub0->addSeparator();
	sub0->addCommand("Red", 110, (colourStyle[0]==0));
	sub0->addCommand("Green", 111, (colourStyle[0]==1));
	sub0->addCommand("Blue", 112, (colourStyle[0]==2));
	sub0->addCommand("Flame", 113, (colourStyle[0]==3));
	sub0->addCommand("Black & White", 114, (colourStyle[0]==4));
	sub0->addCommand("Classic", 115, (colourStyle[0]==5));
	subMenu[0] = sub0;

	//AnalyzARRR menu
	PopupMenu *sub1 = new PopupMenu();
	sub1->addCommand("Normal", 201, (drawStyle[1]==0));
	sub1->addCommand("Fire", 202, (drawStyle[1]==1));
	sub1->addCommand("Solid Bars", 203, (drawStyle[1]==2));
	sub1->addSeparator();
	sub1->addCommand("Bars", 230);
	sub1->addCommand("Lines", 231);
	sub1->addCommand("Peaks", 240);
	sub1->addSeparator();
	sub1->addCommand("Red", 210, (colourStyle[1]==0));
	sub1->addCommand("Green", 211, (colourStyle[1]==1));
	sub1->addCommand("Blue", 212, (colourStyle[1]==2));
	sub1->addCommand("Flame", 213, (colourStyle[1]==3));
	sub1->addCommand("Black & White", 214, (colourStyle[1]==4));
	sub1->addCommand("Classic", 215, (colourStyle[1]==5));
	subMenu[1] = sub1;

	//Oscilliscore menu
	PopupMenu *sub2 = new PopupMenu();
/*	sub2->addCommand("Normal", 301);
	sub2->addCommand("Fire", 302);
	sub2->addCommand("Solid Lines", 303);
	sub2->addSeparator();*/
	sub2->addCommand("Solid", 330, (drawStyle[2]==0));
	sub2->addCommand("Lines", 331, (drawStyle[2]==1));
	sub2->addCommand("Dots", 332, (drawStyle[2]==2));
	sub2->addSeparator();
	sub2->addCommand("Red", 310, (colourStyle[2]==0));
	sub2->addCommand("Green", 311, (colourStyle[2]==1));
	sub2->addCommand("Blue", 312, (colourStyle[2]==2));
	sub2->addCommand("Flame", 313, (colourStyle[2]==3));
	sub2->addCommand("Black & White", 314, (colourStyle[2]==4));
	sub2->addCommand("Classic", 315, (colourStyle[2]==5));
	subMenu[2] = sub2;
	
	//add sub-menus to main menu
	menu->addSubMenu(sub0, "DeJaVU Meter");
	menu->addSubMenu(sub1, "Spectrum AnalyzARRR");
	menu->addSubMenu(sub2, "Oscilliscope");
/*
	{
		ComboBox *tmp;
		tmp = new ComboBox();
		tmp->addItem("hax");
		tmp->init(this);
		tmp->resize(0, 0, 100, 16);
	}
*/

  DCB PortDCB;					// Port Control Settings Structure
//  COMMTIMEOUTS CommTimeouts;	// Comm port timeout structure
	
	hPort = CreateFile ("com1", // Pointer to the name of the port
                      GENERIC_READ | GENERIC_WRITE ,
                                    // Access (read-write) mode
                      0,            // Share mode
                      NULL,         // Pointer to the security attribute
                      OPEN_EXISTING,// How to open the serial port
                      0,            // Port attributes
                      NULL);        // Handle to port with attribute
                                    // to copy

	if ( hPort == INVALID_HANDLE_VALUE ) {
    // Could not open the port.
	MessageBox (NULL, "error", "error", MB_OK);
    return FALSE;
  }

  PortDCB.DCBlength = sizeof(DCB);     

  // Get the default port setting information.
  GetCommState (hPort, &PortDCB);

  // Change the DCB structure settings.
  //PortDCB.BaudRate = 2400;              // Current baud 
	PortDCB.BaudRate = 115200;
  PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
  PortDCB.fParity = FALSE;               // Enable parity checking 
  PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
  PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
  PortDCB.fDtrControl = DTR_CONTROL_ENABLE; 
                                        // DTR flow control type 
  PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
  PortDCB.fTXContinueOnXoff = FALSE;     // XOFF continues Tx 
  PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
  PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
  PortDCB.fErrorChar = FALSE;           // Disable error replacement 
  PortDCB.fNull = FALSE;                // Disable null stripping 
  PortDCB.fRtsControl = DTR_CONTROL_ENABLE; 
                                        // RTS flow control 
  PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on 
                                        // error
  PortDCB.ByteSize = 8;                 // Number of bits/byte, 4-8 
  PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
  PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2 

  // Configure the port according to the specifications of the DCB 
  // structure.
  if (!SetCommState (hPort, &PortDCB))
  {
    // Could not create the read thread.
    MessageBox (NULL, TEXT("Unable to configure the serial port"), 
                TEXT("Error"), MB_OK);
    return FALSE;
  }

  EscapeCommFunction (hPort, SETDTR);
  EscapeCommFunction (hPort, SETRTS);


	COMMTIMEOUTS timeouts;
	ZeroMemory(&timeouts, sizeof(COMMTIMEOUTS));
	timeouts.ReadTotalTimeoutConstant = 10;
	SetCommTimeouts(hPort, &timeouts);

	SetupComm(hPort, 10000, 10000);

	DWORD read, write;
	char data[32];
	WriteFile(hPort, "   ", 3, &write, NULL);
	Sleep(50);
	ReadFile (hPort, data, sizeof(data), &read, 0);
	Sleep(50);
	WriteFile(hPort, "u", 1, &write, NULL);
	Sleep(50);
	data[0] = (char)0xFF;
	WriteFile(hPort, data, 1, &write, NULL);

	startQuickPaint();
  return retval;
}


//change display when it is clicked on
int VUMeterWnd::onLeftButtonDown(int x, int y)
{
  PaintStyle++;
  if (PaintStyle == 3) PaintStyle = 0;
	menu->checkCommand(1, 0);
	menu->checkCommand(2, 0);
	menu->checkCommand(3, 0);
	menu->checkCommand(PaintStyle+1, 1);
	onResize();
  return VUMETERWND_PARENT::onLeftButtonDown(x, y);
}


//when right button is pressed, bring up menu
int VUMeterWnd::onRightButtonDown(int x, int y)
{
	int i;
	int Item = menu->popAtMouse();
	switch (Item)
	{
	case 1:  case 2:  case 3:
		PaintStyle = Item - 1;
		for (i=1; i<4; i++)
			menu->checkCommand(i, 0);
		menu->checkCommand(Item, 1);
		onResize();
		break;

	case 101:  case 102:  case 103:
		drawStyle[0] = Item - 101;
		for (i=101; i<104; i++)
			subMenu[0]->checkCommand(i, 0);
		subMenu[0]->checkCommand(Item, 1);
		break;

	case 110:  case 111:  case 112:
	case 113:  case 114:  case 115:
		colourStyle[0] = Item - 110;
		for (i=110; i<116; i++)
			subMenu[0]->checkCommand(i, 0);
		subMenu[0]->checkCommand(Item, 1);
		onResize();
		break;

	case 150:  case 151:  case 152:
		VUDir = Item - 150;
		for (i=150; i<153; i++)
			subMenu[0]->checkCommand(i, 0);
		subMenu[0]->checkCommand(Item, 1);
		break;

	case 201:  case 202:  case 203:
		drawStyle[1] = Item - 201;
		for (i=201; i<204; i++)
			subMenu[1]->checkCommand(i, 0);
		subMenu[1]->checkCommand(Item, 1);
		break;

	case 210:  case 211:  case 212:
	case 213:  case 214:  case 215:
		colourStyle[1] = Item - 210;
		for (i=210; i<216; i++)
			subMenu[1]->checkCommand(i, 0);
		subMenu[1]->checkCommand(Item, 1);
		onResize();
		break;

	case 310:  case 311:  case 312:
	case 313:  case 314:  case 315:
		colourStyle[2] = Item - 310;
		for (i=310; i<316; i++)
			subMenu[2]->checkCommand(i, 0);
		subMenu[2]->checkCommand(Item, 1);
		onResize();
		break;

	case 330:  case 331:  case 332:
		drawStyle[2] = Item - 330;
		for (i=330; i<333; i++)
			subMenu[2]->checkCommand(i, 0);
		subMenu[2]->checkCommand(Item, 1);
		break;
	}
  return 1;
}


int VUMeterWnd::onPaint(Canvas *canvas) {
	return 0;
  PaintCanvas pbc;
  if (!canvas) {
    pbc.beginPaint(this);
    canvas = &pbc;
  }
	canvas->fillRect(&crect, RGB(0x00, 0x00, 0x00));
	VUMETERWND_PARENT::onPaint(canvas);
	return 1;
}


//hahahahaaa!!!  paint the window.
//now THIS is fun!!
//int VUMeterWnd::onPaint(Canvas *canvas) {
int  VUMeterWnd::onQuickPaint(BltCanvas *c, int w, int h, int newone) {
	return 0;
	if (!colourz) return 0;

	BltCanvas *canvas = c;
/*
  PaintCanvas pbc;
  if (!canvas) {
    pbc.beginPaint(this);
    canvas = &pbc;
  }
*/
//  VUMETERWND_PARENT::onPaint(canvas);

	VisData myVisData;
	int rt = api->core_getVisData(0, &myVisData, sizeof(VisData));

	int leftVUdata  = api->core_getLeftVuMeter(0);  // zero is the "main" core.  
	int rightVUdata = api->core_getRightVuMeter(0); // zero is the "main" core.  

	canvas->fillRect(&crect, RGB(0x00, 0x00, 0x00));

	switch (PaintStyle)
	{
	case 0: // vu meter
		{
			RECT left, rite;
			char str[8]="";

			{
				left.top = crect.top;
				left.bottom = (wHeight / 2) - 1 + crect.top;
				if (VUDir != 2)
				{
					left.left = crect.left;
					left.right = (wWidth * leftVUdata / 255) + crect.left;
				}
				else
				{
					left.left = crect.right - (wWidth * leftVUdata / 255);
					left.right = crect.right;
				}

				switch (drawStyle[0])
				{
				case 0:
					{
						int i;
						if (VUDir != 2)
							for (i=left.left; i<left.right; i++)
							{
								canvas->pushPen(colourz[i-left.left]);
								canvas->lineDraw(i, left.top, i, left.bottom);
								canvas->popPen();
							}
						else
							for (i=left.left; i<left.right; i++)
							{
								canvas->pushPen(colourz[left.right-i]);
								canvas->lineDraw(i, left.top, i, left.bottom);
								canvas->popPen();
							}
					}
					break;

				case 1:
					{
						int i;
						if (VUDir != 2)
							for (i=left.left; i<left.right; i++)
							{
								canvas->pushPen(colourz[wWidth-(left.right-i)]);
								canvas->lineDraw(i, left.top, i, left.bottom);
								canvas->popPen();
							}
						else
							for (i=left.left; i<left.right; i++)
							{
								canvas->pushPen(colourz[wWidth-(i-left.left)]);
								canvas->lineDraw(i, left.top, i, left.bottom);
								canvas->popPen();
							}
					}
					break;

				case 2:
					canvas->fillRect(&left, colourz[left.right-left.left]);
					break;
				}

				if (text) 
				{
					canvas->setTextColor(-(signed int)colourz[left.right-left.left]);
					canvas->textOutCentered(&left, _("Left"));
					SPRINTF( str, "%d%c", (100*leftVUdata)/255, '%' );
					left.right = left.left + canvas->getTextWidth(str);
					canvas->textOutCentered(&left, str);
				}
			}
  
			{
				rite.top = (wHeight / 2) + 1 + crect.top;
				rite.bottom = crect.bottom;
				if (VUDir != 0)
				{
					rite.left = crect.right - (wWidth * rightVUdata / 255);
					rite.right = crect.right;
				}
				else
				{
					rite.left = crect.left;
					rite.right = (wWidth * rightVUdata / 255) + crect.left;
				}

				switch (drawStyle[0])
				{
				case 0:
					{
						int i;
						if (VUDir != 0)
							for (i=rite.left; i<rite.right; i++)
							{
								canvas->pushPen(colourz[rite.right-i]);
								canvas->lineDraw(i, rite.top, i, rite.bottom);
								canvas->popPen();
							}
						else
							for (i=rite.left; i<rite.right; i++)
							{
								canvas->pushPen(colourz[i-rite.left]);
								canvas->lineDraw(i, rite.top, i, rite.bottom);
								canvas->popPen();
							}
					}
					break;

				case 1:
					{
						int i;
						if (VUDir != 0)
							for (i=rite.left; i<rite.right; i++)
							{
								canvas->pushPen(colourz[wWidth-(i-rite.left)]);
								canvas->lineDraw(i, rite.top, i, rite.bottom);
								canvas->popPen();
							}
						else
							for (i=rite.left; i<rite.right; i++)
							{
								canvas->pushPen(colourz[wWidth-(rite.right-i)]);
								canvas->lineDraw(i, rite.top, i, rite.bottom);
								canvas->popPen();
							}
					}
					break;

				case 2:
					canvas->fillRect(&rite, colourz[rite.right-rite.left]);
					break;
				}

				if (text)
				{
					canvas->setTextColor(-(signed int)colourz[rite.right-rite.left]);
					canvas->textOutCentered(&rite, _("Right"));
					SPRINTF( str, "%d%c", (100*rightVUdata)/255, '%' );
					rite.left = rite.right - canvas->getTextWidth(str);
					canvas->textOutCentered(&rite, str);
				}
			}

			canvas->setTextColor(RGB(127, 127, 127));
			if (text) canvas->textOutCentered(&crect, "DeJaVU Meter");
		} //end case 0
		break;


	case 1:  //analyzarrr
		{
			RECT rect;
			int i=0, i2=0;

			int bars = wWidth/6;

			if (rt != 0)
			{
				int Data=0;

				for (i=0; i<bars; i++)
				{
					rect.left = crect.left + (i*6);
					rect.right = rect.left + 5;
					rect.bottom = crect.bottom;
					Data = ((myVisData.spectrumData[0][576*i/bars] + myVisData.spectrumData[1][576*i/bars]) >> 1)*2;
					if (Data > 255) Data = 255;
					int barHeight = (wHeight*Data/255);
					
					switch (drawStyle[1])
					{
					case 0:
						for (i2=0; i2<barHeight; i2++)
						{
							int barLine = crect.bottom - i2;
							canvas->pushPen(colourz[i2]);
							canvas->lineDraw(rect.left, barLine, rect.right, barLine);
							canvas->popPen();
						}
						break;

					case 1:
						for (i2=0; i2<barHeight; i2++)
						{
							int barLine = crect.bottom - i2;
							canvas->pushPen( colourz[wHeight+(i2-barHeight)] );
							canvas->lineDraw(rect.left, barLine, rect.right, barLine);
							canvas->popPen();
						}
						break;

					case 2:
						rect.top = crect.bottom - barHeight;
						canvas->fillRect(&rect, colourz[barHeight]);
						break;
					}
				}
			}

			if (text)
			{
				rect.top = crect.top;
				rect.left = crect.left;
				rect.right = crect.right;
				rect.bottom = rect.top + canvas->getTextHeight();
				canvas->setTextColor(RGB(127, 127, 127));
				canvas->textOutCentered(&rect, "Spectrum AnalyzARRR");
			}

		}
		break;


	case 2: // osciliscope
		{
			int Mid = (wHeight/2)+crect.top;

			if (rt != NULL)
			{
				int i;
				char Data;

				if (drawStyle[2] == 0) {
					canvas->pushPen(colourz[0]);
					canvas->lineDraw(crect.left, Mid, crect.right, Mid);
					canvas->popPen();
				}

				for (i=crect.left; i<crect.right; i++)
				{
					int curVisData = 576*(i-crect.left)/wWidth;
					Data = ((myVisData.waveformData[0][curVisData] + myVisData.waveformData[1][curVisData]) >> 1);
					int barHeight = (wHeight*Data/128);

					COLORREF thePen;
					if (barHeight > wHeight/2-1) thePen = colourz[wHeight/2-1];
					else if (barHeight < -wHeight/2+1) thePen = colourz[wHeight/2-1];
					else if (Data<0) thePen = colourz[-barHeight];
					else  thePen = colourz[barHeight];

					canvas->pushPen(thePen);
					switch (drawStyle[2])
					{
					case 0:
						canvas->lineDraw(i, Mid, i, Mid+barHeight);
						canvas->popPen();
						break;

					case 1:
						if (i==crect.left) canvas->moveTo(i, Mid+barHeight);
						canvas->lineTo(i, Mid+barHeight);
						break;

					case 2:
						canvas->moveTo(i, Mid+barHeight);
						canvas->lineTo(i, Mid+barHeight+1);
						break;
					}
					canvas->popPen();

				} //end for

			} //end if
			else
			{
				canvas->pushPen(colourz[0]);
				canvas->lineDraw(crect.left, Mid, crect.right, Mid);
				canvas->popPen();
			}
	
			if (text)
			{
				RECT rect;
				rect.top = crect.top;
				rect.left = crect.left;
				rect.right = crect.right;
				rect.bottom = rect.top + canvas->getTextHeight();
				canvas->setTextColor(RGB(127, 127, 127));
				canvas->textOutCentered(&rect, "Oscilliscope");
			}

		} //end case 2
		break;
	} //end switch

  return 1;
}
