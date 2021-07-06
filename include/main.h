/*
 * A program to test the features of the interface
 */

#include "HX8357_interface.h"
#include "SSL.h"
 
#define start_pin 2  //The base pin where it and 7 others are to be used as the data pins 
                     //e.g. if it's pin 2 the data pins are 2 to 10. They have to be contiguous
#define RD_PIN 11   
#define RST_PIN 10
#define CS_PIN 12
#define button_pin 15
#define WR_PIN 27
#define DC_PIN 26  // AKA RS pin

void block();
void fillSquares();
void drawGrid();
void drawRays();
void drawSquares();
void fillCircles();
void drawCircles();
void drawRoundSquares();
void fillRoundSquares();
void drawTriangles();
void fillTriangles();
void drawChars();
void printText();
uint16_t colors[8] = {BLUE, RED, CYAN, GREEN, PINK, WHITE, YELLOW};
