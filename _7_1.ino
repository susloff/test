// OLED
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

volatile int brightness = 0;   // raw data from fotoresistor
volatile int temperature = 0;   // raw data from termistor

void adc_init(void)
{      // ADC initialize 
  ADCSRA  &= ~ _BV(ADEN);   // ADC disable
  DIDR0   |=  (_BV(ADC0D))|(_BV(ADC1D));   // digital input buffer on A0 and A1 disable 
  ADMUX   &= ~(_BV(MUX3))||(_BV(MUX2))|(_BV(MUX1))|(_BV(MUX0));   // set A0 as output (brightness)
  ADMUX   |=   _BV(REFS0);   // set voltage
  ADCSRA  |=  (_BV(ADEN))|(_BV(ADSC))|(_BV(ADIE))|(_BV(ADPS2))|(_BV(ADPS1))|(_BV(ADPS0));   /* ADC enable, start news convertation, allow interrupt, 
                                                                                            128 division factor between the system clock frequency and the input clock to the ADC */
}

double Gettemp(int RawTemp) 
{     // magic formula from the Internet =) to convert raw data to Celsius degree
  double temp;
  temp = log(((10240000/RawTemp) - 10000));
  temp = 1 / (0.001129148 + (0.000234125 * temp) + (0.0000000876741 * temp * temp * temp));
  temp = temp - 273.15;
  return temp;
}


ISR(ADC_vect)
{    // Interruption
 uint16_t Result;
  
 Result = ADC;    // read data from the output registor
 switch (ADMUX) {
    case 0x40:    // if A0
      brightness = Result;
      ADMUX   |=  _BV(MUX0);
      break;
    case 0x41:    // if A1
      temperature = Result;
      ADMUX   &= ~ _BV(MUX0);
      break;
 }
 ADCSRA |= _BV(ADSC); 
 }


void setup()
{
 adc_init();
 sei();
 Serial.begin(115200);
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 display.display();
}


void loop()
{
double temp;

temp = Gettemp(temperature);
display.clearDisplay();
display.setTextColor(WHITE);
display.setTextSize(1);
display.setCursor(1,5);
display.println("Temp: ");   
display.setTextSize(2);
display.setCursor(60,2);
display.println(temp);
display.setTextSize(1);
display.setCursor(1,39);
display.println("Light: ");
display.setTextSize(3);
display.setCursor(70,30);
display.println(brightness);
display.display(); 
delay(300);
}
