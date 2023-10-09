CFLAGS=-za99 -oahnxs -ei -zp8 -0 -oe=5
CC=wcl

BUILD_DATE=$(shell date -u +'%y%W')
CFLAGS+=-DBUILD_DATE=\"$(BUILD_DATE)\"
CFLAGS+=-DGIT_HASH=\"$(shell git describe --always --dirty)\"

src=$(wildcard *.c)
obj=$(src:.c=.o)

TARGETS=LCDCAL.EXE BOLDTEST.EXE

.PHONY: all clean
all: LCDCAL.EXE
clean:
	rm -f $(TARGETS) $(obj)

LCDCAL.EXE: lcdcal.c bios.h seg.h
	$(CC) $(CFLAGS) $< -fe=$@
	wdis $(<:.c=.o) -l=$(<:.c=.s)
	@du -b $(<:.c=.o)
	@upx --best --8086 $@

BOLDTEST.EXE: boldtest.c bios.h
	$(CC) $(CFLAGS) $< -fe=$@
	wdis $(<:.c=.o) -l=$(<:.c=.s)
	@upx --best --8086 $@

LCDCAL.img: LCDCAL.EXE BOLDTEST.EXE
	@rm -f $@
	mkdosfs -n LCDCAL -C $@ 360
	mcopy -v -m -i $@ $^ ::/

.PHONY: zip
zip: LCDCAL$(BUILD_DATE).zip

LCDCAL$(BUILD_DATE).zip: LCDCAL.EXE README.TXT
	@rm -f $@
	@7za a -mx9 -bb1 $@ $^
