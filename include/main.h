#include "wizio.h"

#include "HX8357_interface.h"
#include "SSL.h"
 
#define start_pin 2
#define RD_PIN 11
#define RST_PIN 10
#define CS_PIN 12
#define button_pin 15
#define WR_PIN 27
#define DC_PIN 26

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
