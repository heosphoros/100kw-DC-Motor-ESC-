#define SERIAL_ENABLED 1
#define LCD_ENABLED 1
#include <Wire.h>
#if LCD_ENABLED
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2, LCD_5x8DOTS);

#endif
// These defines won't change.  They're used to give names to the pins used:

#define CURRENT_PIN A0 // A1302 hall senzor
#define TEMP_PIN A1 //  pinA1 - lm35 temp senzor   
#define FB2_PIN A2 // FB2     947 - 562   (Bosch)
#define FB1_PIN A3 // FB1     157 - 940   (Bosch)
#define HV_PIN A6 // volt-HV 100k & 2.2k
#define ROTOR_PWM_PIN 6 // MainPWM for Rotor
#define STATOR_PWM_PIN 9 // SecPWM for Stator

int MainFB = 0;               // MainFB ( TPS1 )
int SecFB = 0;                // SecFB ( TPS2 )
float HiVoltage = 0;            // Hi  Voltage 0-220v
int MainPWM = 0;              // MainPWM for Rotor
int SecPWM = 0;               // SecPWM for Stator
int tempValue = 0;            //  lm35 -temperature sensor
float AMP = 0;                  //  A1302 hall senzor



void setup() {
  pinMode(STATOR_PWM_PIN, OUTPUT);
  pinMode(ROTOR_PWM_PIN, OUTPUT);
  digitalWrite(STATOR_PWM_PIN, 0);
  digitalWrite(ROTOR_PWM_PIN, 0);
#if SERIAL_ENABLED
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
#endif
#if LCD_ENABLED
  // initialize the LCD
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world1!");
#endif
}

void loop() {
  // read the analog in value:
  tempValue = (5.0 * analogRead(A1) * 100.0) / 1024;
  AMP = 0;
  HiVoltage = 0;
  for (char i = 0; i < 30; i++) {
    AMP += analogRead(CURRENT_PIN);
    HiVoltage += analogRead(HV_PIN);
  }
  AMP = (AMP / 30 - 514.5) / 2-1.05;
  //HiVoltage = (5.0 * HiVoltage/30 * 4750.0) / 2048 ;
  HiVoltage = HiVoltage / 264.0;
  //MainFB = analogRead(FB1_PIN );
  //MainFB = constrain(MainFB, 154, 940);
  SecFB =  analogRead(FB2_PIN );
  SecFB = constrain(SecFB, 310, 760); 
  // map it to the range of the analog out:
  //mai intai te uiti ca iti vine MAINFB si SECFB ca limite si bagi limitele in map
  //MainPWM = map(MainFB, 154, 940, 0, 12); //limita laPWM 4.7%
  SecPWM = map(SecFB, 310, 770, 0, 12);   //limita laPWM 4.7%
  // change the analog out value:
  //analogWrite(ROTOR_PWM_PIN, MainPWM);
  analogWrite(STATOR_PWM_PIN, SecPWM);
#if SERIAL_ENABLED
  // print the results to the serial monitor:
  //  Serial.print("MainFB = ");
  //  Serial.print(MainFB);
  Serial.print("\t SecFB = ");
  Serial.print(SecFB);
  Serial.print("\t HiVoltage = ");
  Serial.print(HiVoltage);
  Serial.print("\t AMP = ");
  Serial.print(AMP);
  Serial.print("\t temp = ");
  Serial.print(tempValue);
  //  Serial.print("\t MainPwm = ");
  //  Serial.print(MainPWM);
  //  Serial.print("\t SecPwm = ");
  //  Serial.print(SecPWM);
  Serial.println( );
#endif
#if LCD_ENABLED
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(AMP);
  lcd.print(" A");
  lcd.setCursor(9, 0);
  lcd.print(tempValue);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print(HiVoltage);
  lcd.print(" V");
#endif
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(200);
}
