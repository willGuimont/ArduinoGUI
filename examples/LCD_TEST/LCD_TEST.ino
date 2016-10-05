#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <GUI.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// TFT
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

GUI gui(&tft);

// Ornements
// You have to inherite Ornement and override
// "void draw(Adafruit_TFTLCD* tft)"

class UpArrow : public Ornement
{
  virtual void draw(Adafruit_TFTLCD* tft)
  {
    int x = getX();
    int y = getY();
    int w = getW();
    int h = getH();
    tft->fillTriangle(x +  w/2,  y + (w)/3,
                           x +  w/3,  y +   2*(w)/3,
                           x + 2*(w)/3, y + 2*(w)/3,
                           MAGENTA);
  }
};

class DownArrow : public Ornement
{
    virtual void draw(Adafruit_TFTLCD* tft)
  {
    int x = getX();
    int y = getY();
    int w = getW();
    int h = getH();
    tft->fillTriangle(x +  w/2,  y + 2*(w)/3,
                           x +  w/3,  y +   (w)/3,
                           x + 2*(w)/3, y + (w)/3,
                           BLUE);
  }
};

class TextOrnment : public Ornement
{
  virtual void draw(Adafruit_TFTLCD* tft)
  {
    int x = getX();
    int y = getY();
    
      tft->setCursor(45, y + 7);
      tft->setTextColor(BLACK);  tft->setTextSize(5);
      tft->print("Hello");
  }
};

// Callbacks
// You have to inherite Callback and override
// "void execute()"

class PrintText : public Callback
{
  public:
    PrintText(char* text)
    {
      text_ = text;
    }
    void execute()
    {
        Serial.println(text_);
    } 
  private:
    char* text_;
};

void setup(void) {
  Serial.begin(9600);
  tft.reset();
  
  uint16_t identifier = tft.readID();
  tft.begin(identifier);

  tft.fillScreen(BLACK);
 
  pinMode(13, OUTPUT);
  
  // (Button is abstract)
  // gui.addButton(new Button);
  // gui.addButton((new Button))->setOrnement(new Ornement)->setCallback(new Callback));
  
  // RectButton(int x, int y, int w, int h);
  // RectButton(int x, int y, int w, int h, long color);
  
  gui.addButton((new RectButton(10, 10, 100, 100, GREEN))->setOrnement(new UpArrow)->setCallback(new PrintText("Up button")));
  gui.addButton((new RectButton(120, 10, 100, 100, YELLOW))->setOrnement(new DownArrow)->setCallback(new PrintText("Down button")));
  gui.addButton((new RectButton(10, 120,210, 50, GREEN))->setOrnement(new TextOrnment)->setCallback(new PrintText("Text button")));
  
  // RoundButton(int x, int y, int r);
  // RoundButton(int x, int y, int r, long color);
  
  gui.addButton((new RoundButton(70, 230, 50, PINK))->setCallback(new PrintText("Round button")));
  
  gui.draw();
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height()-80, 0); // May need ajustment
    if (gui.update(p))        // update return true if updated (if the point p collide with a button)
    {
      tft.fillScreen(BLACK);  // Clear screen
      gui.draw();             // Draw GUI
      delay(50);              // Must wait for the screen to have finished drawing
    }
    
  }
}

