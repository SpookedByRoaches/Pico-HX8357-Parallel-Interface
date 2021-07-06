

#include "HX8357_interface.h"


HX_8357_8Bit::HX_8357_8Bit(int base_pin, int rst_pin, int rd_pin, int cs_pin, int wr_pin, int dc_pin, int width, int height):
                           base_pin(base_pin), rst_pin(rst_pin), rd_pin(rd_pin), cs_pin(cs_pin), wr_pin(wr_pin),
                           dc_pin(dc_pin), width(width), height(height)
{
    *ptr_to_null = 0;
    data_bus_mask = (0xFFul) << base_pin;
    textsize_x = 1;
    textsize_y = 1;
    cursor_x = 0;
    cursor_y = 0;
    textcolor = 0xFFFF;
    textbgcolor = 0x0000;
}

/*
 * Function Name: init()
 * Description: 
 */
void HX_8357_8Bit::init()
{
    //This could be used in the future to take advantage of the PIO hardrware
    //As of now, just setting the GPIO mask is faster
    /*data_bus_io = pio0;
    offset = pio_add_program(data_bus_io, &output_program);

    sm = pio_claim_unused_sm(data_bus_io, true);
    output_program_init(data_bus_io, sm, offset, base_pin , 8);
    pio_sm_set_clkdiv(data_bus_io, sm, 1);
    */
    uint32_t pins_mask = 0;
    pins_mask = (1ul << dc_pin) | (1ul << wr_pin) | (1ul << rd_pin) | (1ul << cs_pin) | (1ul << rst_pin) | (data_bus_mask);
    gpio_init_mask(pins_mask);
    

    gpio_set_dir_out_masked(pins_mask);

    gpio_put(dc_pin, HIGH);
    gpio_put(wr_pin, HIGH);
    gpio_put(rd_pin, HIGH);
    gpio_put(cs_pin, HIGH);

    gpio_put(rst_pin, HIGH);
    sleep_ms(50);
    gpio_put(rst_pin, LOW);
    sleep_ms(50);
    gpio_put(rst_pin, HIGH);
    gpio_put(cs_pin, LOW);
    writecommand(HX8357_SWRESET);
    gpio_put(cs_pin, HIGH);
    
    gpio_put(cs_pin, LOW);
    writecommand(HX8357_SLPOUT);
    gpio_put(cs_pin, HIGH);
    sleep_ms(20);
    gpio_put(cs_pin, LOW);
    writecommand(HX8357B_SETPOWER);
    writedata(0x07);
    writedata(0x42);
    writedata(0x18);
    gpio_put(cs_pin, HIGH);


    gpio_put(cs_pin, LOW);
    writecommand(HX8357B_SETVCOM);
    writedata(0x00);
    writedata(0x07);
    writedata(0x10);
    gpio_put(cs_pin, HIGH);

    gpio_put(cs_pin, LOW);
    writecommand(0xD2);
    writedata(0x01);
    writedata(0x02);
    gpio_put(cs_pin, HIGH);

    gpio_put(cs_pin, LOW);
    writecommand(HX8357B_SETPWRNORMAL);
    writedata(0x10);
    writedata(0x3B);
    writedata(0x00);
    writedata(0x02);
    writedata(0x11);
    gpio_put(cs_pin, HIGH);

    gpio_put(cs_pin, LOW);
    writecommand(HX8357B_SETDISPLAYFRAME);
    writedata(0x08);
    gpio_put(cs_pin, HIGH);

    gpio_put(cs_pin, LOW);
    writecommand(HX8357B_GAMMASET);
    writedata(0x00);
    writedata(0x32);
    writedata(0x36);
    writedata(0x45);
    writedata(0x06);
    writedata(0x16);
    writedata(0x37);
    writedata(0x75);
    writedata(0x77);
    writedata(0x54);
    writedata(0x0C);
    writedata(0x00);
    gpio_put(cs_pin, HIGH);

    gpio_put(cs_pin, LOW);
    writecommand(HX8357_MADCTL);
    writedata(ROTATION_ZERO_RAD);
    gpio_put(cs_pin, HIGH);

    gpio_put(cs_pin, LOW);
    writecommand(HX8357_COLMOD);
    writedata(0x55);
    gpio_put(cs_pin, HIGH);


    gpio_put(cs_pin, LOW);
    writecommand(HX8357_CASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);
    gpio_put(cs_pin, HIGH);

    gpio_put(cs_pin, LOW);
    writecommand(HX8357_PASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0xDF);
    gpio_put(cs_pin, HIGH);
    
    
    gpio_put(cs_pin, LOW);
    writecommand(HX8357_DISPON);
    gpio_put(cs_pin, HIGH);
}

/*
 * Function Name: setDataBus()
 * Description: Put the value of the command or data into the data bus for the
 *              Driver to read
 * Params: 
 *  val: The actual byte
 */
void HX_8357_8Bit::setDataBus(uint8_t val)
{
    uint32_t new_val = (uint32_t)val << base_pin;
    gpio_put_masked(data_bus_mask, new_val);
}

/*
 * Function Name: writecommand()
 * Description: Write a command to the Driver chip
 * Params: 
 *  command: command byte
 */
void HX_8357_8Bit::writecommand(uint8_t command)
{
    gpio_put(dc_pin, DC_COMMAND);
    #ifdef FLIPPED_CONNECTIONS
    setDataBus(reverse_lookup_table[command]);
    #else
    setDataBus(command);
    #endif //FLIPPED_CONNECTIONS
    gpio_put(wr_pin, HIGH);
    gpio_put(wr_pin, LOW);
}

/*
 * Function Name: writedata()
 * Description: Write a byte of data to the Driver chip
 * Params: 
 *  data: data byte
 */
void HX_8357_8Bit::writedata(uint8_t data)
{
    gpio_put(dc_pin, DC_DATA);
    #ifdef FLIPPED_CONNECTIONS
    setDataBus(reverse_lookup_table[data]);
    #else
    setDataBus(data);
    #endif //FLIPPED_CONNECTIONS
    gpio_put(wr_pin, HIGH);
    gpio_put(wr_pin, LOW);
}

/*
 * Function Name: setAddrWindow()
 * Description: Set the window frame for the driver to recieve color bytes
 * Params: 
 *  uint16_t x0: Starting column
 *  uint16_t y0: Starting row
 *  uint16_t x1: End column
 *  uint16_t y1: End row
 */
void HX_8357_8Bit::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t columns[4] = {HIGHER_BYTE(x0), LOWER_BYTE(x0), HIGHER_BYTE(x1), LOWER_BYTE(x1)};
    uint8_t pages[4]   = {HIGHER_BYTE(y0), LOWER_BYTE(y0), HIGHER_BYTE(y1), LOWER_BYTE(y1)};

    gpio_put(cs_pin, LOW);

    writecommand(HX8357_CASET);

    for (int i = 0; i < 4; i++)
        writedata(columns[i]);
    
    gpio_put(cs_pin, HIGH);

    gpio_put(cs_pin, LOW);

    writecommand(HX8357_PASET);

    for (int i = 0; i < 4; i++)
        writedata(pages[i]);
    
    gpio_put(cs_pin, HIGH);
}

/*
 * Function Name: pushColor()
 * Description: Write a color to the driver chip "len" times
 *  This is without the assumption that the cs_pin is low or that
 *  the chip received the RAMWR command
 * 
 * Params: 
 *  uint16_t color: RGB565 color value
 *  uint32_t len: Number of repetitions
 */
void HX_8357_8Bit::pushColor(uint16_t color, uint32_t len)
{
    gpio_put(cs_pin, LOW);

    writecommand(HX8357_RAMWR);

    color = ~color;

    while(len--){
        writedata(HIGHER_BYTE(color));
        writedata(LOWER_BYTE(color));
    }
    gpio_put(cs_pin, HIGH);
}

/*
 * Function Name: strobeColor()
 * Description: Write a color to the driver chip "len" times
 *              This assumes that the cs_pin is low and
 *              the RAMWR command has been received
 * Params: 
 *  uint16_t color: RGB565 color value
 *  uint32_t len: Number of repetitions
 */
void HX_8357_8Bit::strobeColor(uint16_t color, uint16_t len)
{
    color = ~color;

    while(len--){
        writedata(HIGHER_BYTE(color));
        writedata(LOWER_BYTE(color));
    }
}

/*
 * Function Name: fillScreen()
 * Description: Fills the screen with a color
 * Params: 
 *  uint16_t color: RGB565 color
 */
void HX_8357_8Bit::fillScreen(uint16_t color)
{
    fillRect(0, 0, width, height, color);
}

/*
 * Function Name: fillRect()
 * Description: Fills a rectangle at a certain location with a color
 * Params: 
 *  int16_t x: Starting column
 *  int16_t y: Starting row
 *  int16_t w: Rectangle width
 *  int16_t h: Rectangle height
 *  uint16_t color: RGB565 color
 */
void HX_8357_8Bit::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color)
{
    if ((x > width) || (y > height) || (w == 0) || (h == 0)) return;

    if ((x + w - 1) > width)  w = width  - x;
    if ((y + h - 1) > height) h = height - y;

    setAddrWindow(x, y, x + w - 1,  y + h - 1);

    pushColor(color, w*h);
}


/*
 * Function Name: drawPixel()
 * Description: Draw a single pixel at a certain location
 * Params: 
 *  int16_t x: Column
 *  int16_t y: Row
 *  uint16_t color: color
 */
void HX_8357_8Bit::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if ((x >= width) || (y >= height)) return;

    setAddrWindow(x, y, x, y);
    pushColor(color, 1);
}

/*
 * Function Name: drawFastVLine()
 * Description: Draw a vertical line optimized for speed
 * Params: 
 *  int16_t x: column
 *  int16_t y: row
 *  int16_t h: height
 *  uint16_t color: color
 */
void HX_8357_8Bit::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    if ((x >= width) || (y >= height)) return;
    if ((y + h - 1) >= height) h = height - y;

    setAddrWindow(x, y, x, y + h - 1);

    pushColor(color, h);
}

/*
 * Function Name: drawFastHLine()
 * Description: Draw a horizontal line optimized for speed
 * Params: 
 *  int16_t x: column
 *  int16_t y: row
 *  int16_t w: width
 *  uint16_t color: color
 */
void HX_8357_8Bit::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    if ((x >= width) || (y >= height)) return;
    if ((x + w - 1) > width) w = width - x;

    setAddrWindow(x, y, x + w - 1 , y);

    pushColor(color, w);
}

/*
 * Function Name: drawLine()
 * Description: Draw a line from any point to another using the bresenham
 *              algorithm
 * Params: 
 *  int16_t x0: First points column
 *  int16_t y0: First points row
 *  int16_t x1: Second points column 
 *  int16_t y1: Second points row
 *  uint16_t color: color
 */
void HX_8357_8Bit::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep){
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1){
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx = x1 - x0;
    int16_t dy = abs(y1 - y0);

    int16_t err = dx >> 1;
    int16_t ystep;
    int16_t xs = x0;
    int16_t dlen = 0;
    if (y0 < y1)
        ystep = 1;
    else
        ystep = -1;

    for (; x0 <= x1; x0++){
        if (steep)
            drawPixel(y0, x0, color);
        else
            drawPixel(x0, y0, color);
        err -= dy;
        if (err < 0){
            y0 += ystep;    
            err += dx;
        }
    }
}

/*
 * Function Name: setRotation()
 * Description: Set the rotation of the screen
 *              ROTATION_ZERO_RAD makes the top-left corner the 
 *              starting point
 *              The rotations are anti-clockwise
 * Params: 
 *  uint8_t m: Rotation value where each integer corresponds to a half radian rotation
 */
void HX_8357_8Bit::setRotation(uint8_t m)
{
    gpio_put(cs_pin, LOW);
    writecommand(HX8357_MADCTL);
    rotation = m % 4;
    if (rotation%2) {
        width = TFT_HEIGHT;
        height = TFT_WIDTH;
    } else {
        width = TFT_WIDTH;
        height = TFT_HEIGHT;
    }

    switch (rotation){
    case 0: // Portrait
        writedata(ROTATION_ZERO_RAD);
        break;
    case 1: // Landscape (Portrait + 90)
        writedata(ROTATION_HALF_RAD);
        break;
    case 2: // Inverter portrait
        writedata(ROTATION_ONE_RAD);
        break;
    case 3: // Inverted landscape
        writedata(ROTATION_THREE_HALVES_RAD);
        break;
    }
    gpio_put(cs_pin, HIGH); 
}

/*
 * Function Name: drawRect()
 * Description: Same as fillrect but only draws the outline
 * Params: (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
 */
void HX_8357_8Bit::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    if ((x > width) || (y > height) || (w == 0) || (h == 0)) return;

    if ((x + w - 1) > width)  w = width  - x;
    if ((y + h - 1) > height) h = height - y;

    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w, y, h, color);

    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h, w, color);
}

/*
 * Function Name: fillCircleHelper()
 * Description: Fills a quarter circle to help fillCircle and fillRoundRect
 * Params: 
 *  int16_t x0: center point column
 *  int16_t y0: center point row
 *  int16_t r: circle radius
 *  uint8_t cornername: The quadrant of the circle to draw
 *  int16_t delta: Offset from center-point, used for round-rects
 *  uint16_t color: color
 */
void HX_8357_8Bit::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -r*2;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;

    delta++;
    while (x < y){
        if (f >= 0){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (x < (y + 1)){
            if (cornername & 1)
                drawFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
            
            if (cornername & 2)
                drawFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
        }
        if (y != py) {
            if (cornername & 1)
                drawFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
            if (cornername & 2)
                drawFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
            py = y;
        }
        px = x;
    }
}

/*
 * Function Name: drawCircleHelper()
 * Description: 
 * Params: 
 *  int16_t x0: center point column
 *  int16_t y0: center point row
 *  int16_t r: circle radius
 *  uint8_t cornername: The quadrant of the circle to draw
 *  uint16_t color: color
 */
void HX_8357_8Bit::drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y){
        if (f >= 0){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4){
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2){
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8){
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1){
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

/*
 * Function Name: fillCircle()
 * Description: Draw a filled circle
 * Params: 
 *  int16_t x0: center point column
 *  int16_t y0: center point row
 *  int16_t r: circle radius
 *  uint16_t color: color
 */
void HX_8357_8Bit::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  drawFastVLine(x0, y0 - r, r*2 + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

/*
 * Function Name: drawCircle()
 * Description: Draw a non-filled circle
 * Params: 
 *  int16_t x0: center point column
 *  int16_t y0: center point row
 *  int16_t r: circle radius
 *  uint16_t color: color
 */
void HX_8357_8Bit::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y){
        if (f >= 0){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

/*
 * Function Name: drawRoundRect()
 * Description: Draw a rounded rectangle
 * Params: 
 *  int16_t x: Top left corner x coordinate
 *  int16_t y: Top left corner y coordinate 
 *  int16_t w: Width in pixels
 *  int16_t h: Height in pixels
 *  int16_t r: Radius of corner rounding
 *  uint16_t color: color
 */
 void HX_8357_8Bit::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                 int16_t r, uint16_t color) 
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
            r = max_radius;
    // smarter version
    drawFastHLine(x + r, y, w - 2 * r, color);         // Top
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    drawFastVLine(x, y + r, h - 2 * r, color);         // Left
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/*
 * Function Name: fillRoundRect()
 * Description: Fill a rounded rectangle
 * Params: 
 *  int16_t x: Top left corner x coordinate
 *  int16_t y: Top left corner y coordinate 
 *  int16_t w: Width in pixels
 *  int16_t h: Height in pixels
 *  int16_t r: Radius of corner rounding
 *  uint16_t color: color
 */
void HX_8357_8Bit::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                 int16_t r, uint16_t color) 
{
  int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
  if (r > max_radius)
    r = max_radius;
  // smarter version
  
  fillRect(x + r, y, w - 2 * r, h, color);
  // draw four corners
  fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

/*
 * Function Name: drawTriangle()
 * Description: Draw a triangle with the 3 provided corners
 * Params: 
 *  int16_t x0: First corner column
 *  int16_t y0: First corner row
 *  int16_t x1: Second corner column
 *  int16_t y1: Second corner row
 *  int16_t x2: Third corner column
 *  int16_t y2: Third corner row
 */
void HX_8357_8Bit::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color) 
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

/*
 * Function Name: fillTriangle()
 * Description: Fill a triangle with the 3 provided corners
 * Params: 
 *  int16_t x0: First corner column
 *  int16_t y0: First corner row
 *  int16_t x1: Second corner column
 *  int16_t y1: Second corner row
 *  int16_t x2: Third corner column
 *  int16_t y2: Third corner row
 */
void HX_8357_8Bit::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color) 
{

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1){
        swap(y0, y1);
        swap(x0, x1);
    }
    if (y1 > y2){
        swap(y2, y1);
        swap(x2, x1);
    }
    if (y0 > y1){
        swap(y0, y1);
        swap(x0, x1);
    }

    if (y0 == y2){ 
        a = b = x0;
        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        drawFastHLine(a, y0, b - a + 1, color);
        return;
    }

    int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
            dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1; // Include y1 scanline
    else
        last = y1 - 1; // Skip it

    for (y = y0; y <= last; y++){
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if (a > b)
            swap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for (; y <= y2; y++){
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (a > b)
            swap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }
}


/*
 * Function Name: drawBitmap()
 * Description: Draw a monochrome bitmap with the provided background and foreground
 *              colors
 * Params: 
 *  int16_t x: Starting column
 *  int16_t y: Starting row
 *  const uint8_t *bitmap: bitmap array pointer
 *  int16_t w: width of photo
 *  int16_t h: height of photo
 *  uint16_t color: foreground color
 *  uint16_t bg: background color
 */
void HX_8357_8Bit::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
    if (((w + x) > width) | ((h + y) > height)) return;
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    setAddrWindow(x, y, x + w - 1 , y + h - 1);

    gpio_put(cs_pin, LOW);

    writecommand(HX8357_RAMWR);

    for (int16_t j = 0; j < h; j++, y++){
        for (int16_t i = 0; i < w; i++){
            if (i & 7)
                byte <<= 1;
            else
                byte = bitmap[j * byteWidth + i / 8];
            strobeColor((byte & 0x80) ? color : bg, 1);
        }
    }
    gpio_put(cs_pin, HIGH);
}

/*
 * Function Name: drawRGBBitmap()
 * Description: Draw a bitmap with the provided array colors
 * Params: 
 *  int16_t x: Starting column
 *  int16_t y: Starting row
 *  const uint16_t *bitmap: bitmap array pointer
 *  int16_t w: width of photo
 *  int16_t h: height of photo
 */
void HX_8357_8Bit::drawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) 
{
    if (((w + x) > width) | ((h + y) > height))
        return;

    setAddrWindow(x, y, x + w - 1, y + h - 1);

    gpio_put(cs_pin, LOW);

    writecommand(HX8357_RAMWR);

    for (int16_t j = 0; j < h; j++){
        for (int16_t i = 0; i < w; i++){
            strobeColor(bitmap[j * w + i], 1);
        }
    }

    gpio_put(cs_pin, HIGH);
}

/*
 * Get the screen height
 */
int HX_8357_8Bit::getHeight()
{
    return height;
}

/*
 * Get the screen width
 */
int HX_8357_8Bit::getWidth()
{
    return width;
}

/*
 * Get the screen rotation
 */
int HX_8357_8Bit::getRotation()
{

    return rotation;
}

/*
 * Function Name: drawChar()
 * Description: Draws a character into the screen without updating the cursor
 * Params: 
 *  int16_t x: Starting column
 *  int16_t y: Starting row
 *  unsigned char c: Actual character
 *  uint16_t color: foreground color
 *  uint16_t bg: background color
 *  uint8_t size_x: magnification of the font horizontally
 *  uint8_t size_y: magnification of the font vertically
 */
void HX_8357_8Bit::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x,
                            uint8_t size_y)
{
    if ((x >= width) ||              // Clip right
        (y >= width) ||             // Clip bottom
        ((x + 6 * size_x - 1) < 0) || // Clip left
        ((y + 8 * size_y - 1) < 0))   // Clip top
        return;

    if ((c >= 176))
        c++; // Handle 'classic' charset behavior

    for (int8_t i = 0; i < 5; i++)
    { // Char bitmap = 5 columns
        uint8_t line = font[c * 5 + i];
        for (int8_t j = 0; j < 8; j++, line >>= 1)
        {
            if (line & 1)
            {
                if (size_x == 1 && size_y == 1)
                    drawPixel(x + i, y + j, color);
                else
                    fillRect(x + i * size_x, y + j * size_y, size_x, size_y,
                                  color);
            }
            else if (bg != color)
            {
                if (size_x == 1 && size_y == 1)
                    drawPixel(x + i, y + j, bg);
                else
                    fillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
            }
        }
    }
    if (bg != color)
    { // If opaque, draw vertical line for last column
        if (size_x == 1 && size_y == 1)
            drawFastVLine(x + 5, y, 8, bg);
        else
            fillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
    }
}

/*
 * Function Name: setCursor()
 * Description: Sets where characters would be written to the screen
 * Params: 
 *  int16_t x: column
 *  int16_t y: row
 */
void HX_8357_8Bit::setCursor(int16_t x, int16_t y)
{
    cursor_x = x;
    cursor_y = y;
}

/*
 * Function Name: setTextSize()
 * Description: Sets the magnification of the font
 * Params: 
 *  uint8_t size: value of the magnification
 */
void HX_8357_8Bit::setTextSize(uint8_t size)
{
    textsize_x = size;
    textsize_y = size;
}

/*
 * Function Name: setTextColor()
 * Description: Set the foreground text color
 * Params: 
 *  uint16_t color
 */
void HX_8357_8Bit::setTextColor(uint16_t color)
{
    textcolor = color;
}

/*
 * Function Name: setTextColor()
 * Description: Sets the foreground and background color of the text
 * Params:
 *  uint16_t fgcolor: foreground color
 *  uint16_t bgcolor: background color
 */
void HX_8357_8Bit::setTextColor(uint16_t fgcolor, uint16_t bgcolor)
{
    textcolor = fgcolor;
    textbgcolor = bgcolor;
}

/*
 * Function Name: writeChar()
 * Description: writes a character to the screen based on the parameters
 *              set previously and are in the object attributes. It also
 *              changes the cursor value for the next character to be written
 * Params: 
 *  uint8_t c: Character
 */
void HX_8357_8Bit::writeChar(uint8_t c) {
    if (c == '\n') {              
      cursor_x = 0;               
      cursor_y += textsize_y * 8; 
    } else if (c != '\r') {      
      if ((cursor_x + textsize_x * 6) > width) { 
        cursor_x = 0;                                       
        cursor_y += textsize_y * 8;
      }
      drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
               textsize_y);
      cursor_x += textsize_x * 6;
    }
}

/*
 * Function Name: print()
 * Description: similar to writeChar but with a null-terminated string
 * Params: 
 *  uint8_t *str: null-terminated string (VERY IMPORTANT TO HAVE IT NULL TERMINATED)
 */
void HX_8357_8Bit::print(uint8_t *str)
{
    uint8_t curChar;

    for (int i = 0; str[i] != '\0'; i++){
        writeChar(str[i]);
    }
}

template <class T> void swap( T& a, T& b ){
    T c(a); a=b; b=c;
}