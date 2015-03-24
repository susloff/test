// светодиодная шкала подключена к группе пинов расположенных
// подряд. Даём понятные имена первому и последнему пинам
//#define FIRST_LED_PIN  2
//#define LAST_LED_PIN   11

volatile int brightness = 0;

void adc_init(void)
{ 
// &= ~ обнуление бита
// |= присвоение биту 1
ADCSRA &= ~ (1<<ADEN); // отключаем ацп
DIDR0 |= (1 << ADC0D); // выбираем вход а0
ADCSRA |= (ADPS2<<1)|(ADPS1<<1)|(ADPS0<<1); //делитель частоты 128
ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADIE);  // включаем ацп / запускаем одиночное преобразование / разрешаем прерывание после завершения прерывания
}

ISR(ADC_vect)
{
 brightness = ADCH; // отсылаем данные
 ADCSRA |= (1<<ADSC); //запускаем новое одиночное преобразование
 Serial.println(brightness);
 }

void setup()
{
  // в шкале 10 светодиодов. 
  //for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; ++pin)
   // pinMode(pin, OUTPUT); 
 Serial.begin(115200);
 adc_init();
 sei();
}
 
void loop()
{

 //Serial.println(brightness);
/*  int result = (brightness / 100);
  for (int pin = FIRST_LED_PIN; pin <= result; ++pin) 
   digitalWrite(pin, HIGH);
  delay(100);
  for (int pin = FIRST_LED_PIN; pin <= result; ++pin) 
   digitalWrite(pin, LOW); 
   */
  
}

