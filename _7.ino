#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// светодиодная шкала подключена к группе пинов расположенных
// подряд. Даём понятные имена первому и последнему пинам
#define FIRST_LED_PIN  2
#define LAST_LED_PIN   11

volatile int brightness = 0;

void adc_init(void)
{ 
// &= ~ обнуление бита
// |= присвоение биту 1
ADCSRA &= ~ _BV(ADEN); // отключаем ацп
DIDR0  |=   _BV(ADC0D); // выбираем вход а0
ADMUX  &= ~ (_BV(MUX3))|(_BV(MUX2))|(_BV(MUX1))|(_BV(MUX0));
ADMUX  |=   _BV(REFS0);
ADCSRA |=   (_BV(ADPS2))|(_BV(ADPS1))|(_BV(ADPS0)); //делитель частоты 128
ADCSRA |=   (_BV(ADEN))|(_BV(ADSC))|(_BV(ADIE));  // включаем ацп / запускаем одиночное преобразование / разрешаем прерывание после завершения преобразования
}

ISR(ADC_vect)
{
 brightness = ADC; // отсылаем данные
 ADCSRA |= _BV(ADSC); //запускаем новое одиночное преобразование
 }

void setup()
{
  // в шкале 10 светодиодов. 
 Serial.begin(115200);
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 display.display();
 for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; ++pin)
   pinMode(pin, OUTPUT); 
 adc_init();
 sei();
}
 
void loop()
{
int result = ((brightness) / 100);

for (int pin = FIRST_LED_PIN; pin <= result+1; ++pin) 
   digitalWrite(pin, HIGH);
delay(100);
for (int pin = FIRST_LED_PIN; pin <= result+1; ++pin) 
   digitalWrite(pin, LOW); 
   
display.clearDisplay();
display.setTextColor(WHITE);
display.setTextSize(1);
display.setCursor(1,5);
display.println("LED's: ");
display.setTextSize(2);
display.setCursor(100,2);
display.println(result);
display.setTextSize(1);
display.setCursor(1,39);
display.println("Brightness: ");
display.setTextSize(3);
display.setCursor(70,30);
display.println(brightness);
display.display();
}

