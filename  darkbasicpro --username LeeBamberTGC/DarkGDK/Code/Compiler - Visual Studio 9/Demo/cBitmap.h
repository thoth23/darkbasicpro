#include <stdio.h>
#include <math.h>
#include "darksdk.h"

class bitmap
{
	typedef struct
    {
		unsigned short bfType;           
		unsigned int   bfSize;           
		unsigned short bfReserved1;      
		unsigned short bfReserved2;      
		unsigned int   bfOffBits;        
	} bmpFHEAD;


	typedef struct bmpIHEAD
    {
		unsigned int   biSize;
		int            biWidth;
		int            biHeight;
		unsigned short biPlanes;
		unsigned short biBitCount;
		unsigned int   biCompression;
		unsigned int   biSizeImage;
		int            biXPelsPerMeter;
		int            biYPelsPerMeter;
		unsigned int   biClrUsed;
		unsigned int   biClrImportant;
	} bmpIHEAD;


	typedef struct bmpPAL
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} PAL;



	int width, height;
	
public:
	unsigned char *data;

	bitmap(void);

	bool create(int x, int y);
	bool load(char *filename);
	bool save(LPDIRECT3DSURFACE9 surface, char *filename);
	bool save(char *filename);

	int getwidth(void)	{ if (data) return width; else return -1;  };
	int getheight(void) { if (data) return height; else return -1; };

	void getcolor(int x, int y, BYTE &r, BYTE &g, BYTE &b);
	void setcolor(int x, int y, BYTE  r, BYTE  g, BYTE  b);

	~bitmap(void);
};