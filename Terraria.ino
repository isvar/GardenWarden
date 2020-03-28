#include <LiquidCrystal_I2C.h>
#include <DS1307RTC.h >
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <TimeLib.h>

// moisture sensors Pins and values
int senmoisture_pin_1 = A1;
int senmoisture_pin_2 = A2;
float moisture_1;
float moisture_2;

// Light Sensor Pins and values
int photoresistance_pin = A0;
float photoresistance_val;

// Digital Temperature Sensor setup
int temp_sensor_pin = 8;
#define DHTTYPE DHT11
DHT dht(temp_sensor_pin, DHTTYPE);
int tempt_check_threshold = 2000;
unsigned long prev_time = 0;
float humidity = 0;
float temp = 0;

tmElements_t tm;

// Informative Leds Pins and values
int led_pin_1 = 2;
int led_pin_2 = 4;
bool pin_state = false;

//set up lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
Serial.begin(9600);

lcd.init();
lcd.backlight();

dht.begin();

pinMode(led_pin_1, OUTPUT);
pinMode(led_pin_2, OUTPUT);
}

void loop() {
  unsigned long current_time = millis();

  photoresistance_val = analogRead(photoresistance_pin);

  moisture_1 = analogRead(senmoisture_pin_1);
  moisture_2 = analogRead(senmoisture_pin_2);

  if (current_time - prev_time >= tempt_check_threshold){
    humidity = dht.readHumidity();
    temp = dht.readTemperature();
    prev_time = current_time;
    }
  
  lcd.setCursor(0,0);
  lcd.print("Humedad");
  lcd.setCursor(0,1);
  lcd.print(humidity);
  lcd.setCursor(6,1);
  lcd.print("%");
  delay(750);
  for (int i = 0; i < 15; i++){
    lcd.scrollDisplayRight();
    delay(150);
    }
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("Temperatura");
  lcd.setCursor(0,1);
  lcd.print(temp);
  lcd.setCursor(6,1);
  lcd.print("C");  
  delay(750);
  for (int i = 0; i < 15; i++){
    lcd.scrollDisplayRight();
    delay(150);
    }
  lcd.clear();

// Displaying Time to lcd
  if(RTC.read(tm)){
    lcd.setCursor(0,0);  
    lcd.print("Current Time");
    lcd.setCursor(0,1);
    print2digitsLCD(tm.Hour);
    lcd.setCursor(2,1);
    lcd.print(":");
    lcd.setCursor(3,1);
    print2digitsLCD(tm.Minute);
    lcd.setCursor(5,1);
    lcd.print(":");
    lcd.setCursor(6,1);
    print2digitsLCD(tm.Second);
    delay(750);
    lcd.clear();
    }
  
  if (pin_state){
    digitalWrite(led_pin_1, HIGH);
    digitalWrite(led_pin_2, LOW);
    pin_state = false;
    
    }else{
    digitalWrite(led_pin_1, LOW);
    digitalWrite(led_pin_2, HIGH);
    pin_state = true;
      }

  if(RTC.read(tm)){
    Serial.print("Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println(" ");    
    }

  Serial.print("Luminicencia: ");
  Serial.print(photoresistance_val);
  Serial.print(" Humedad de suelo: ");
  Serial.print(moisture_1);
  Serial.print(" ");
  Serial.print(moisture_2);
  Serial.print(" Humedad: ");
  Serial.print(humidity);
  Serial.print(" Temperatura: ");
  Serial.println(temp);
  delay(2000);

}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void print2digitsLCD(int number){
  if (number >= 0 && number < 10){
    String zero = "0";
    lcd.print(zero + number);
    }else{
    lcd.print(number);
      }
  }
