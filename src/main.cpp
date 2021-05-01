// Demo using arcFill to draw ellipses and a segmented elipse
#include <M5Core2.h>
#include <utility/Sprite.h>

TFT_eSprite sprite = TFT_eSprite(&M5.Lcd);
TFT_eSprite img = TFT_eSprite(&M5.Lcd);
TFT_eSprite moji = TFT_eSprite(&M5.Lcd);
TFT_eSprite png = TFT_eSprite(&M5.Lcd);

int x = 120;
int y = 120;
int r = 120;

int mode = 0;

void drawMemori(int degree, uint32_t color, TFT_eSprite sprite)
{
  float pi = 3.14;
  float rad = degree * pi / 180;

  sprite.drawLine(x, y, (int)(x + r * cos(rad)), (int)(y + r * sin(rad)), color);
}

void drawMemori(int degree)
{
  drawMemori(degree, TFT_BLACK, img);
}

// void drawMemori2(float rad)
// {
//   img.drawLine(x, y, (int)(x + r * cos(rad)), (int)(y + r * sin(rad)), TFT_BLACK);
// }

void setup(void)
{
  M5.begin();
  M5.IMU.Init();

  M5.Lcd.fillScreen(TFT_BLACK);

  sprite.setColorDepth(8);
  sprite.createSprite(240, 240);

  img.setColorDepth(8);
  img.createSprite(240, 240);
  img.fillScreen(TFT_BLACK);
  img.fillCircle(x, y, 120, TFT_WHITE);

  for (int i = 0; i < 60; i++)
  {
    drawMemori(i * 6);
  }
  img.fillCircle(x, y, 110, TFT_WHITE);

  for (int i = 0; i < 12; i++)
  {
    drawMemori(i * 30);
  }
  img.fillCircle(x, y, 100, TFT_WHITE);

  img.drawLine(x, y, x, y - 100, TFT_BLACK);

  sprite.pushSprite(0, 0);

  png.createSprite(96, 96);
  png.fillScreen(WHITE);
  png.drawJpgFile(SD, "/notifications_active_black_48dp.jpg", 0, 0, 96, 96, 0, 0);
}

int i_deg = 0;

void update_i_deg()
{
  float ax, ay, az;
  M5.IMU.getAccelData(&ax, &ay, &az);

  ax = (1 < ax) ? 1 : ax;
  ax = (ax < -1) ? -1 : ax;
  ay = (1 < ay) ? 1 : ay;
  ay = (ay < -1) ? -1 : ay;

  float f_deg = atan2(ax, ay) / 3.14 * 180;
  i_deg = (int)f_deg;
  i_deg = (i_deg / 6) * 6;
}

int count = 0;

void loop_mode0()
{
  update_i_deg();

  img.pushRotated(&sprite, i_deg);

  moji.createSprite(100, 100);
  moji.fillScreen(TFT_WHITE);

  String sss = String(
      (i_deg <= 0 ? i_deg * -1 : 360 - i_deg) / 6);
  moji.drawCentreString(sss, 50, 20, FONT4);

  moji.pushRotated(&sprite, i_deg);

  sprite.drawLine(120, 120, 120, 120 - 100, TFT_RED);

  sprite.pushSprite(40, 0);

  count = (i_deg <= 0 ? i_deg * -1 : 360 - i_deg) / 6;
}

void loop_mode1()
{
  img.pushRotated(&sprite, i_deg);

  moji.createSprite(100, 100);
  moji.fillScreen(TFT_WHITE);

  String sss = String(count);
  moji.drawCentreString(sss, 50, 20, FONT4);

  moji.pushRotated(&sprite, i_deg);

  drawMemori((count - (i_deg <= 0 ? i_deg * -1 : 360 - i_deg) / 6) * 6 - 90, TFT_RED, sprite);

  // sprite.drawLine(120, 120, 120, 120 - 100, TFT_RED);
  sleep(1);
  count--;
  if (count < 0)

  {
    mode = 2;
    png.pushRotated(&sprite, i_deg);
  }

  sprite.pushSprite(40, 0);
}

void loop()
{
  switch (mode)
  {
  case 0:
    loop_mode0();
    break;
  case 1:
    loop_mode1();
    break;
  default:
    break;
  }

  if (M5.Touch.ispressed())
  {
    mode = 1;
    Serial.println("Screen Touched.");
  }

  delayMicroseconds(1 * 1000);
}
