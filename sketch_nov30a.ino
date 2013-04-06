#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET  13
#define OLED_CS     12
#define OLED_DC     11
#define OLED_CLK    10
#define OLED_MOSI    9

#define OLED_CS_2    4
#define OLED_RESET_2 7
#define OLED_CS_3    3
#define OLED_RESET_3 6
#define OLED_CS_4    2
#define OLED_RESET_4 8

Adafruit_SSD1306 display1(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
Adafruit_SSD1306 display2(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET_2, OLED_CS_2);
Adafruit_SSD1306 display3(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET_3, OLED_CS_3);
Adafruit_SSD1306 display4(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET_4, OLED_CS_4);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

uint8_t waterTemp  = 0;
uint8_t fuelLevel  = 128;
uint8_t delayValue = 500;
uint8_t oilTemp    = 0;
uint8_t voltage    = 0;

void setup()   {                
//  Serial.begin(9600);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display1.begin(SSD1306_SWITCHCAPVCC);
  display2.begin(SSD1306_SWITCHCAPVCC);
  display3.begin(SSD1306_SWITCHCAPVCC);
//  display4.begin(SSD1306_SWITCHCAPVCC);
  // init done
  display1.display(); // show splashscreen on screen 1
  display2.display(); // show splashscreen on screen 2
  display3.display(); // show splashscreen on screen 3
//  display4.display(); // show splashscreen on screen 4
  
  delay(2000);
  display1.clearDisplay();   // clears the screen and buffer
  display2.clearDisplay();   // clears the screen and buffer
  display3.clearDisplay();   // clears the screen and buffer
//  display4.clearDisplay();   // clears the screen and buffer

  fuelLevel = 128;
  drawFUEL();             //draw the FUEL gauge
  display2.display();
  delay(50);
  display2.clearDisplay();
  updateFUEL();

  waterTemp = 0;
  drawWATERTEMP();        //draw the WATER TEMP gauge
  display1.display();
  delay(50);
  display1.clearDisplay();
  
  oilTemp = analogRead(A1);
  drawOILTEMP();         //draw the OIL TEMP gauge
  display3.display();
  delay(50);
  display3.clearDisplay();

  voltage = analogRead(A0);
//  drawVOLTAGE();          //draw the VOLTAGE gauge
//  display4.display();
  delay(50);
//  display4.clearDisplay();
}


void loop() {

  //read the WATER TEMP sensor
  //if changed then output the change to the correct display
  if(waterTemp >= 64) { //100
    waterTemp = 64;     //100
  } else {
    waterTemp = waterTemp+1;
  }
//  Serial.print("WaterTemp=");Serial.println(waterTemp);
  updateWATERTEMP();
  delay(delayValue);
  
  static unsigned long nextFUELReadTime = 5000L; //300000L; //5mins
    if( nextFUELReadTime < millis() )
    {
      nextFUELReadTime = millis() + 5000L; //300000L; //5mins

      //read the FUEL sensor (probably do this once a minute? doesn't need to be that accurate!)
      //if changed then output the change to the correct display
      if(fuelLevel <= 10) {
        fuelLevel = 0;
      } else {
        fuelLevel = fuelLevel - 1;
      }
//      Serial.print("FuelLevel=");Serial.println(fuelLevel);
      updateFUEL();
    }


  //read the OIL TEMP sensor
  //if changed then output the change to the correct display
  oilTemp = analogRead(A1);
  Serial.print("oilTemp=");Serial.println(oilTemp);
  updateOILTEMP();
  delay(delayValue);

  //read the VOLTAGE sensor
  //if changed then output the change to the correct display
  voltage = analogRead(A0);
  Serial.print("voltage=");Serial.println(voltage);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float realVoltage = voltage * (5.0 / 1023.0);
  Serial.print("dec voltage=");Serial.println(realVoltage);
  //should have a check here to see if voltage is the same, if yes, no need to update display
  updateVOLTAGE();
  delay(delayValue);
}

void drawWATERTEMP(void) {
//  display1.clearDisplay();
  display1.drawTriangle(64,0,  0,50, 128,50, WHITE);
  display1.drawTriangle(64,0, 20,50, 108,50, WHITE);
  display1.drawTriangle(64,0, 40,50,  88,50, WHITE);
  display1.drawLine(64,0, 64,50, WHITE);
  display1.fillRect(0, 0, 128, 40, BLACK);
  display1.drawFastHLine(0,50, 128, BLACK);

  display1.setTextSize(1);
  display1.setTextColor(WHITE);
  display1.setCursor(47,5);
  display1.println("SMITHS");
  display1.setCursor(32,25);
  display1.println("TEMPERATURE");
  display1.setCursor(0,54);
  display1.println("COLD");
  display1.setCursor(47,54);
  display1.println("NORMAL");
  display1.setCursor(110,54);
  display1.println("HOT");
}

void drawFUEL(void) {
  display2.clearDisplay();
  display2.drawTriangle(64,0,  0,50, 128,50, WHITE);
  display2.drawTriangle(64,0, 20,50, 108,50, WHITE);
  display2.drawTriangle(64,0, 40,50,  88,50, WHITE);
  display2.drawLine(64,0, 64,50, WHITE);
  display2.fillRect(0, 0, 128, 40, BLACK);
  display2.drawFastHLine(0,50, 128, BLACK);

  display2.setTextSize(1);
  display2.setTextColor(WHITE);
  display2.setCursor(47,5);
  display2.println("SMITHS");
  display2.setCursor(41,20);
  display2.println("CAPACITY");
  display2.setCursor(44,29);
  display2.println("8 GALLS");
  display2.setCursor(0,54);
  display2.println("0");
  display2.setCursor(57,54);
  display2.println("1/2");
  display2.setCursor(122,54);
  display2.println("F");
}

void drawOILTEMP(void) {
  display3.drawTriangle(64,0,  0,50, 128,50, WHITE);
  display3.drawTriangle(64,0, 20,50, 108,50, WHITE);
  display3.drawTriangle(64,0, 40,50,  88,50, WHITE);
  display3.drawLine(64,0, 64,50, WHITE);
  display3.fillRect(0, 0, 128, 40, BLACK);
  display3.drawFastHLine(0,50, 128, BLACK);

  display3.setTextSize(1);
  display3.setTextColor(WHITE);
  display3.setCursor(47,5);
  display3.println("SMITHS");
  display3.setCursor(32,25);
  display3.println("OIL TEMP");
  display3.setCursor(0,54);
  display3.println("COLD");
  display3.setCursor(47,54);
  display3.println("NORMAL");
  display3.setCursor(110,54);
  display3.println("HOT");
}

void drawVOLTAGE(void) {
  display4.drawTriangle(64,0,  0,50, 128,50, WHITE);
  display4.drawTriangle(64,0, 20,50, 108,50, WHITE);
  display4.drawTriangle(64,0, 40,50,  88,50, WHITE);
  display4.drawLine(64,0, 64,50, WHITE);
  display4.fillRect(0, 0, 128, 40, BLACK);
  display4.drawFastHLine(0,50, 128, BLACK);

  display4.setTextSize(1);
  display4.setTextColor(WHITE);
  display4.setCursor(47,5);
  display4.println("SMITHS");
  display4.setCursor(32,25);
  display4.println("VOLTAGE");
  display4.setCursor(0,54);
  display4.println("DEAD");
  display4.setCursor(45,54);
  display4.println("WARNING");
  display4.setCursor(90,54);
  display4.println("NORMAL");
}

void drawTEMPGauge(uint8_t waterlevel)
{
  display1.drawLine(64,0, waterlevel,  58, WHITE);
  display1.drawLine(65,0, waterlevel+1,58, WHITE);
  display1.drawLine(66,0, waterlevel+2,58, WHITE);
}

void drawFUELGauge(uint8_t fuellevel)
{
  display2.drawLine(64,0, fuellevel,  58, WHITE);
  display2.drawLine(65,0, fuellevel+1,58, WHITE);
  display2.drawLine(66,0, fuellevel+2,58, WHITE);
}

void drawOILTEMPGauge(uint8_t oillevel)
{
  display1.drawLine(64,0, oillevel,  58, WHITE);
  display1.drawLine(65,0, oillevel+1,58, WHITE);
  display1.drawLine(66,0, oillevel+2,58, WHITE);
}

void drawVOLTAGEGauge(uint8_t voltage)
{
  display4.drawLine(64,0, voltage,  58, WHITE);
  display4.drawLine(65,0, voltage+1,58, WHITE);
  display4.drawLine(66,0, voltage+2,58, WHITE);
}

void updateWATERTEMP(void) {
  if(waterTemp < 100) {
    drawWATERTEMP();
    drawTEMPGauge(waterTemp);
  } else if(waterTemp == 100) {
    drawWATERTEMP();
    drawTEMPGauge(waterTemp);
    display1.invertDisplay(true);
    delay(150);
    display1.invertDisplay(false);
    delay(150);
    display1.invertDisplay(true);
    delay(150);
    display1.invertDisplay(false);
    delay(150);
    display1.invertDisplay(true);
    delay(150);
    display1.invertDisplay(false);
  }
  display1.display();
  display1.clearDisplay();
}

void updateFUEL(void) {
  if(fuelLevel >= 30) {
    drawFUEL();
    drawFUELGauge(fuelLevel);
  } else if(fuelLevel != 0){
    drawFUEL();
    drawFUELGauge(fuelLevel);
    display2.setTextSize(2);
    display2.setTextColor(WHITE);
    display2.setCursor(20,51);
    display2.println("FUEL LOW");
  } else if(fuelLevel == 0) {
    drawFUEL();
    drawFUELGauge(fuelLevel);
    display2.setTextSize(3);
    display2.setTextColor(WHITE);
    display2.setCursor(30,5);
    display2.println("FUEL");
    display2.setCursor(20,35);
    display2.println("EMPTY");
    display2.invertDisplay(true);
    delay(150);
    display2.invertDisplay(false);
    delay(150);
    display2.invertDisplay(true);
    delay(150);
    display2.invertDisplay(false);
    delay(150);
    display2.invertDisplay(true);
    delay(150);
    display2.invertDisplay(false);
  }
  display2.display();
  display2.clearDisplay();
}

void updateOILTEMP(void) {
  if(oilTemp < 100) {
    drawOILTEMP();
    drawOILTEMPGauge(oilTemp);
  } else if(oilTemp == 100) {
    drawOILTEMP();
    drawOILTEMPGauge(oilTemp);
    display3.invertDisplay(true);
    delay(150);
    display3.invertDisplay(false);
    delay(150);
    display3.invertDisplay(true);
    delay(150);
    display3.invertDisplay(false);
    delay(150);
    display3.invertDisplay(true);
    delay(150);
    display3.invertDisplay(false);
  }
  display3.display();
  display3.clearDisplay();
}

void updateVOLTAGE(void) {
  if(voltage > 500) {
    drawVOLTAGE();
    drawVOLTAGEGauge(voltage);
  } else if(voltage < 500) {
    drawVOLTAGE();
    drawVOLTAGEGauge(voltage);
    display4.invertDisplay(true);
    delay(150);
    display4.invertDisplay(false);
    delay(150);
    display4.invertDisplay(true);
    delay(150);
    display4.invertDisplay(false);
    delay(150);
    display4.invertDisplay(true);
    delay(150);
    display4.invertDisplay(false);
  }
  display4.display();
  display4.clearDisplay();
}

