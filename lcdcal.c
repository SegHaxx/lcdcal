// Copyright 2023 Seg <seg@haxxed.com>

#define BANNER "LCD Calibrator by Seg <seg@haxxed.com>"

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include <conio.h>
#include <graph.h>

#include "bios.h"
#include "seg.h"

static struct videoconfig vc;
static bool is_EGA=false;

static inline void do_help(void){
	_settextwindow(2,3,vc.numtextrows,vc.numtextcols);
	_settextposition(0,0);
	_outtext(BANNER "\n\n"
		"Press the auto-adjust button on your VGA LCD panel\n\n"
		"<space> to cycle mode\n"
		"A to test all modes\n"
		"B to toggle border\n\n"
		"X to test XGA 1024x768i\n"
		"(not supported on most panels)\n\n"
		"www.haxxed.com");
	_settextposition(vc.numtextrows-2,0);
	_outtext("build " BUILD_DATE " " GIT_HASH);
	_settextwindow(
			vc.numtextrows-15,vc.numtextcols-32,
			vc.numtextrows,vc.numtextcols);
	_settextposition(0,0);
	printseg();
}

static char* itoa_u16(char* dst,uint16_t n){
	char tmp[6];
	char* str=tmp+5;
	*str = 0;
	do{
		*--str = '0'+(n%10);
		n /= 10;
	} while (n > 0);
	while((*dst++=*str++));
	return dst-1;
}

static char mask_solid[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
static char mask_50   [8]={0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa};

static bool do_gr_test(short mode){
	if(!_setvideomode(mode)) return false;

	_getvideoconfig(&vc);
	static char buf[16];
	char* str=buf;
	str=itoa_u16(str,vc.numxpixels);
	--vc.numxpixels;
	*str++='x';
	str=itoa_u16(str,vc.numypixels);
	--vc.numypixels;
	*str++=' ';
	str=itoa_u16(str,vc.numcolors);

	// fill background
	_setfillmask(mask_50);
	_rectangle(_GFILLINTERIOR,0,0,vc.numxpixels,vc.numypixels);
	_rectangle(_GBORDER,0,0,vc.numxpixels,vc.numypixels);
	_setfillmask(mask_solid);

	// draw text
	short w=_getgtextextent(buf);
	short x=(vc.numxpixels-w)/2;
	struct _fontinfo f;
	_getfontinfo(&f);
	short h=f.pixheight;
	short y=(vc.numypixels-h)/2;
	_rectangle(_GFILLINTERIOR,x,y,x+w,y+h);
	_moveto(x+1,y+1);
	_setplotaction(_GXOR);
	_outgtext(buf);

	return true;
}

static unsigned short modes[]={
	_MRES256COLOR,_MRES16COLOR,_MRES4COLOR,       // 320x200
	_HRES16COLOR,_HRESBW,                        // 640x200
	_URES256COLOR,0,                            // 640x400
	_ERESCOLOR,0,                              // 640x350
	_VRES256COLOR,_VRES16COLOR,_VRES2COLOR,0, // 640x480
	_SVRES256COLOR,_SVRES16COLOR,0,0};       // 800x600
static unsigned short xgamodes[]={
	_XRES256COLOR,_XRES16COLOR,0,0};       // 1024x768
static unsigned short hercmode[]={
	_HERCMONO,0,0};

static inline bool do_gr_tests(unsigned short* mode,bool allmodes){
	while(1){
		if(!*mode) ++mode;
		if(!*mode){
			return true;
		}
		if(!do_gr_test(*mode++)){
			continue;
		}
		bool border_on=false;
		while(1){
			switch(toupper(getch())){
				case ' ':
				case '\t':
				case 'M':
					goto next;
				case 'B':
					border_on=!border_on;
					!is_EGA?
						setborderCGA(border_on?1:0):
						setborderEGA(border_on?0xf:0);
					break;
				case 27:
				case 'Q':
					return false;
				default:
					break;
			}
		}
next:
		if(!allmodes) while(*mode) ++mode;
	}
}

static inline void fillrow(short y){
	static char c[4]={0xb1,0xb1,0xb1,0xb1};
	_settextposition(y,0);
	for(int x=0;x<vc.numtextcols;x+=4){
		_outmem(c,4);
	}
}

int main(void){
	//while(1){
	//	unsigned short c=getch();
	//	printf("%c %d\n",c,c);
	//}
	_setvideomode(_TEXTC80);
	_getvideoconfig(&vc);

	unsigned short* modelist=modes;
	switch(vc.adapter){
		case _MDPA:
		case _HGC:
			modelist=hercmode;
		case _CGA:
		case _MCGA:
			set_blink_or_boldCGA();
			break;
		default:
			is_EGA=true;
			set_blink_or_boldEGA(0);
	}
	_setbkcolor(7);
	_settextcolor(16);

	// fill background
	_settextwindow(0,0,vc.numtextrows,vc.numtextcols);
	_wrapon(_GWRAPOFF);
	_displaycursor(_GCURSOROFF);
	for(int y=0;y<=vc.numtextrows;++y){
		fillrow(y);
	}

	// draw text
	int x=vc.numtextcols;
	int y=vc.numtextrows/2;
	_settextposition(y,(x-5)/2);
	_outtext("H for help");

	bool border_on=false;
	bool allmodes=false;
	while(1){
		switch(toupper(getch())){
			case 'A':
				allmodes=true;
			case ' ':
			case '\t':
			case 'M':
				if(!do_gr_tests(modelist,allmodes)) goto exit;
				// workaround for herc bug
				if(_HGC) _setvideomode(_DEFAULTMODE);
				return main();
			case 'B':
				border_on=!border_on;
				!is_EGA?
					setborderCGA(border_on?0x1f:0):
					setborderEGA(border_on?0x3f:0);
				break;
			case 'H':
			case '?':
			case '/':
			case ';':
				fillrow(y);
				do_help();
				break;
			case 'X':
				if(!do_gr_tests(xgamodes,false)) goto exit;
				return main();
			default:
				break;
			case 27:
			case 'Q':
				goto exit;
		}
	}

exit:
	_setvideomode(_DEFAULTMODE);
	return 0;
}
