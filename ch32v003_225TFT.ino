// CH32V006/CH32V003 ST7789 76x284 Display Driver

#define TFT_RST  PD4
#define TFT_DC   PD3
#define TFT_CS   PD2
//SPI MOSI/SDA = PC6, SCL = PC5 default pins
// CH32V006 ST7789 76x284 Display Driver

// Color definitions (RGB565)
#define BL 0x001F  // Red
#define GN 0x07E0  // Green  
#define RD 0xF800  // Blue
#define WH 0xFFFF  // White
#define BK 0x0000  // Black
#define CY 0xFFE0  // Cyan
#define YL 0xFE20  // Yellow
#define MG 0xF81F  // Magenta
#define SL 0x7BEF  // Gray

// Display configuration
#define DISPLAY_WIDTH  76
#define DISPLAY_HEIGHT 284
#define TFT_X_OFFSET   82
#define TFT_Y_OFFSET   18
#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x08

#include <SPI.h>

// Global variables
int textCol = WH, textBgr = BK, tftIdx = 0, tftIdy = 0, xStart, yStart;
byte textSize = 1, rotate = 0;

// ========== FONT TABLE ==========
const uint8_t Chartable[] = 
{ 0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x2F,0x00,0x00, 0x00,0x07,0x00,0x07,0x00, 0x14,0x7F,0x14,0x7F,0x14,
  0x24,0x2A,0x7F,0x2A,0x12, 0x23,0x13,0x08,0x64,0x62, 0x36,0x49,0x55,0x22,0x50, 0x00,0x05,0x03,0x00,0x00,
  0x00,0x1C,0x22,0x41,0x00, 0x00,0x41,0x22,0x1C,0x00, 0x14,0x08,0x3E,0x08,0x14, 0x08,0x08,0x3E,0x08,0x08,
  0x00,0x00,0x50,0x30,0x00, 0x10,0x10,0x10,0x10,0x10, 0x00,0x60,0x60,0x00,0x00, 0x20,0x10,0x08,0x04,0x02,
  0x3E,0x51,0x49,0x45,0x3E, 0x00,0x42,0x7F,0x40,0x00, 0x42,0x61,0x51,0x49,0x46, 0x21,0x41,0x45,0x4B,0x31,
  0x18,0x14,0x12,0x7F,0x10, 0x27,0x45,0x45,0x45,0x39, 0x3C,0x4A,0x49,0x49,0x30, 0x01,0x71,0x09,0x05,0x03,
  0x36,0x49,0x49,0x49,0x36, 0x06,0x49,0x49,0x29,0x1E, 0x00,0x36,0x36,0x00,0x00, 0x00,0x56,0x36,0x00,0x00,
  0x08,0x14,0x22,0x41,0x00, 0x14,0x14,0x14,0x14,0x14, 0x00,0x41,0x22,0x14,0x08, 0x02,0x01,0x51,0x09,0x06,
  0x32,0x49,0x59,0x51,0x3E, 0x7E,0x11,0x11,0x11,0x7E, 0x7F,0x49,0x49,0x49,0x36, 0x3E,0x41,0x41,0x41,0x22,
  0x7F,0x41,0x41,0x22,0x1C, 0x7F,0x49,0x49,0x49,0x41, 0x7F,0x09,0x09,0x09,0x01, 0x3E,0x41,0x49,0x49,0x7A,
  0x7F,0x08,0x08,0x08,0x7F, 0x00,0x41,0x7F,0x41,0x00, 0x20,0x40,0x41,0x3F,0x01, 0x7F,0x08,0x14,0x22,0x41,
  0x7F,0x40,0x40,0x40,0x40, 0x7F,0x02,0x0C,0x02,0x7F, 0x7F,0x04,0x08,0x10,0x7F, 0x3E,0x41,0x41,0x41,0x3E,
  0x7F,0x09,0x09,0x09,0x06, 0x3E,0x41,0x51,0x21,0x5E, 0x7F,0x09,0x19,0x29,0x46, 0x46,0x49,0x49,0x49,0x31,
  0x01,0x01,0x7F,0x01,0x01, 0x3F,0x40,0x40,0x40,0x3F, 0x1F,0x20,0x40,0x20,0x1F, 0x3F,0x40,0x38,0x40,0x3F,
  0x63,0x14,0x08,0x14,0x63, 0x07,0x08,0x70,0x08,0x07, 0x61,0x51,0x49,0x45,0x43, 0x00,0x7F,0x41,0x41,0x00,
  0x02,0x04,0x08,0x10,0x20, 0x00,0x41,0x41,0x7F,0x00, 0x11,0x39,0x55,0x11,0x1F, 0x40,0x40,0x40,0x40,0x40,
  0x10,0x38,0x54,0x10,0x1F, 0x20,0x54,0x54,0x54,0x78, 0x7F,0x48,0x44,0x44,0x38, 0x38,0x44,0x44,0x44,0x20,
  0x38,0x44,0x44,0x48,0x7F, 0x38,0x54,0x54,0x54,0x18, 0x08,0x7E,0x09,0x01,0x02, 0x0C,0x52,0x52,0x52,0x3E,
  0x7F,0x08,0x04,0x04,0x78, 0x00,0x44,0x7D,0x40,0x00, 0x20,0x40,0x44,0x3D,0x00, 0x7F,0x10,0x28,0x44,0x00,
  0x00,0x41,0x7F,0x40,0x00, 0x7C,0x04,0x18,0x04,0x78, 0x7C,0x08,0x04,0x04,0x78, 0x38,0x44,0x44,0x44,0x38,
  0x7C,0x14,0x14,0x14,0x08, 0x08,0x14,0x14,0x18,0x7C, 0x7C,0x08,0x04,0x04,0x08, 0x48,0x54,0x54,0x54,0x20,
  0x04,0x3F,0x44,0x40,0x20, 0x3C,0x40,0x40,0x20,0x7C, 0x1C,0x20,0x40,0x20,0x1C, 0x3C,0x40,0x30,0x40,0x3C,
  0x44,0x28,0x10,0x28,0x44, 0x0C,0x50,0x50,0x50,0x3C, 0x44,0x64,0x54,0x4C,0x44, 0x11,0x39,0x55,0x11,0x1F,
  0x10,0x38,0x54,0x10,0x1F, 0x20,0x54,0x57,0x54,0x78, 0x38,0x54,0x57,0x54,0x18, 0x00,0x48,0x7B,0x40,0x00,
  0x38,0x44,0x47,0x44,0x38, 0x38,0x47,0x44,0x47,0x38, 0x38,0x45,0x44,0x45,0x38, 0x3C,0x40,0x47,0x20,0x7C,
  0x3C,0x47,0x40,0x27,0x7C, 0x3C,0x41,0x40,0x21,0x7C };

/*Basic Colors (BGR 5-6-5 format)
RED: 0x001F GREEN: 0x07E0 BLUE: 0xF800 YELLOW: 0x07FF (RED + GREEN) MAGENTA: 0xF81F (RED + BLUE) CYAN: 0xFFE0 (GREEN + BLUE)
WHITE: 0xFFFF (ALL ON) BLACK: 0x0000 (ALL OFF) ORANGE: 0x04FF PINK: 0xFC1F PURPLE: 0x8010 LIME: 0x05E0 TEAL: 0x87E0 NAVY: 0x8000
MAROON: 0x0010 DARK GRAY: 0x4208 GRAY: 0x8410 LIGHT GRAY: 0xC618 SILVER: 0xA514 BROWN: 0x0210 OLIVE: 0x0410 FOREST GREEN: 0x0280
GOLD: 0x051F Lighter: 0x063F Darker: 0x0410*/
// ========== SETUP ==========
void setup() { 
    /*// Clock setup if no 24MHz crystal
    RCC->CTLR |= RCC_PLLON;
    while(!(RCC->CTLR & RCC_PLLRDY));
    RCC->CFGR0 |= 0;
    RCC->CFGR0 &= ~RCC_SW;
    RCC->CFGR0 |= RCC_SW_PLL;
    while((RCC->CFGR0 & RCC_SWS) != RCC_SWS_PLL);
   */
    // Pin initialization
    pinMode(PD2, OUTPUT);  // TFT_CS
    pinMode(PD3, OUTPUT);  // TFT_DC
    pinMode(PD4, OUTPUT);  // TFT_RST
    
    //Serial.begin(115200);
    
    SPI.begin(); 
    SPI.setClockDivider(1);  //SPI clock 12Mhz
 
    InitTFT();
    setRotation(1);
    delay(500); 
}

// ========== MAIN LOOP ==========
void loop() { 
static byte x;  
runDisplayTest();  setRotation(x++);
}

// ========== DISPLAY FUNCTIONS ==========
void writeCommand(uint8_t cmd) {
    GPIOD->OUTDR &= ~(1<<3);  // DC LOW (command)
    GPIOD->OUTDR &= ~(1<<2);  // CS LOW (PD2)
    SPI1_Transfer(cmd);
    GPIOD->OUTDR |= (1<<2);   // CS HIGH (PD2)
}

void writeData(uint8_t data) {
    GPIOD->OUTDR |= (1<<3);   // DC HIGH (data)
    GPIOD->OUTDR &= ~(1<<2);  // CS LOW (PD2)
    SPI1_Transfer(data);
    GPIOD->OUTDR |= (1<<2);   // CS HIGH (PD2)
}

void SPI1_Transfer(uint8_t data) {
    
 // CH32V003: SPI1 is at 0x40013000
    // Wait for TX empty
    while (!(SPI1->STATR & (1 << 1)));  // SPI_STATR_TXE
    
    SPI1->DATAR = data;
    
    // Wait for RX not empty
    while (!(SPI1->STATR & (1 << 0)));  // SPI_STATR_RXNE
    
    // Read to clear RXNE flag (IMPORTANT!)
    volatile uint8_t dummy __attribute__((unused)) = SPI1->DATAR;

}

// ========== SET ADDRESS WINDOW WITH OFFSETS ==========
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // Add display offsets
    x += xStart;  
    y += yStart;
    uint32_t x1 = ((uint32_t)x<<16) | (x+w);
    uint32_t y1 = ((uint32_t)y<<16) | (y+h);

    writeCommand(0x2A); // Column address set
    writeData(x1 >> 24);
    writeData(x1 >> 16);
    writeData(x1 >> 8);
    writeData(x1 & 0xFF);
    
    writeCommand(0x2B); // Row address set
    writeData(y1 >> 24);
    writeData(y1 >> 16);
    writeData(y1 >> 8);
    writeData(y1 & 0xFF);
    
    writeCommand(0x2C); // Memory write
}

// ========== SET ROTATION ==========
void setRotation(uint8_t rotation) {
    rotate = rotation % 4;
    uint8_t madctl;
   //TFT_X_OFFSET=82  TFT_Y_OFFSET=18
    switch (rotate) {
        case 0:  // Portrait
            madctl = 0xC0;  // MY=1, MX=1, MV=0
            xStart=TFT_X_OFFSET; yStart=TFT_Y_OFFSET;
            break;
        case 1:  // Landscape (90°)
            madctl = 0xb0;  // MY=1, MX=0, MV=1
            xStart=TFT_Y_OFFSET; yStart=TFT_X_OFFSET;
            break;
        case 2:  // Portrait (180°)
            madctl = 0x00;  // MY=0, MX=0, MV=0
            xStart=TFT_X_OFFSET; yStart=TFT_Y_OFFSET;
            break;
        case 3:  // Landscape (270°)
            madctl = 0x60;  // MY=0, MX=1, MV=1
            xStart=TFT_Y_OFFSET; yStart=TFT_X_OFFSET;
            break;}
    writeCommand(0x36);   // MADCTL command
    writeData(madctl);
    delay(10);
    }


// ========== BASIC DRAWING FUNCTIONS ==========
void tftPixel(uint16_t x, uint16_t y, uint16_t color) {
    setAddrWindow(x, y, x, y);
    GPIOD->OUTDR |= (1<<3);    // DC HIGH (data)
    GPIOD->OUTDR &= ~(1<<2);   // CS LOW (PD2)
    SPI1_Transfer(color >> 8);      // High byte
    SPI1_Transfer(color & 0xFF);    // Low byte
    GPIOD->OUTDR |= (1<<2);    // CS HIGH (PD2)
}

void fillScreen(uint16_t color) {
    uint8_t hi = color >> 8, lo = color & 0xFF;
    
    // Set address window for entire display
    if (rotate == 0 || rotate == 2) {
        // Portrait: 76x284
        setAddrWindow(0, 0, 75, 283);
    } else {
        // Landscape: 284x76  
        setAddrWindow(0, 0, 283, 75);
    }
    
    GPIOD->OUTDR |= (1<<3);    // DC HIGH (data)
    GPIOD->OUTDR &= ~(1<<2);   // CS LOW (PD2)
    
    // Fill all pixels
    for(uint32_t i = 0; i < 76 * 284; i++) {
        SPI1_Transfer(hi);
        SPI1_Transfer(lo);  
    }
     GPIOD->OUTDR |= (1<<2);    // CS HIGH (PD2)
}

// ========== TEXT FUNCTIONS ==========
void setTextSize(byte x) {
    if (x < 1) x = 1;
    if (x > 4) x = 4;
    textSize = x;
}

void setTextCol(int c, int b) {
    textCol = c;
    textBgr = b;
}

void tftCursor(byte tftx, byte tfty) {
    tftIdx = tftx;
    tftIdy = tfty;
}

void tftChar(uint8_t x, uint8_t y, char c) {
    if (c < 32 || c > 127) return;
    
    const uint8_t *charData = &Chartable[(c - 32) * 5];
    
    for (uint8_t col = 0; col < 5; col++) {
        uint8_t pixels = charData[col];
        for (uint8_t row = 0; row < 7; row++) {
            bool pixelOn = pixels & (1 << row);
            
            if (pixelOn || textBgr != -1) {
                for (uint8_t sx = 0; sx < textSize; sx++) {
                    for (uint8_t sy = 0; sy < textSize; sy++) {
                        uint16_t px = x + (col * textSize) + sx;
                        uint16_t py = y + (row * textSize) + sy;
                        uint16_t pixelColor = pixelOn ? textCol : (uint16_t)textBgr; 
                        tftPixel(px, py, pixelColor);
                    }
                }
            }
        }
    }
}

void tftPrint(char *p) {
    while (*p) {
        tftChar(tftIdx, tftIdy, *p++);
        tftIdx += 6 * textSize;
        
        // Newline check
        if (rotate == 0 || rotate == 2) {
            // Portrait width
            if (tftIdx >= 76 - textSize * 6) { 
                tftIdx = 0; 
                tftIdy += 8 * textSize; 
            }
        } else {
            // Landscape width
            if (tftIdx >= 284 - textSize * 6) { 
                tftIdx = 0; 
                tftIdy += 8 * textSize; 
            }
        }
    }
}

void tftPrint(float n) {
    char buffer[32];
    dtostrf(n, 6, 1, buffer);
    char *p = buffer;
    while (*p == ' ') p++;  // Skip spaces
    tftPrint(p);
}

// ========== GRAPHICS FUNCTIONS ==========
#define swap(a, b) { int16_t t = a; a = b; b = t; }

void drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx, dy, err, ystep;
    int steep = abs(y1 - y0) > abs(x1 - x0);
    
    if (steep) { swap(x0, y0); swap(x1, y1); }
    if (x0 > x1) { swap(x0, x1); swap(y0, y1); }
    
    dx = x1 - x0;
    dy = abs(y1 - y0);
    err = dx / 2;
    
    if (y0 < y1) { ystep = 1; } 
    else { ystep = -1; }
    
    for (; x0 <= x1; x0++) {
        if (steep) {
            tftPixel(y0, x0, color);
        } else {
            tftPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) { y0 += ystep; err += dx; }
    }
}

void hLine(int x, int y, int w, uint16_t color) {
    drawLine(x, y, x + w-1, y, color);
}

void vLine(int x, int y, int h, uint16_t color) {
    drawLine(x, y, x, y + h-1, color);
}

void drawRect(int x, int y, int w, int h, uint16_t color) {
    hLine(x, y, w, color);
    hLine(x, y + h - 1, w, color);
    vLine(x, y, h, color);
    vLine(x + w - 1, y, h, color);
}

void fillRect(int x, int y, int w, int h, uint16_t color) {
    for (int i = x; i < x + w; i++) {
        vLine(i, y, h, color);
    }
}

void drawCircle(int x0, int y0, int r, uint16_t color) {
    int f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    
    tftPixel(x0, y0 + r, color);
    tftPixel(x0, y0 - r, color);
    tftPixel(x0 + r, y0, color);
    tftPixel(x0 - r, y0, color);
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        tftPixel(x0 + x, y0 + y, color);
        tftPixel(x0 - x, y0 + y, color);
        tftPixel(x0 + x, y0 - y, color);
        tftPixel(x0 - x, y0 - y, color);
        tftPixel(x0 + y, y0 + x, color);
        tftPixel(x0 - y, y0 + x, color);
        tftPixel(x0 + y, y0 - x, color);
        tftPixel(x0 - y, y0 - x, color);
    }
}

void fillCircle(int x0, int y0, int r, uint16_t color) {
    vLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

// ========== HELPER FUNCTIONS ==========
void fillCircleHelper(int x0, int y0, int r, char cornername, int delta, uint16_t color) {
    int f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        if (cornername & 0x1) {
            vLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            vLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2) {
            vLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            vLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

// ========== INITIALIZE DISPLAY ==========
void InitTFT(void) {
    // Initialize pins
    GPIOD->OUTDR |= (1<<2);    // CS HIGH
    GPIOD->OUTDR |= (1<<4);    // RST HIGH
    
    // Hardware reset
    GPIOD->OUTDR &= ~(1<<4);   // RST LOW
    delay(20);
    GPIOD->OUTDR |= (1<<4);    // RST HIGH
    delay(150);
    
    // ST7789 Initialization Sequence
    writeCommand(0x01);   // Software reset
    delay(150);
    
    writeCommand(0x11);   // Sleep out
    delay(255);
    
    writeCommand(0x36);   // Memory Data Access Control
    writeData(0xC0);      // MX=1, MY=1, RGB mode
    
    writeCommand(0x3A);   // Interface Pixel Format
    writeData(0x55);      // 16-bit color (RGB565)
    delay(10);
    
    // Set display area (76x284)
    writeCommand(0x2A);   // Column Address Set
    writeData(0x00);
    writeData(0x00);      // Start column = 0
    writeData(0x00);
    writeData(0xef);      // End column = 75,239
    
    writeCommand(0x2B);   // Row Address Set
    writeData(0x00);
    writeData(0x00);      // Start row = 0
    writeData(0x01);
    writeData(0x3f);      // End row = 283 ,319
    
    writeCommand(0x20);   // Display Inversion off
    delay(10);
    
    writeCommand(0x29);   // Display On
    delay(100);
    // Clear screen
    fillScreen(BK);
}


void drawTestPattern(uint8_t rot) {
    // Draw different patterns based on rotation
    if (rot == 0 || rot == 2) {
        // Portrait patterns
        drawRect(0, 0,76,284, WH);
        drawRect(5, 5, 66, 50, GN);
        fillRect(10, 10, 25, 20, RD);
        fillCircle(50, 30, 15, BL);
        drawLine(0, 0, 75, 283, YL);
    } else {
        // Landscape patterns
        drawRect(0, 0,284,76, WH);
        drawRect(10, 10, 264, 56, GN);
        fillRect(20, 20, 100, 20, RD);
        fillCircle(200, 38, 15, BL);
        drawLine(0, 0, 283, 75, YL);
    }
}

// ========== DISPLAY TEST ==========
void runDisplayTest() {
       
    // Test solid colors
    fillScreen(RD); delay(500);
    fillScreen(GN); delay(500);
    fillScreen(BL); delay(500);
    fillScreen(BK);
    
    // Draw border
   //fillRect(0,0,75, 285,  CY);
   
    fillCircle(38, 230, 15, CY);
    // Draw center cross
    drawLine(38, 140, 38, 144, YL);
    drawLine(36, 142, 40, 142, YL);
    delay(1000);
    setTextCol(WH, BK);
    if (rotate ==2 || rotate ==0) {
    setTextSize(1);  
    tftCursor(10, 10);
    tftPrint("CH32V006");
    tftCursor(10, 25);
    tftPrint("76x284 TFT");
    tftCursor(10, 40);
    tftPrint("Test PASS");

    }
    else {setTextSize(2); tftCursor(10, 10);
    tftPrint("CH32V006 CH32V003");
    tftCursor(10, 30);
    tftPrint("76x284 ST7789 TFT");
    tftCursor(10, 50);
    tftPrint("Test PASS PASS PASS");
   }
   delay(2000);    
    fillScreen(BK);
    drawTestPattern(rotate);delay(2000);
}
