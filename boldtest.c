#include <conio.h>
#include <graph.h>

#include "bios.h"

inline void draw(void){
	struct videoconfig vc;
	_wrapon(_GWRAPOFF);
	_getvideoconfig(&vc);
	char c=0xb1;
	for(int y=0;y<=vc.numtextrows;++y){
		_setbkcolor(y);
		_settextposition(y,0);
		for(int x=0;x<vc.numtextcols;++x){
			_settextcolor(x);
			_outmem(&c,1);
		}
	}
}

int main(){
	_setvideomode(_TEXTC80);
	_displaycursor(_GCURSOROFF);

	static struct videoconfig vc;
	_getvideoconfig(&vc);
	switch(vc.adapter){
		case _MDPA:
		case _HGC:
		case _CGA:
		case _MCGA:
			set_blink_or_boldCGA();
			break;
		default:
			set_blink_or_boldEGA(0);
	}

	draw();
	unsigned char state=0;
	while(1){
		set_blink_or_boldEGA(state);
		getch();
		state^=1;
	}

	_settextrows(50);
	_setvideomode(_DEFAULTMODE);
	return 0;
}
