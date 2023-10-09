LCD Calibrator by Seg <seg@haxxed.com>

Generates a high-contrast signal suitable for optimal calibration of LCD panels
with analog VGA input, capture devices and scan converters such as the
RetroTINK.

Find out what resolutions your display handles poorly or not at all!

Written in Open Watcom C, and will run on any DOS PC with 64k RAM. It uses the
Watcom graphics library which has good coverage of MDA/HGC/CGA/EGA/VGA and
early 8bit SVGA such as the ET4000AX. It knows nothing of VESA BIOS.

You begin in text mode, <Space> will cycle through a selection of key graphics
modes for quickly dialing in your panel or capture device.

Hit <A> at the text screen to exhaustively test all available resolutions and
color depths.

A fun fact that gets lost on modern LCD displays and most emulators, is that
VGA has a vestigial overscan border, for CGA/EGA compatibility. It is visible
during screen flashes in Doom for example. This border even exists in 800x600
SVGA resolution! Hit <B> to toggle the border on, then calibrate your monitor.
No panel I've tried handles this well, but hey it's there to experiment with.

<X> at the text screen to test 1024x768, early SVGA cards implement this as a
now unusual 43hz interlaced mode from the IBM 8514, which no LCD panel I have
tried supports. Later VESA BIOS cards may output a compatable non-interlaced
refresh rate.

Shout out to www.lagom.nl/lcd-test for schooling me on calibrating LCD panels
when I got my first flat panel back in 2009 and being my go-to calibrator ever
since.

If you're calibrating CRTs in DOS check out Video Test Bench:
https://github.com/unclejed613/video-test-bench-dos-freeware

  -- Seg <seg@haxxed.com>

https://github.com/SegHaxx/lcdcal
https://www.haxxed.com/
https://oldbytes.space/@Seg
