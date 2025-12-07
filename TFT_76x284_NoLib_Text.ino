//Written debugged and cleaned with the help of DeepSeek - Thank you.

#include <SPI.h>
#include <avr/pgmspace.h>  // For storing font in program memory

// ========== DISPLAY CONFIGURATION ==========
#define TFT_CS        3
#define TFT_RST       4
#define TFT_DC        5

#define TFT_WIDTH     76
#define TFT_HEIGHT    284
#define TFT_X_OFFSET  82
#define TFT_Y_OFFSET  18

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

// ========== FONT DEFINITION (7x5 pixels) ==========
// Each character is 5 bytes wide (columns), 7 bits high (rows)
// Stored in PROGMEM to save RAM

const uint8_t font7x5[96][5] PROGMEM = {
  {0x00,0x00,0x00,0x00,0x00}, // Space (ASCII 32)
  {0x00,0x00,0x5F,0x00,0x00}, // !
  {0x00,0x07,0x00,0x07,0x00}, // "
  {0x14,0x7F,0x14,0x7F,0x14}, // #
  {0x24,0x2A,0x7F,0x2A,0x12}, // $
  {0x23,0x13,0x08,0x64,0x62}, // %
  {0x36,0x49,0x55,0x22,0x50}, // &
  {0x00,0x05,0x03,0x00,0x00}, // '
  {0x00,0x1C,0x22,0x41,0x00}, // (
  {0x00,0x41,0x22,0x1C,0x00}, // )
  {0x14,0x08,0x3E,0x08,0x14}, // *
  {0x08,0x08,0x3E,0x08,0x08}, // +
  {0x00,0x50,0x30,0x00,0x00}, // ,
  {0x08,0x08,0x08,0x08,0x08}, // -
  {0x00,0x60,0x60,0x00,0x00}, // .
  {0x20,0x10,0x08,0x04,0x02}, // /
  {0x3E,0x51,0x49,0x45,0x3E}, // 0
  {0x00,0x42,0x7F,0x40,0x00}, // 1
  {0x42,0x61,0x51,0x49,0x46}, // 2
  {0x21,0x41,0x45,0x4B,0x31}, // 3
  {0x18,0x14,0x12,0x7F,0x10}, // 4
  {0x27,0x45,0x45,0x45,0x39}, // 5
  {0x3C,0x4A,0x49,0x49,0x30}, // 6
  {0x01,0x71,0x09,0x05,0x03}, // 7
  {0x36,0x49,0x49,0x49,0x36}, // 8
  {0x06,0x49,0x49,0x29,0x1E}, // 9
  {0x00,0x36,0x36,0x00,0x00}, // :
  {0x00,0x56,0x36,0x00,0x00}, // ;
  {0x08,0x14,0x22,0x41,0x00}, // <
  {0x14,0x14,0x14,0x14,0x14}, // =
  {0x00,0x41,0x22,0x14,0x08}, // >
  {0x02,0x01,0x51,0x09,0x06}, // ?
  {0x32,0x49,0x79,0x41,0x3E}, // @
  {0x7E,0x11,0x11,0x11,0x7E}, // A
  {0x7F,0x49,0x49,0x49,0x36}, // B
  {0x3E,0x41,0x41,0x41,0x22}, // C
  {0x7F,0x41,0x41,0x22,0x1C}, // D
  {0x7F,0x49,0x49,0x49,0x41}, // E
  {0x7F,0x09,0x09,0x09,0x01}, // F
  {0x3E,0x41,0x49,0x49,0x7A}, // G
  {0x7F,0x08,0x08,0x08,0x7F}, // H
  {0x00,0x41,0x7F,0x41,0x00}, // I
  {0x20,0x40,0x41,0x3F,0x01}, // J
  {0x7F,0x08,0x14,0x22,0x41}, // K
  {0x7F,0x40,0x40,0x40,0x40}, // L
  {0x7F,0x02,0x0C,0x02,0x7F}, // M
  {0x7F,0x04,0x08,0x10,0x7F}, // N
  {0x3E,0x41,0x41,0x41,0x3E}, // O
  {0x7F,0x09,0x09,0x09,0x06}, // P
  {0x3E,0x41,0x51,0x21,0x5E}, // Q
  {0x7F,0x09,0x19,0x29,0x46}, // R
  {0x46,0x49,0x49,0x49,0x31}, // S
  {0x01,0x01,0x7F,0x01,0x01}, // T
  {0x3F,0x40,0x40,0x40,0x3F}, // U
  {0x1F,0x20,0x40,0x20,0x1F}, // V
  {0x3F,0x40,0x38,0x40,0x3F}, // W
  {0x63,0x14,0x08,0x14,0x63}, // X
  {0x07,0x08,0x70,0x08,0x07}, // Y
  {0x61,0x51,0x49,0x45,0x43}, // Z
  {0x00,0x7F,0x41,0x41,0x00}, // [
  {0x02,0x04,0x08,0x10,0x20}, // Backslash
  {0x00,0x41,0x41,0x7F,0x00}, // ]
  {0x04,0x02,0x01,0x02,0x04}, // ^
  {0x40,0x40,0x40,0x40,0x40}, // _
  {0x00,0x01,0x02,0x04,0x00}, // `
  {0x20,0x54,0x54,0x54,0x78}, // a
  {0x7F,0x48,0x44,0x44,0x38}, // b
  {0x38,0x44,0x44,0x44,0x20}, // c
  {0x38,0x44,0x44,0x48,0x7F}, // d
  {0x38,0x54,0x54,0x54,0x18}, // e
  {0x08,0x7E,0x09,0x01,0x02}, // f
  {0x0C,0x52,0x52,0x52,0x3E}, // g
  {0x7F,0x08,0x04,0x04,0x78}, // h
  {0x00,0x44,0x7D,0x40,0x00}, // i
  {0x20,0x40,0x44,0x3D,0x00}, // j
  {0x7F,0x10,0x28,0x44,0x00}, // k
  {0x00,0x41,0x7F,0x40,0x00}, // l
  {0x7C,0x04,0x18,0x04,0x78}, // m
  {0x7C,0x08,0x04,0x04,0x78}, // n
  {0x38,0x44,0x44,0x44,0x38}, // o
  {0x7C,0x14,0x14,0x14,0x08}, // p
  {0x08,0x14,0x14,0x18,0x7C}, // q
  {0x7C,0x08,0x04,0x04,0x08}, // r
  {0x48,0x54,0x54,0x54,0x20}, // s
  {0x04,0x3F,0x44,0x40,0x20}, // t
  {0x3C,0x40,0x40,0x20,0x7C}, // u
  {0x1C,0x20,0x40,0x20,0x1C}, // v
  {0x3C,0x40,0x30,0x40,0x3C}, // w
  {0x44,0x28,0x10,0x28,0x44}, // x
  {0x0C,0x50,0x50,0x50,0x3C}, // y
  {0x44,0x64,0x54,0x4C,0x44}, // z
  {0x00,0x08,0x36,0x41,0x00}, // {
  {0x00,0x00,0x7F,0x00,0x00}, // |
  {0x00,0x41,0x36,0x08,0x00}, // }
  {0x10,0x08,0x08,0x10,0x08}, // ~
};

// ========== TEXT STATE VARIABLES ==========
uint16_t cursorX = 0;
uint16_t cursorY = 0;
uint16_t textColor = WHITE;
uint16_t bgColor = BLACK;
uint8_t textSize = 1;  // 1 = normal, 2 = double height

// ========== BASIC DISPLAY FUNCTIONS ==========
void setup(void) {
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  
  // Reset display
  digitalWrite(TFT_RST, HIGH);
  delay(100);
  digitalWrite(TFT_RST, LOW);
  delay(10);
  digitalWrite(TFT_RST, HIGH);
  delay(100);
  
  // Default states
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TFT_DC, HIGH);
  
  // Initialize SPI
  SPI.begin();
  SPCR = (1 << SPE) | (1 << MSTR);
  SPSR = (1 << SPI2X);
  
  // Initialize display
  initializeDisplay();
  
  Serial.println("Display initialized!");
  
  // Run text demo
  textDemo();
}

void initializeDisplay() {
  writeCommand(0x01); delay(250);
  writeCommand(0x11); delay(25);
  writeCommand(0x3A); writeData(0x55); delay(25);
  writeCommand(0x36); writeData(0x00);
  writeCommand(0x20); delay(25);
  writeCommand(0x13); delay(25);
  writeCommand(0x29); delay(25);
  writeCommand(0x36); writeData(0xC0);
}

// ========== CURSOR AND TEXT FUNCTIONS ==========

// Set cursor position
void setCursor(uint16_t x, uint16_t y) {
  cursorX = x;
  cursorY = y;
}

// Set text color (background defaults to black)
void setTextColor(uint16_t color) {
  textColor = color;
}

// Set text color with background
void setTextColor(uint16_t color, uint16_t background) {
  textColor = color;
  bgColor = background;
}

// Set text size (1 = normal, 2 = double height)
void setTextSize(uint8_t size) {
  if (size >= 1 && size <= 2) {
    textSize = size;
  }
}

// Draw a single character
void drawChar(char c) {
  if (c < 32 || c > 127) return;  // Only printable ASCII
  
  // Get character data from PROGMEM
  uint8_t charData[5];
  for (int i = 0; i < 5; i++) {
    charData[i] = pgm_read_byte(&font7x5[c - 32][i]);
  }
  
  // Draw character pixel by pixel
  for (int col = 0; col < 5; col++) {
    uint8_t column = charData[col];
    
    for (int row = 0; row < 7; row++) {
      if (column & (1 << row)) {
        // Pixel should be text color
        if (textSize == 1) {
          drawPixel(cursorX + col, cursorY + row, textColor);
        } else {
          // Double height: draw 2x2 block for each pixel
          drawPixel(cursorX + col*2, cursorY + row*2, textColor);
          drawPixel(cursorX + col*2 + 1, cursorY + row*2, textColor);
          drawPixel(cursorX + col*2, cursorY + row*2 + 1, textColor);
          drawPixel(cursorX + col*2 + 1, cursorY + row*2 + 1, textColor);
        }
      } else if (bgColor != BLACK) {
        // Fill background if not black
        if (textSize == 1) {
          drawPixel(cursorX + col, cursorY + row, bgColor);
        } else {
          drawPixel(cursorX + col*2, cursorY + row*2, bgColor);
          drawPixel(cursorX + col*2 + 1, cursorY + row*2, bgColor);
          drawPixel(cursorX + col*2, cursorY + row*2 + 1, bgColor);
          drawPixel(cursorX + col*2 + 1, cursorY + row*2 + 1, bgColor);
        }
      }
    }
  }
  
  // Move cursor (add space between characters)
  if (textSize == 1) {
    cursorX += 6;  // 5px char + 1px space
  } else {
    cursorX += 12; // 10px char + 2px space
  }
  
  // Check for screen wrap
  if (textSize == 1 && cursorX > TFT_WIDTH - 6) {
    cursorX = 0;
    cursorY += 8;  // 7px char + 1px space
  } else if (textSize == 2 && cursorX > TFT_WIDTH - 12) {
    cursorX = 0;
    cursorY += 16; // 14px char + 2px space
  }
}

// Print a string
void print(const char* str) {
  while (*str) {
    drawChar(*str++);
  }
}

// Print a string from PROGMEM
void print_P(const char* str) {
  char c;
  while ((c = pgm_read_byte(str++))) {
    drawChar(c);
  }
}

// Print a number (integer)
void print(int num) {
  char buffer[12];
  itoa(num, buffer, 10);
  print(buffer);
}

// Print a number (long)
void print(long num) {
  char buffer[12];
  ltoa(num, buffer, 10);
  print(buffer);
}

// Print a number (float with specified decimals)
void print(float num, int decimals = 2) {
  char buffer[20];
  dtostrf(num, 0, decimals, buffer);
  print(buffer);
}

// Print hex number (8-bit)
void printHex(uint8_t num) {
  char buffer[3];
  sprintf(buffer, "%02X", num);
  print(buffer);
}

// Print hex number (16-bit)
void printHex(uint16_t num) {
  char buffer[5];
  sprintf(buffer, "%04X", num);
  print(buffer);
}

// Print hex number (32-bit)
void printHex(uint32_t num) {
  char buffer[9];
  sprintf(buffer, "%08lX", num);
  print(buffer);
}

// Print with newline
void println(const char* str) {
  print(str);
  cursorX = 0;
  cursorY += (textSize == 1) ? 8 : 16;
}

void println(int num) {
  print(num);
  cursorX = 0;
  cursorY += (textSize == 1) ? 8 : 16;
}

void println(float num, int decimals = 2) {
  print(num, decimals);
  cursorX = 0;
  cursorY += (textSize == 1) ? 8 : 16;
}

// ========== BASIC DISPLAY FUNCTIONS ==========

void writeCommand(uint8_t cmd) {
  PORTD &= ~0x20;
  PORTD &= ~0x08;
  SPI.transfer(cmd);
  PORTD |= 0x08;
}

void writeData(uint8_t data) {
  PORTD |= 0x20;
  PORTD &= ~0x08;
  SPI.transfer(data);
  PORTD |= 0x08;
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  x0 += TFT_X_OFFSET;
  y0 += TFT_Y_OFFSET;
  x1 += TFT_X_OFFSET;
  y1 += TFT_Y_OFFSET;
  
  writeCommand(0x2A);
  writeData(x0 >> 8);
  writeData(x0 & 0xFF);
  writeData(x1 >> 8);
  writeData(x1 & 0xFF);
  
  writeCommand(0x2B);
  writeData(y0 >> 8);
  writeData(y0 & 0xFF);
  writeData(y1 >> 8);
  writeData(y1 & 0xFF);
  
  writeCommand(0x2C);
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color) {
  if (x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
  
  setAddrWindow(x, y, x, y);
  
  PORTD |= 0x20;
  PORTD &= ~0x08;
  
  SPI.transfer(color >> 8);
  SPI.transfer(color & 0xFF);
  
  PORTD |= 0x08;
}

void fillScreen(uint16_t color) {
  setAddrWindow(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);
  
  PORTD |= 0x20;
  PORTD &= ~0x08;
  
  uint8_t hi = color >> 8;
  uint8_t lo = color & 0xFF;
  uint32_t total = (uint32_t)TFT_WIDTH * TFT_HEIGHT;
  
  for (uint32_t i = 0; i < total; i++) {
    SPI.transfer(hi);
    SPI.transfer(lo);
  }
  
  PORTD |= 0x08;
}

// ========== DEMO FUNCTION ==========

void textDemo() {
  // Clear screen
  fillScreen(BLACK);
  
  // Demo 1: Normal size text
  Serial.println("Demo 1: Normal text");
  setCursor(0, 0);
  setTextColor(WHITE);
  setTextSize(1);
  println("7x5 FontDemo");
  println("============");
  
  // Demo 2: Different colors
  setTextColor(RED);
  print("Red ");
  setTextColor(GREEN);
  println("Green");
  setTextColor(BLUE);
  println("Blue");
  
  // Demo 3: Numbers
  setTextColor(YELLOW);
  println("Integer:");
  print(12345);
  println("");  println("");
  println("Float: ");
  print(3.14159, 5);
  println("");  println("");
  // Demo 4: Hex numbers
  print("Hex 0xBODA: ");
  printHex(0xb0da);
  println("");
  
  // Demo 5: Double height text
  setTextSize(2);
  setTextColor(CYAN);
  setCursor(0,140);
  println("DOUBLEHEIGHT");
  
 /* // Demo 6: Text with background
  setTextSize(1);
  setTextColor(BLACK, YELLOW);  // Black text on yellow background
  setCursor(0, 120);
  println("BACKGROUND TEXT");
  
  // Demo 7: Scrolling text effect
  setTextColor(MAGENTA);
  setTextSize(1);
  for (int i = 0; i < 10; i++) {
    setCursor(0, 140 + i * 8);
    print("Line ");
    print(i);
    print(": Scroll test");
  }
  */ 
  // Demo 8: Character set
  setTextColor(WHITE);
  setTextSize(1);
  setCursor(0, 200);
  print("Chars: ");
  for (char c = 'A'; c <= 'Z'; c++) {
    drawChar(c);
  }
   
 
  setCursor(0, 230);
  for (char c = 'a'; c <= 'z'; c++) {
    drawChar(c);
  }
  
  setCursor(0, 260);
  for (char c = '0'; c <= '9'; c++) {
    drawChar(c);
  }

  
  Serial.println("Text demo complete!");
}

void loop() {
  // Nothing here - demo runs once
}