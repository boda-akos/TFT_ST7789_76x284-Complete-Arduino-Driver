//Written and cleaned with the help of DeepSeek - Thank you.
//uses only 1300 Byte Flash, remove Serial.print() and the demo

#include <SPI.h>

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

void setup(void) {
  Serial.begin(9600);
  Serial.println("Starting TFT initialization...");
  
  // Initialize pins
  pinMode(TFT_RST, OUTPUT);
  pinMode(SYNC_PIN, OUTPUT);
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
  digitalWrite(SYNC_PIN, LOW);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TFT_DC, HIGH);
  
  // Initialize SPI
  SPI.begin();
  SPCR = (1 << SPE) | (1 << MSTR);  // Enable SPI, Master, Mode 0
  SPSR = (1 << SPI2X);              // Double speed
  
  // Initialize display
  initializeDisplay();
  
  Serial.println("Display initialized!");
  
  // Run demonstration
  runDemo();
}

void initializeDisplay() {
  // Working initialization sequence for YOUR display
  writeCommand(0x01); delay(250);  // Software reset
  writeCommand(0x11); delay(25);   // Sleep out
  writeCommand(0x3A); writeData(0x55); delay(25);  // 16-bit color mode
  writeCommand(0x36); writeData(0x00);             // MADCTL (normal orientation)
  writeCommand(0x20); delay(25);   // INVOFF (NOT INVON!) - CRITICAL FOR COLORS
  writeCommand(0x13); delay(25);   // Normal display on
  writeCommand(0x29); delay(25);   // Display on
  writeCommand(0x36); writeData(0xC0);             // MADCTL with rotation
}

void runDemo() {
  // Clear to black
  fillScreen(BLACK);
  delay(500);
  
  // Draw colored squares
  Serial.println("Drawing colored squares...");
  fillRect(10, 10, 20, 20, RED);
  fillRect(40, 10, 20, 20, GREEN);
  fillRect(10, 40, 20, 20, BLUE);
  fillRect(40, 40, 20, 20, YELLOW);
  delay(2000);
  
  // Clear and draw lines
  Serial.println("Drawing lines...");
  fillScreen(BLACK);
  drawLine(0, 0, TFT_WIDTH-1, TFT_HEIGHT-1, WHITE);
  drawLine(TFT_WIDTH-1, 0, 0, TFT_HEIGHT-1, MAGENTA);
  delay(2000);
  
  // Clear and draw circles
  Serial.println("Drawing circles...");
  fillScreen(BLACK);
  drawCircle(TFT_WIDTH/2, TFT_HEIGHT/2, 30, CYAN);
  fillCircle(TFT_WIDTH/2, TFT_HEIGHT/2, 20, BLUE);
  delay(2000);
  
  Serial.println("Demo complete!");
}

// ========== BASIC DISPLAY FUNCTIONS ==========

void writeCommand(uint8_t cmd) {
  PORTD &= ~0x20;  // TFT_DC LOW (command)
  PORTD &= ~0x08;  // TFT_CS LOW
  
  SPI.transfer(cmd);
  
  PORTD |= 0x08;   // TFT_CS HIGH
}

void writeData(uint8_t data) {
  PORTD |= 0x20;   // TFT_DC HIGH (data)
  PORTD &= ~0x08;  // TFT_CS LOW
  
  SPI.transfer(data);
  
  PORTD |= 0x08;   // TFT_CS HIGH
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  // Add offsets for your specific display
  x0 += TFT_X_OFFSET;
  y0 += TFT_Y_OFFSET;
  x1 += TFT_X_OFFSET;
  y1 += TFT_Y_OFFSET;
  
  writeCommand(0x2A);  // CASET
  writeData(x0 >> 8);
  writeData(x0 & 0xFF);
  writeData(x1 >> 8);
  writeData(x1 & 0xFF);
  
  writeCommand(0x2B);  // RASET
  writeData(y0 >> 8);
  writeData(y0 & 0xFF);
  writeData(y1 >> 8);
  writeData(y1 & 0xFF);
  
  writeCommand(0x2C);  // RAMWR
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color) {
  if (x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
  
  setAddrWindow(x, y, x, y);
  
  PORTD |= 0x20;   // TFT_DC HIGH (data)
  PORTD &= ~0x08;  // TFT_CS LOW
  
  SPI.transfer(color >> 8);
  SPI.transfer(color & 0xFF);
  
  PORTD |= 0x08;   // TFT_CS HIGH
}

void fillScreen(uint16_t color) {
  setAddrWindow(0, 0, TFT_WIDTH-1, TFT_HEIGHT-1);
  
  PORTD |= 0x20;   // TFT_DC HIGH (data)
  PORTD &= ~0x08;  // TFT_CS LOW
  
  uint8_t hi = color >> 8;
  uint8_t lo = color & 0xFF;
  uint32_t total = (uint32_t)TFT_WIDTH * TFT_HEIGHT;
  
  for (uint32_t i = 0; i < total; i++) {
    SPI.transfer(hi);
    SPI.transfer(lo);
  }
  
  PORTD |= 0x08;   // TFT_CS HIGH
}

void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
  // Clip to screen
  if (x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
  if (x + w > TFT_WIDTH) w = TFT_WIDTH - x;
  if (y + h > TFT_HEIGHT) h = TFT_HEIGHT - y;
  if (w == 0 || h == 0) return;
  
  setAddrWindow(x, y, x + w - 1, y + h - 1);
  
  PORTD |= 0x20;   // TFT_DC HIGH (data)
  PORTD &= ~0x08;  // TFT_CS LOW
  
  uint8_t hi = color >> 8;
  uint8_t lo = color & 0xFF;
  uint32_t total = (uint32_t)w * h;
  
  for (uint32_t i = 0; i < total; i++) {
    SPI.transfer(hi);
    SPI.transfer(lo);
  }
  
  PORTD |= 0x08;   // TFT_CS HIGH
}

void drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color) {
  fillRect(x, y, w, 1, color);
}

void drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color) {
  fillRect(x, y, 1, h, color);
}

void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x + w - 1, y, h, color);
}

// ========== LINE DRAWING ==========
#define swap(a, b) { int16_t t = a; a = b; b = t; }

void drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
  int dx, dy, err, ystep;
  int steep = abs(y1 - y0) > abs(x1 - x0);
  
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }
  
  dx = x1 - x0;
  dy = abs(y1 - y0);
  err = dx / 2;
  
  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// ========== CIRCLE DRAWING ==========
void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
  int x = r;
  int y = 0;
  int err = 0;

  while (x >= y) {
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 - y, y0 - x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 + x, y0 - y, color);
    
    y++;
    err += 1 + 2*y;
    if (2*(err - x) + 1 > 0) {
      x--;
      err += 1 - 2*x;
    }
  }
}

void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
  for (int y = -r; y <= r; y++) {
    for (int x = -r; x <= r; x++) {
      if (x*x + y*y <= r*r) {
        if (x0 + x >= 0 && x0 + x < TFT_WIDTH && 
            y0 + y >= 0 && y0 + y < TFT_HEIGHT) {
          drawPixel(x0 + x, y0 + y, color);
        }
      }
    }
  }
}

void loop() {
  // Empty - all demos run once in setup
}
