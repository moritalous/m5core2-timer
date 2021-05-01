#include <M5Core2.h>
#include <utility/Sprite.h>

int x = 120;
int y = 120;
int r = 120;

int i_deg = 0;
int count = 0;
int mode = 0;

TFT_eSprite sprite = TFT_eSprite(&M5.Lcd);
TFT_eSprite dial = TFT_eSprite(&M5.Lcd);
TFT_eSprite moji = TFT_eSprite(&M5.Lcd);
TFT_eSprite png = TFT_eSprite(&M5.Lcd);

void drawDial(int degree, uint32_t color, TFT_eSprite sprite)
{
  float pi = 3.14;
  float rad = degree * pi / 180;

  sprite.drawLine(x, y, (int)(x + r * cos(rad)), (int)(y + r * sin(rad)), color);
}

void drawDial(int degree)
{
  drawDial(degree, TFT_BLACK, dial);
}

void createDial()
{
  dial.setColorDepth(16);
  dial.createSprite(240, 240);
  dial.fillScreen(TFT_BLACK);
  dial.fillCircle(x, y, 120, TFT_WHITE);

  for (int i = 0; i < 60; i++)
  {
    drawDial(i * 6);
  }
  dial.fillCircle(x, y, 110, TFT_WHITE);

  for (int i = 0; i < 12; i++)
  {
    drawDial(i * 30);
  }
  dial.fillCircle(x, y, 100, TFT_WHITE);

  dial.drawLine(x, y, x, y - 100, TFT_BLACK);
}

void createPng()
{
  png.createSprite(96, 96);
  png.fillScreen(WHITE);
  png.drawJpgFile(SD, "/notifications_active_black_48dp.jpg", 0, 0, 96, 96, 0, 0);
}

void drawMoji(String str)
{
  moji.fillScreen(TFT_WHITE);
  moji.drawCentreString(str, 50, 140, FONT4);

  moji.pushRotated(&sprite, i_deg);
}

int getHandDegree()
{
  return (count - (i_deg <= 0 ? i_deg * -1 : 360 - i_deg) / 6) * 6 - 90;
}

void drawHand(int degree)
{
  drawDial(degree, TFT_RED, sprite);
}

void setup(void)
{
  M5.begin();
  M5.IMU.Init();

  M5.Lcd.fillScreen(TFT_BLACK);

  sprite.setColorDepth(16);
  sprite.createSprite(240, 240);

  createDial();
  createPng();

  moji.createSprite(100, 200);
}

void update_i_deg()
{
  float ax, ay, az;
  M5.IMU.getAccelData(&ax, &ay, &az);

  float f_deg = atan2(ax, ay) / 3.14 * 180;
  i_deg = (int)(f_deg / 6) * 6;
}

void loop_mode0()
{
  update_i_deg();
  count = (i_deg <= 0 ? i_deg * -1 : 360 - i_deg) / 6;

  dial.pushRotated(&sprite, i_deg);
  drawMoji(String((i_deg <= 0 ? i_deg * -1 : 360 - i_deg) / 6));
  drawHand(-90);

  sprite.pushSprite(40, 0);
}

void loop_mode1()
{
  dial.pushRotated(&sprite, i_deg);
  drawMoji(String(count));
  drawHand(getHandDegree());

  sprite.pushSprite(40, 0);

  count--;

  sleep(1);
}

void loop_mode2()
{
  dial.pushRotated(&sprite, i_deg);
  drawHand(getHandDegree());
  png.pushRotated(&sprite, i_deg);

  sprite.pushSprite(40, 0);
}

void loop()
{

  if (mode == 0 && M5.Touch.ispressed())
  {
    mode = 1;
  }

  if (mode == 1 && count < 1)
  {
    mode = 2;
  }

  switch (mode)
  {
  case 0:
    loop_mode0();
    break;
  case 1:
    loop_mode1();
    break;
  case 2:
    loop_mode2();
    break;
  default:
    break;
  }

  delayMicroseconds(1 * 1000);
}
