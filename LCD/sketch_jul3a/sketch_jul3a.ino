#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <math.h>

#define TFT_CS -1
#define TFT_RST 8
#define TFT_DC 9
#define BLACK 0x0000 // 黑色 
#define NAVY 0x000F // 深蓝色 
#define DGREEN 0x03E0 // 深绿色 
#define DCYAN 0x03EF // 深青色 
#define MAROON 0x7800 // 深红色 
#define PURPLE 0x780F // 紫色 
#define OLIVE 0x7BE0 // 橄榄绿 
#define LGRAY 0xC618 // 灰白色
#define DGRAY 0x7BEF // 深灰色 
#define BLUE 0x001F // 蓝色 
#define GREEN 0x07E0 // 绿色 
#define CYAN 0x07FF // 青色 
#define RED 0xF800 // 红色 
#define MAGENTA 0xF81F // 品红 
#define YELLOW 0xFFE0 // 黄色 
#define WHITE 0xFFFF // 白色 

Adafruit_ILI9341 tft = Adafruit_ILI9341(&SPI, TFT_DC, TFT_CS, TFT_RST);

void drawEllipse(float x0, float y0, float radiusX, float radiusY, float color) {
  for (float i = 0; i < 360; i += 0.1) {
    //i是角度 radiusX是长轴 radiusY是短轴
    float angle = i * 0.0174532925; // 将角度转换为弧度
    //极坐标形式
    float x = x0 + (cos(angle) * radiusX);
    float y = y0 + (sin(angle) * radiusY);
    //画像素点
    tft.drawPixel(x, y, color);
  }
}


void fillEllipse(float x0, float y0, float radiusX, float radiusY, float color) {
  for (float i = 0; i < 360; i += 0.1) {
    //i是角度 radiusX是长轴 radiusY是短轴
    float angle = i * 0.0174532925; // 将角度转换为弧度
    //极坐标形式
    float x = x0 + (cos(angle) * radiusX);
    float y = y0 + (sin(angle) * radiusY);
    //画像素线
    tft.drawLine(x0, y0, x, y, color);
  }
}

void drawPartCircle(float x0, float y0, float radius, float start_angle, float end_angle, float color) {
  for (float i = 0; i < start_angle-end_angle; i += 0.1) {
    //i是角度 radiusX是长轴 radiusY是短轴
    float angle = start_angle + i * 0.0174532925; // 将角度转换为弧度
    //极坐标形式
    float x = x0 + (cos(angle) * radius);
    float y = y0 + (sin(angle) * radius);
    //画像素点
    tft.drawPixel(x, y, color);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  tft.begin(40000000);
  tft.setRotation(3);
}

void loop() {
  int16_t x = 160;  // 中心点横坐标
  int16_t y = 120; // 中心点纵坐标
  float radiusX = 40;
  float radiusY = 30;
  //
  tft.fillCircle(x, y, 100, BLUE);
  tft.fillCircle(x, y-20, 70, WHITE);
  tft.fillCircle(x, y, 10, RED);
  //
  fillEllipse(x-30, y+40, 20, 30, WHITE);
  drawEllipse(x-30, y+40, 20, 30, BLACK);
  fillEllipse(x+30, y+40, 20, 30, WHITE);
  drawEllipse(x+30, y+40, 20, 30, BLACK);
  //眼睛
  tft.fillCircle(x-20, y+40, 10, BLUE);
  tft.fillCircle(x+20, y+40, 10, BLUE);
  //嘴
  tft.drawLine(x, y-10, x, y-40, BLACK);
  tft.drawLine(x-10, y-10, x-50, y+10, BLACK);
  tft.drawLine(x+10, y-10, x+50, y+10, BLACK);
  tft.drawLine(x-10, y-30, x-50, y-30, BLACK);
  tft.drawLine(x+10, y-30, x+50, y-30, BLACK);
  tft.drawLine(x-10, y-50, x-50, y-70, BLACK);
  tft.drawLine(x+10, y-50, x+50, y-70, BLACK);
  tft.drawLine(x-10, y-60, x+10, y-60, BLACK);
  delay(1000000);
}
