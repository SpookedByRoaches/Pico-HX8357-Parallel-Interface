#include "main.h"



HX_8357_8Bit tft(start_pin, RST_PIN, RD_PIN, CS_PIN, WR_PIN, DC_PIN, 320, 480);



int main() {
    stdio_init_all();
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_down(button_pin);

    tft.init();
    uint64_t begin_time = 0;
    uint64_t end_time = 0;
    long unsigned int time_elapsed = 0;

    tft.fillScreen(BLACK);

    uint8_t rot = 0;

    sleep_ms(2000);
    printf("Hello\n");
    while(true){
        drawGrid();
     
        block();
        tft.fillScreen(BLACK);
        block();
    }

}

void fillSquares()
{
    int x_val = 0;
    int y_val = 0;
    uint8_t curCol = 0;
    uint64_t begin_time = 0;
    uint64_t end_time = 0;
    long unsigned int time_elapsed = 0;
    for (int i = 0; i < 480*320/(20*20); i++){
        tft.fillRect(x_val, y_val, 20, 20, colors[curCol]);
        curCol = (curCol+1)%7;
        if (x_val == tft.getWidth()-20){
            x_val = 0;
            if (y_val == tft.getHeight() - 20)
                y_val = 0;

            else
                y_val += 20;
            
        }
        else{
            x_val += 20;
        }
    }
}

void drawGrid()
{
    int x_val = 0;
    int y_val = 0;

    for (int i = 0; i < tft.getWidth(); i += 20)
        tft.drawFastVLine(i, 0, tft.getHeight(), RED);

    for (int i = 0; i < tft.getHeight(); i += 20)
        tft.drawFastHLine(0, i, tft.getWidth(), RED);
}

void drawRays()
{
    for (int i = 0; i <= tft.getWidth(); i += 20){
        tft.drawLine(0, 0, i, tft.getHeight(), GREEN);
    }

    for (int i = 0; i <= tft.getHeight(); i += 20){
        tft.drawLine(0, 0, tft.getWidth(), i, RED);
    }
}

void drawSquares()
{
    int x_val = 0;
    int y_val = 0;
    uint8_t curCol = 0;

    for (int i = 0; i < 480*320/(20*20); i++){
        tft.drawRect(x_val, y_val, 19, 19, colors[curCol]);
        curCol = (curCol+1)%7;
        if (x_val == tft.getWidth()-20){
            x_val = 0;
            if (y_val == tft.getHeight() - 20)
                y_val = 0;

            else
                y_val += 20;
            
        }
        else{
            x_val += 20;
        }
    }
}

void drawRoundSquares()
{
    int x_val = 0;
    int y_val = 0;
    uint8_t curCol = 0;

    for (int i = 0; i < 480*320/(20*20); i++){
        tft.drawRoundRect(x_val, y_val, 19, 19, 4,colors[curCol]);
        curCol = (curCol+1)%7;
        if (x_val == tft.getWidth()-20){
            x_val = 0;
            if (y_val == tft.getHeight() - 20)
                y_val = 0;

            else
                y_val += 20;
            
        }
        else{
            x_val += 20;
        }
    }
}

void fillRoundSquares()
{
    int x_val = 0;
    int y_val = 0;
    uint8_t curCol = 0;

    for (int i = 0; i < 480*320/(20*20); i++){
        tft.fillRoundRect(x_val, y_val, 19, 19, 4,colors[curCol]);
        curCol = (curCol+1)%7;
        if (x_val == tft.getWidth()-20){
            x_val = 0;
            if (y_val == tft.getHeight() - 20)
                y_val = 0;

            else
                y_val += 20;
            
        }
        else{
            x_val += 20;
        }
    }
}

void fillCircles()
{
    int x_val = 10;
    int y_val = 10;
    uint8_t curCol = 0;
    for (int i = 10; i < 480*320/(20*20); i++){
        tft.fillCircle(x_val, y_val, 9, colors[curCol]);
        curCol = (curCol+1)%7;
        if (x_val == (tft.getWidth()-10)){
            x_val = 10;
            if (y_val == (tft.getHeight() - 10))
                y_val = 10;
            else
                y_val += 20;
        }
        else{
            x_val += 20;
        }
    }
}

void drawCircles()
{
    int x_val = 10;
    int y_val = 10;
    uint8_t curCol = 0;
    for (int i = 10; i < 480*320/(20*20); i++){
        tft.drawCircle(x_val, y_val, 9, colors[curCol]);
        curCol = (curCol+1)%7;
        if (x_val == (tft.getWidth()-10)){
            x_val = 10;
            if (y_val == (tft.getHeight() - 10))
                y_val = 10;
            else
                y_val += 20;
        }
        else{
            x_val += 20;
        }
    }
}

void drawTriangles()
{
    uint16_t x0 = 0,
    y0 = 0,
    x1 = 20,
    y1 = 0,
    x2 = 10,
    y2 = 20,
    curCol = 0;

    for (int i = 0; i < tft.getHeight(); i += 20){
        for (int j = 0; j < tft.getWidth(); j += 20){
            tft.drawTriangle(x0, y0, x1, y1, x2, y2, colors[curCol]);
            x0 += 20;
            x1 += 20;
            x2 += 20;
            curCol++;
            curCol %= 8;
        }
        x0 = 0;
        y0 += 20;
        x1 = x0 + 20;
        y1 = y0;
        x2 = x0 + 10;
        y2 = y1 + 20;
        curCol = 0;
    }

}

void fillTriangles()
{
    uint16_t x0 = 0,
    y0 = 0,
    x1 = 20,
    y1 = 0,
    x2 = 10,
    y2 = 20,
    curCol = 0;

    for (int i = 0; i < tft.getHeight(); i += 20){
        for (int j = 0; j < tft.getWidth(); j += 20){
            tft.fillTriangle(x0, y0, x1, y1, x2, y2, colors[curCol]);
            x0 += 20;
            x1 += 20;
            x2 += 20;
            curCol++;
            curCol %= 8;
        }
        x0 = 0;
        y0 += 20;
        x1 = x0 + 20;
        y1 = y0;
        x2 = x0 + 10;
        y2 = y1 + 20;
        curCol = 0;
    }

}

void drawChars()
{
    tft.setTextSize(2);

    for (int i = 0; i < 255; i++)
    {
        if (! (i%53))
            tft.writeChar('\n');
        tft.writeChar(i);
    }
}

void printText()
{
    tft.setTextSize(2);
    uint8_t text[256];
    for (int i = 0; i < 255; i++)
        text[i] = ((uint8_t)random()%80) + 33;
    
    text[255] = '\0';

    tft.print(text);
    tft.setCursor(0, 0);
}

void block()
{
    printf("Blocked\n");
    while(gpio_get(button_pin));
    sleep_ms(50);
    while(!gpio_get(button_pin));
}

