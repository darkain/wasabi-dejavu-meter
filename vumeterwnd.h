#ifndef _VUMETERWND_H
#define _VUMETERWND_H

//#include "../../bfc/virtualwnd.h"
#include "../../bfc/wnds/buttwnd.h"
#include "../../bfc/wnds/qpaintwnd.h"
//#include "../../bfc/timerclient.h"
#include "../../common/nsGUID.h"
#include "../../common/popup.h"

#define VUMETERWND_PARENT QuickPaintWnd


typedef struct {
	enum { LEFT = 0, RIGHT = 1 };
	unsigned char spectrumData[2][576];
	char waveformData[2][576];
} VisData;


class VUMeterWnd : public VUMETERWND_PARENT {
public:
  VUMeterWnd();
  virtual ~VUMeterWnd();

  static const char *getWindowTypeName();
  static GUID getWindowTypeGuid();
  static void setIconBitmaps(ButtonWnd *button);    

	int getPreferences(int what);
  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
  virtual int onLeftButtonDown(int x, int y);
  virtual int onRightButtonDown(int x, int y);
	virtual int onResize();
//  void timerCallback(int id);

  virtual int onQuickPaint(BltCanvas *c, int w, int h, int newone); // return 1 if your content has changed, or 0 to cancel update of your buffer to the window

	//XML Stuff
	virtual int setXmlParam(const char *name, const char *strvalue); // receives from system

	virtual void timerclient_timerCallback(int id);

private:
	int PaintStyle;
	int drawStyle[3];
	int colourStyle[3];
	COLORREF *colourz;
	bool text;
	int VUDir;

	RECT crect;
	int wWidth;
	int wHeight;

	PopupMenu *menu;
	PopupMenu *subMenu[3];

	HANDLE hPort;

	char sendData[33];
};


// XuiObject Service
extern char VUMeterWndXuiObjectStr[];
extern char VUMeterWndXuiSvcName[];
class VUMeterWndXuiSvc : public XuiObjectSvc<VUMeterWnd, 
VUMeterWndXuiObjectStr, VUMeterWndXuiSvcName> {};


#endif _VUMETERWND_H


/*
			#define numColourz 20
			COLORREF colourz0[numColourz] = 
			{
				RGB(0xA8, 0x00, 0x99),
				RGB(0xAF, 0x00, 0x88),
				RGB(0xB8, 0x00, 0x77),
				RGB(0xBF, 0x00, 0x66),
				RGB(0xC8, 0x00, 0x55),
				RGB(0xCF, 0x00, 0x44),
				RGB(0xD8, 0x00, 0x33),
				RGB(0xDF, 0x00, 0x22),
				RGB(0xF8, 0x00, 0x11),
				RGB(0xFF, 0x00, 0x00),

				RGB(0xFF, 0x11, 0x11),
				RGB(0xFF, 0x22, 0x22),
				RGB(0xFF, 0x33, 0x33),
				RGB(0xFF, 0x44, 0x44),
				RGB(0xFF, 0x55, 0x55),
				RGB(0xFF, 0x66, 0x66),
				RGB(0xFF, 0x77, 0x77),
				RGB(0xFF, 0x88, 0x88),
				RGB(0xFF, 0x99, 0x99),
				RGB(0xFF, 0xAA, 0xAA),
			};

			COLORREF colourz1[numColourz] = 
			{
				RGB(0x99, 0xA8, 0x00),
				RGB(0x88, 0xAF, 0x00),
				RGB(0x77, 0xB8, 0x00),
				RGB(0x66, 0xBF, 0x00),
				RGB(0x55, 0xC8, 0x00),
				RGB(0x44, 0xCF, 0x00),
				RGB(0x33, 0xD8, 0x00),
				RGB(0x22, 0xDF, 0x00),
				RGB(0x11, 0xF8, 0x00),
				RGB(0x00, 0xFF, 0x00),

				RGB(0x11, 0xFF, 0x11),
				RGB(0x22, 0xFF, 0x22),
				RGB(0x33, 0xFF, 0x33),
				RGB(0x44, 0xFF, 0x44),
				RGB(0x55, 0xFF, 0x55),
				RGB(0x66, 0xFF, 0x66),
				RGB(0x77, 0xFF, 0x77),
				RGB(0x88, 0xFF, 0x88),
				RGB(0x99, 0xFF, 0x99),
				RGB(0xAA, 0xFF, 0xAA),
			};

			COLORREF colourz2[numColourz] = 
			{
				RGB(0x00, 0x99, 0xA8),
				RGB(0x00, 0x88, 0xAF),
				RGB(0x00, 0x77, 0xB8),
				RGB(0x00, 0x66, 0xBF),
				RGB(0x00, 0x55, 0xC8),
				RGB(0x00, 0x44, 0xCF),
				RGB(0x00, 0x33, 0xD8),
				RGB(0x00, 0x22, 0xDF),
				RGB(0x00, 0x11, 0xF8),
				RGB(0x00, 0x00, 0xFF),

				RGB(0x11, 0x11, 0xFF),
				RGB(0x22, 0x22, 0xFF),
				RGB(0x33, 0x33, 0xFF),
				RGB(0x44, 0x44, 0xFF),
				RGB(0x55, 0x55, 0xFF),
				RGB(0x66, 0x66, 0xFF),
				RGB(0x77, 0x77, 0xFF),
				RGB(0x88, 0x88, 0xFF),
				RGB(0x99, 0x99, 0xFF),
				RGB(0xAA, 0xAA, 0xFF),
			};

			COLORREF colourz3[numColourz] = 
			{
				RGB(0x99, 0x00, 0x99),
				RGB(0x99, 0x00, 0x88),
				RGB(0x99, 0x00, 0x77),
				RGB(0x99, 0x00, 0x66),
				RGB(0x99, 0x00, 0x55),
				RGB(0x99, 0x11, 0x44),
				RGB(0x99, 0x22, 0x33),
				RGB(0x9F, 0x33, 0x22),
				RGB(0xA8, 0x44, 0x11),
				RGB(0xAF, 0x55, 0x00),

				RGB(0xB8, 0x66, 0x00),
				RGB(0xBF, 0x77, 0x00),
				RGB(0xC8, 0x88, 0x00),
				RGB(0xCF, 0x99, 0x00),
				RGB(0xD8, 0xAA, 0x00),
				RGB(0xDF, 0xBB, 0x00),
				RGB(0xE8, 0xCC, 0x00),
				RGB(0xEF, 0xDD, 0x00),
				RGB(0xF8, 0xEE, 0x00),
				RGB(0xFF, 0xFF, 0x00),
			};

			COLORREF colourz4[numColourz] = 
			{
				RGB(0x00, 0x00, 0x00),
				RGB(0x11, 0x11, 0x11),
				RGB(0x22, 0x22, 0x22),
				RGB(0x33, 0x33, 0x33),
				RGB(0x44, 0x44, 0x44),
				RGB(0x55, 0x55, 0x55),
				RGB(0x66, 0x66, 0x66),
				RGB(0x77, 0x77, 0x77),
				RGB(0x88, 0x88, 0x88),
				RGB(0x99, 0x99, 0x99),

				RGB(0xAA, 0xAA, 0xAA),
				RGB(0xBB, 0xBB, 0xBB),
				RGB(0xCC, 0xCC, 0xCC),
				RGB(0xDD, 0xDD, 0xDD),
				RGB(0xEE, 0xEE, 0xEE),
				RGB(0xFF, 0xFF, 0xFF),
				RGB(0xFF, 0xFF, 0xFF),
				RGB(0xEE, 0xEE, 0xEE),
				RGB(0xDD, 0xDD, 0xDD),
				RGB(0xCC, 0xCC, 0xCC),
			};

			COLORREF colourz[numColourz];
			for (i=0; i<numColourz; i++)
			{
				switch (colourStyle)
				{
				case 1:
					colourz[i] = colourz1[i];
					break;

				case 2:
					colourz[i] = colourz2[i];
					break;

				case 3:
					colourz[i] = colourz3[i];
					break;

				case 4:
					colourz[i] = colourz4[i];
					break;

				default:
					colourz[i] = colourz0[i];
					break;
				}
			}

*/




/*
  switch (PaintStyle)
  {
	case 0:
		{
			char str[8]="";

			RECT crect = clientRect();
			RECT left, rite;
			invalidRect = crect;
		  
 
			canvas->fillRect(&crect, RGB(0x00, 0x00, 0x00));
			
			{
				left.top = crect.top;
				left.left = crect.left;
				left.right = ((crect.right - crect.left) * leftVUdata / 255 ) + crect.left;
				left.bottom = ((crect.bottom - crect.top) / 2) - 1 + crect.top;
				int green = (leftVUdata < 128) ? 255 : 255 - ((leftVUdata-128) * 2);
				int red = (leftVUdata < 128) ? 255 - ((leftVUdata) * 2) : 255;
				canvas->fillRect(&left, RGB(red, green, 0x00));

				canvas->setTextColor(RGB(255, 128, 0));
				canvas->textOutCentered(&left, _("Left"));
				SPRINTF( str, "%d%c", (100*leftVUdata)/255, '%' );
				left.right = left.left + canvas->getTextWidth(str);
				canvas->textOutCentered(&left, str);
			}
  
			{
				rite.top = ((crect.bottom - crect.top) / 2) + 1 + crect.top;
				rite.left = crect.left;
				rite.right = ((crect.right - crect.left) * rightVUdata / 255 ) + crect.left;
				rite.bottom = crect.bottom;
				int green = (rightVUdata < 128) ? 255 : 255 - ((rightVUdata-128) * 2);
				int red = (rightVUdata < 128) ? 255 - ((rightVUdata) * 2) : 255;
				canvas->fillRect(&rite, RGB(red, green, 0x00));

				canvas->setTextColor(RGB(0, 128, 255));
				canvas->textOutCentered(&rite, _("Right"));
				SPRINTF( str, "%d%c", (100*rightVUdata)/255, '%' );
				rite.right = rite.left + canvas->getTextWidth(str);
				canvas->textOutCentered(&rite, str);
			}




			canvas->setTextColor(RGB(127, 127, 127));
			canvas->textOutCentered(&crect, "VU Meter");
		} break;


		
//  int getVisData(void *dataptr, int sizedataptr);
	case 1:
		{
			RECT crect = clientRect();
			RECT rect;
//			unsigned char *data;
			int i=0, i2=0;
			int dataAmount = (crect.right - crect.left)/6;

			invalidRect = crect;

//			data = (unsigned char*)MALLOC(dataAmount);

			canvas->fillRect(&crect, RGB(0x00, 0x00, 0x00));


			if (rt != 0)
			{
				int Data=0;
				for (i=0; i<dataAmount; i++)
				{
					rect.left = crect.left + (i*6);
					rect.right = rect.left + 5;
					Data = ((myVisData.spectrumData[0][576*i/dataAmount] + myVisData.spectrumData[1][576*i/dataAmount]) >> 1)*2;
					if (Data > 255) Data = 255;

					for (i2=1; i2<numColourz+1; i2++)
					{
						if ( Data >= (i2*(255/numColourz)) )
						{
							rect.bottom = crect.bottom - ((i2-1)*((crect.bottom-crect.top)/numColourz));
							rect.top = rect.bottom - ((crect.bottom - crect.top) / numColourz );
							
							switch (drawStyle)
							{
							case 0:
							canvas->fillRect(&rect, colourz[i2-1]);
								break;
							case 1:
								canvas->fillRect(&rect, colourz[ -(Data*numColourz/256)+i2+numColourz-2 ]);
							break;
							case 2:
								canvas->fillRect(&rect, colourz[ (Data*numColourz/256) ]);
								break;
							}
						}
						
						else if ( Data > ((i2-1)*(255/numColourz)) )
						{
							rect.bottom = crect.bottom - ((i2-1)*((crect.bottom-crect.top)/numColourz));
							rect.top = crect.bottom - ((crect.bottom - crect.top) * Data / 255 );
				
							switch (drawStyle)
							{
							case 0:
							canvas->fillRect(&rect, colourz[i2-1]);
								break;
							case 1:
								canvas->fillRect(&rect, colourz[ -(Data*numColourz/256)+i2+numColourz-2 ]);
								break;
							case 2:
								canvas->fillRect(&rect, colourz[ (Data*numColourz/256) ]);
								break;
							}
						}
					}
				}
			}

			rect.top = crect.top;
			rect.left = crect.left;
			rect.right = crect.right;
			rect.bottom = rect.top + canvas->getTextHeight("Spectrum AnalyzARRR");
			canvas->setTextColor(RGB(127, 127, 127));
			canvas->textOutCentered(&rect, "Spectrum AnalyzARRR");

			
//			FREE(data);

		} break;
  }*/

/*
  int leftVUdata = api->core_getLeftVuMeter(0); // zero is the "main" core.  
  int rightVUdata = api->core_getRightVuMeter(0); // zero is the "main" core.  

  int h = crect.bottom - crect.top;
  int w = crect.right - crect.left;

  int vu_height = h / 2;

  if (leftVUdata) {
    RECT leftvu;
    leftvu.bottom = crect.bottom - (h / 4);  // One quarter of the window height from the bottom.
    leftvu.left = crect.left + 10;
    leftvu.right = leftvu.left + 20;
    int leftvu_h = (vu_height * leftVUdata) / 256;  // if you tell me to shift 8, I'll say "yes, but YOU know that.  does everyone?"
    leftvu.top = (leftvu.bottom - leftvu_h) + 2;
    int green = (leftVUdata < 128) ? 255 : 255 - ((leftVUdata-128) * 2);
    int red = (leftVUdata < 128) ? 255 - ((leftVUdata) * 2) : 255;
    canvas->fillRect(&leftvu, RGB(red, green, 0x00));
  }

  if (rightVUdata) {
    RECT rightvu;
    rightvu.bottom = crect.bottom - (h / 4);  // One quarter of the window height from the bottom.
    rightvu.right = crect.right - 10;
    rightvu.left = rightvu.right - 20;
    int rightvu_h = (vu_height * rightVUdata) / 256;  // if you tell me to shift 8, I'll say "yes, but YOU know that.  does everyone?"
    rightvu.top = (rightvu.bottom - rightvu_h) + 2;
    int green = (rightVUdata < 128) ? 255 : 255 - ((rightVUdata-128) * 2);
    int red = (rightVUdata < 128) ? 255 - ((rightVUdata) * 2) : 255;
    canvas->fillRect(&rightvu, RGB(red, green, 0x00));
  }



  typedef struct {
    enum {
      LEFT = 0,
      RIGHT = 1
    };
    unsigned char spectrumData[2][576];
    char waveformData[2][576];
  } VisData;

  VisData myVisData;
  int retval = api->core_getVisData(0, &myVisData, sizeof(VisData));

  //spectrum
  if (retval) {
    canvas->pushPen(RGB(0xFF, 0xFF, 0xFF));
    float dataW = (float)(w - 80); // figure out the width, in pixels, we're going to draw to.
    float pointW = dataW / 576.0F; // figure out the width, in fractional pixels, for each datapoint.
    int dataH = h;  // total height of the complex vis data area we're going to blit into.
    int sectionH = dataH / 5; // 5 sections, Wave & Spect, L & R channels, and one more to center it.
    int startPointX = 40; // the X value to begin drawing at.
    int startPointY = (sectionH / 2); // the Y value to begin drawing at.
    int zeroY; // the 0 point on the y axis for our line rendering for each section.
    int lastX = 0, lastY = 0, thisX = 0, thisY = 0; // the line draw coords
    int i;

    //left channel
    zeroY = startPointY + sectionH; // bottom of the first section, draw upward.
    lastX = startPointX;
    lastY = zeroY;
    for (i = 0; i < 576; i++) {
      thisX = startPointX + (int)(pointW * (float)i);  
      thisY = zeroY - (((myVisData.spectrumData[VisData::LEFT][i]) * sectionH) / 256);
      canvas->lineDraw(lastX, lastY, thisX, thisY);
      lastX = thisX;
      lastY = thisY;
    }   

    //right channel
    zeroY = startPointY + (sectionH * 2); // bottom of the second section, draw upward.
    lastX = startPointX;
    lastY = zeroY;
    for (i = 0; i < 576; i++) {
      thisX = startPointX + (int)(pointW * (float)i);  
      thisY = zeroY - (((myVisData.spectrumData[VisData::RIGHT][i]) * sectionH) / 256);
      canvas->lineDraw(lastX, lastY, thisX, thisY);
      lastX = thisX;
      lastY = thisY;
    }   


  //wave form

    // Left channel
    zeroY = startPointY + (sectionH * 3); // middle of the third section, draw up and down.
    lastX = startPointX;
    lastY = zeroY;
    for (i = 0; i < 576; i++) {
      thisX = startPointX + (int)(pointW * (float)i);  
      thisY = zeroY - (((myVisData.waveformData[VisData::LEFT][i]) * sectionH) / 128);
      canvas->lineDraw(lastX, lastY, thisX, thisY);
      lastX = thisX;
      lastY = thisY;
    }   

    // Right channel
    zeroY = startPointY + (sectionH * 4); // middle of the fourth section, draw up and down.
    lastX = startPointX;
    lastY = zeroY;
    for (i = 0; i < 576; i++) {
      thisX = startPointX + (int)(pointW * (float)i);  
      thisY = zeroY - (((myVisData.waveformData[VisData::RIGHT][i]) * sectionH) / 128);
      canvas->lineDraw(lastX, lastY, thisX, thisY);
      lastX = thisX;
      lastY = thisY;
    }   

    canvas->popPen();
  }
*/