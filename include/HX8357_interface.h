#ifndef _HX_8357_INTERFACE_
#define _HX_8357_INTERFACE_

#define FLIPPED_CONNECTIONS    //In my setup the data connections are flipped
                               //D7 -> GPIO2
                               //D6 -> GPIO3 ....
                               //D0 -> GPIO10
                               //Comment out the defenition if the connections are not flipped on your setup
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <reent.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "pico/config.h"
#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico/bootrom.h"
#include "pico/mutex.h"
#include "pico/multicore.h"
#include "hardware/flash.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/rtc.h"
#include "hardware/divider.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"
#include "hardware/irq.h"
#include "hardware/structs/systick.h"
#include "glcdfont.c"

#define LOW  0
#define HIGH 1

#define LOWER_BYTE(x)   (uint8_t)(x & 0xFF)
#define HIGHER_BYTE(x)  (uint8_t)(x >> 8)

#define HX8357_NOP     0x00
#define HX8357_SWRESET 0x01
#define HX8357_RDDID   0x04
#define HX8357_RDDST   0x09

#define HX8357_RDPOWMODE  0x0A
#define HX8357_RDMADCTL  0x0B
#define HX8357_RDCOLMOD  0x0C
#define HX8357_RDDIM  0x0D
#define HX8357_RDDSDR  0x0F

#define HX8357_SLPIN   0x10
#define HX8357_SLPOUT  0x11
#define HX8357B_PTLON   0x12
#define HX8357B_NORON   0x13

#define HX8357_INVOFF  0x20
#define HX8357_INVON   0x21
#define HX8357_DISPOFF 0x28
#define HX8357_DISPON  0x29

#define HX8357_CASET   0x2A
#define HX8357_PASET   0x2B
#define HX8357_RAMWR   0x2C
#define HX8357_RAMRD   0x2E

#define HX8357B_PTLAR   0x30
#define HX8357_TEON  0x35
#define HX8357_TEARLINE  0x44
#define HX8357_MADCTL  0x36
#define HX8357_COLMOD  0x3A

#define HX8357_SETOSC 0xB0
#define HX8357_SETPWR1 0xB1
#define HX8357B_SETDISPLAY 0xB2
#define HX8357_SETRGB 0xB3
#define HX8357D_SETCOM  0xB6

#define HX8357B_SETDISPMODE  0xB4
#define HX8357D_SETCYC  0xB4
#define HX8357B_SETOTP 0xB7
#define HX8357D_SETC 0xB9

#define HX8357B_SET_PANEL_DRIVING 0xC0
#define HX8357D_SETSTBA 0xC0
#define HX8357B_SETDGC  0xC1
#define HX8357B_SETID  0xC3
#define HX8357B_SETDDB  0xC4
#define HX8357B_SETDISPLAYFRAME 0xC5
#define HX8357B_GAMMASET 0xC8
#define HX8357B_SETCABC  0xC9
#define HX8357_SETPANEL  0xCC


#define HX8357B_SETPOWER 0xD0
#define HX8357B_SETVCOM 0xD1
#define HX8357B_SETPWRNORMAL 0xD2

#define HX8357B_RDID1   0xDA
#define HX8357B_RDID2   0xDB
#define HX8357B_RDID3   0xDC
#define HX8357B_RDID4   0xDD

#define HX8357D_SETGAMMA 0xE0

#define HX8357B_SETGAMMA 0xC8
#define HX8357B_SETPANELRELATED  0xE9

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_SS  0x02
#define MADCTL_GS  0x01

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

#define ROTATION_ZERO_RAD           MADCTL_MX | MADCTL_BGR
#define ROTATION_HALF_RAD           MADCTL_MV | MADCTL_BGR
#define ROTATION_ONE_RAD            MADCTL_MY | MADCTL_BGR
#define ROTATION_THREE_HALVES_RAD   MADCTL_MX | MADCTL_MY | MADCTL_MV  | MADCTL_BGR

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF
#define PINK     0xEB3D

#define DC_COMMAND  0
#define DC_DATA     1
#define WAIT_FOR_CHIP_US 1  

template <class T> void swap( T& a, T& b );
class HX_8357_8Bit{
public:
    HX_8357_8Bit(int base_pin, int rst_pin, int rd_pin, int cs_pin, int wr_pin, int dc_pin, int width, int height);

    void init(),
    setDataBus(uint8_t val),
    
    fillScreen(uint16_t color),
    drawPixel(int16_t x, int16_t y, uint16_t color),

    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
    drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),

    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),
    fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),

    setRotation(uint8_t r),
    //invertDisplay(bool i),

    drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color),
    fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color),

    //drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color),
    //fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color),

    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),
    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),

    drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg),
    drawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h),

    setCursor(int16_t x, int16_t y),
    setCursor(int16_t x, int16_t y, uint8_t font),
    setTextColor(uint16_t color),
    setTextColor(uint16_t fgcolor, uint16_t bgcolor),
    setTextSize(uint8_t size),
    setTextFont(uint8_t font),
    setTextWrap(bool wrap),
    //setTextDatum(uint8_t datum),
    //setTextPadding(uint16_t x_width),
    print(uint8_t *str),
    writeChar(uint8_t c);

    int getWidth(), getHeight(), getRotation();
    uint8_t rotation;
private:
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x,
                            uint8_t size_y),
    pushColors(uint16_t *data, uint8_t len),
    pushColors(uint8_t  *data, uint32_t len),
    strobeColor(uint16_t color, uint16_t len),
    writecommand(uint8_t command),
    writedata(uint8_t data),
    setAddrWindow(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1),
    pushColor(uint16_t color, uint32_t len);

    int base_pin, rst_pin, rd_pin, cs_pin, wr_pin, dc_pin,
    width, height;
    uint16_t textcolor, textbgcolor, cursor_x, cursor_y;
    uint8_t  ptr_to_null[1], textsize_x, textsize_y;
    uint32_t data_bus_mask;
};


#ifdef FLIPPED_CONNECTIONS
const uint8_t reverse_lookup_table[256] = {0x0, 
0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 
0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 0x8, 
0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 
0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8, 0x4, 
0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 
0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 0xc, 
0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 
0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 0x2, 
0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 
0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 0xa, 
0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 
0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa, 0x6, 
0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 
0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6, 0xe, 
0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 
0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 0x1, 
0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 
0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1, 0x9, 
0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 
0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 0x5, 
0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 
0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 0xd, 
0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 
0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd, 0x3, 
0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 
0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 0xb, 
0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 
0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 0x7, 
0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 
0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7, 0xf, 
0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 
0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff, 
};
#endif //FLIPPED_CONNECTIONS


#endif //_HX_8357_INTERFACE_