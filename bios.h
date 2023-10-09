static void set_blink_or_boldCGA(void);
#pragma aux set_blink_or_boldCGA = \
"mov ax,40H "\
"mov es,ax "\
"mov dx,es:[063H] "\
"add dx,4 "\
"mov al,es:[065H] "\
"and al,0dfH      "\
"out dx,al        "\
"mov es:[065H],al "\
modify [ ax dx es ] ;

static void set_blink_or_boldEGA(unsigned char);
#pragma aux set_blink_or_boldEGA = \
"mov ax,0x1003 "\
"int 0x10 "\
modify [ ax bx cx ] \
parm [ bl ] ;

static void setborderCGA(unsigned char);
#pragma aux setborderCGA = \
"mov ah,0xb "\
"int 0x10 "\
modify [ ax bx cx ] \
parm [ bl ] ;

static void setborderEGA(unsigned char);
#pragma aux setborderEGA = \
"mov ax,0x1001 "\
"int 0x10 "\
modify [ ax bx cx ] \
parm [ bh ] ;
