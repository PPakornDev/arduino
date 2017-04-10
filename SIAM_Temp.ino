#include <Wire.h> // Library for bus I2C
#include <dht.h>  // Library for humidity sensor
#include <LCD.h>  // Library for LCD
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <Pitches.h> // Library for Melody

#define DHT11_PIN 7   // กำหนดขา Data ของ Humidity Sensor
#define I2C_ADDR 0x3F // กำหนดตำแหน่ง Address ของ I2C
#define BACKLIGHT_PIN 3

dht DHT; //Variable for Humidity Sensor
LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);

unsigned short pinRed = 11;   // RGB LED for red color at pin 11
unsigned short pinGreen = 10; // RGB LED for green color at pin 10
unsigned short pinBlue = 9;   // RGB LED for blue color at pin 9

/* เสียงของ Melody */
int melody[] = { 
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

/* ช่วงความยาวของเสียง */
int noteDurations[] = { 
  4, 8, 8, 4, 4, 4, 4, 4 
};

void setup() {
  // put your setup code here, to run once:

  /* Initialize LCD */
  lcd.begin (16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH); // Open Backlight LCD
  lcd.home(); // ไปที่ตัวอักษรที่ 0 แถวที่ 1

  /* Initialize pin for RGB LED */
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);

  /* Initialize pin for Damp */
  pinMode(4, OUTPUT); // red color for "damp"
  pinMode(3, OUTPUT); // yellow color for "damp"
  pinMode(2, OUTPUT); // green color for "damp"

  /* Welcome screen */
  lcd.setCursor(4, 0);
  lcd.print("Welcome");
  lcd.setCursor(3, 1);
  lcd.print("SIAM Temp");

  /* Play melody to welcome */
  for(int thisNote = 0; thisNote < 8; thisNote++)
  {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }

  /* initialize serial communication at 9600 bits per second */
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();  // ลบข้อมูลที่หน้าจอออก
  int chk = DHT.read11(DHT11_PIN);  // อ่านค่าจากตัว Humidity Sensor

  /* Display Temperature */
  lcd.setCursor(0,0);
  lcd.print("Temp ");
  lcd.print(DHT.temperature);

  /* Display Humidity */
  lcd.setCursor(0,1);
  lcd.print("Damp ");
  lcd.print(DHT.humidity);

  /* ตรวจสอบอุณหภูมิพร้อมแสดงสถานะไฟ RGB LED*/
  if(DHT.temperature >= 30)
  {
    setColor(255, 0, 0); // Red Color
  }
  else
  {
    setColor(0, 255, 0); // Green Color
  }
  
  /* ตรวจสอบความชื้นพร้อมแสดงสถานะไฟ LED*/
  if(DHT.humidity >= 60)
  {
    tone(8, 3520, 200); //เล่นเสียงเตือน
    digitalWrite(4,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(2,LOW);
  }
  else if((DHT.humidity >= 50) && (DHT.humidity <= 59))
  {
    digitalWrite(4,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(2,LOW);
  }
  else
  {
    digitalWrite(4,LOW);
    digitalWrite(3,LOW);
    digitalWrite(2,HIGH);
  }

  String data[] = { String(DHT.temperature), String(DHT.humidity)};
  Serial.println(data[0] + "," + data[1]);
  
  delay(1000);  // หน่วงเวลา 1 วินาที
  noTone(8);    // ปิดเสียง
}

/* Method Display Color */
void setColor(int red, int green, int blue)
{
  analogWrite(pinRed, red);
  analogWrite(pinGreen, green);
  analogWrite(pinBlue, blue);
}
