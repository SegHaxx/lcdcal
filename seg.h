#define SEG_W 32
#define SEG_H 30

static uint16_t seg[30][4]={
	{0x0060,0x0060,0x0300,0x0300},
	{0x0050,0x0070,0x0500,0x0700},
	{0x0048,0x0078,0x0900,0x0f00},
	{0x0044,0x007c,0x1100,0x1f00},
	{0x0042,0x007e,0x2100,0x3f00},
	{0x0041,0x007f,0x4100,0x7f00},
	{0x0040,0x007f,0x8100,0xff00},
	{0x0040,0x007f,0x0100,0xff00},
	{0x007b,0x007f,0xc100,0xff00},
	{0x0084,0x00ff,0x2100,0xff00},
	{0x7fa5,0x7fff,0x21e0,0xffe0},
	{0xf084,0xffff,0x2040,0xffc0},
	{0xf07b,0xffff,0xc080,0xff80},
	{0xe000,0xffff,0x0040,0xffc0},
	{0x8000,0xffff,0x01e0,0xffe0},
	{0x8020,0xffff,0x0100,0xff00},
	{0x7fc0,0x7fff,0x01f0,0xfff0},
	{0x2000,0x3fff,0x0008,0xfff8},
	{0x1f80,0x1fff,0x0004,0xfffc},
	{0x0040,0x007f,0x0004,0xfffc},
	{0x0040,0x007f,0x0104,0xfffc},
	{0x0040,0x007f,0x81c2,0xfffe},
	{0x0040,0x007f,0x813e,0xff3e},
	{0x0040,0x007f,0x8100,0xff00},
	{0x01c3,0x01ff,0x8100,0xff00},
	{0x0204,0x03ff,0x0100,0xff00},
	{0x0489,0x07ff,0x0100,0xff00},
	{0x052a,0x07ff,0x4100,0xff00},
	{0x052a,0x07ff,0x4100,0xff00},
	{0x03ff,0x03ff,0xfe00,0xfe00}};

static void printseg(void){
	for(int y=0;y<SEG_H;y+=2){
		char buf[SEG_W+2];
		char* ptr=buf;
		for(int x=0;x<4;x+=2){
			int16_t a =(int16_t)seg[y][x];
			int16_t b =(int16_t)seg[y+1][x];
			int16_t am=(int16_t)seg[y][x+1];
			int16_t bm=(int16_t)seg[y+1][x+1];
			for(int i=0;i<16;++i){
				static char foo[]={' ',0xdf,0xdc,0xdb,0xb1};
				int index=4;
				if((am<0)||(bm<0)){
					index=(a<0)|((b<0)<<1);
				}
				*ptr++=foo[index];
				a<<=1;
				b<<=1;
				am<<=1;
				bm<<=1;
			}
		}
		buf[SEG_W-2]='\n';
		buf[SEG_W-1]=0;
		_outtext(buf);
	}
}
