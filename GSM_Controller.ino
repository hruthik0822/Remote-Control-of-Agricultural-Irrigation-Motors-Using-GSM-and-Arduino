#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Fan A3
#define Light 4
#define TV 5

int temp = 0, i = 0;
int led = 13;
char str[15];

// This function runs once when the Arduino starts up
void setup()
{
  lcd.init();

  // Turn on the backlight and print a message.
  lcd.backlight();
  Serial.begin(9600);

  pinMode(led,   OUTPUT);
  pinMode(Fan,   OUTPUT);
  pinMode(Light, OUTPUT);
  pinMode(TV,    OUTPUT);

  digitalWrite(led,   HIGH);
  digitalWrite(Fan,   HIGH);
  digitalWrite(Light, HIGH);
  digitalWrite(TV,    HIGH);

  lcd.setCursor(0, 0);
  lcd.print("  GSM Control   ");
  lcd.setCursor(0, 1);
  lcd.print("    Automaton   ");
  delay(2000);

  lcd.clear();
  lcd.print("Motor On/OFF    ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  
  // Serial.println("AT+CNMI=2,2,0,0,0");
  delay(500);
  // Serial.println("AT+CMGF=1");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Motor           ");
  lcd.setCursor(0, 1);
  lcd.print("OFF             ");
}

// This function runs repeatedly after setup() is finished
void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Motor           ");
  if (temp == 1)
  {
    check();
    temp = 0;
    i = 0;
    delay(1000);
  }
}

// This function is called automatically when serial data is available
void serialEvent()
{
  while (Serial.available())
  {
    if (Serial.find("#A."))
    {
      digitalWrite(led, HIGH);
      delay(1000);
      digitalWrite(led, LOW);

      while (Serial.available())
      {
        char inChar = Serial.read();
        str[i++] = inChar;
        if (inChar == '*')
        {
          temp = 1;
          return;
        }
      }
    }
  }
}

// This function checks the received command and controls the devices
void check()
{
  if (!(strncmp(str, "tv on", 5)))
  {
    digitalWrite(TV, HIGH);
    lcd.setCursor(13, 1);
    lcd.print("      ");
    delay(200);
  }
  else if (!(strncmp(str, "tv off", 6)))
  {
    digitalWrite(TV, LOW);
    lcd.setCursor(13, 1);
    lcd.print("        ");
    delay(200);
  }
  else if (!(strncmp(str, "fan on", 5)))
  {
    digitalWrite(Fan, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("ON   ");
    delay(200);
  }
  else if (!(strncmp(str, "fan off", 7)))
  {
    digitalWrite(Fan, LOW);
    lcd.setCursor(0, 1);
    lcd.print("OFF    ");
    delay(200);
  }
  else if (!(strncmp(str, "light on", 8)))
  {
    digitalWrite(Light, HIGH);
    lcd.setCursor(7, 1);
    lcd.print("      ");
    delay(200);
  }
  else if (!(strncmp(str, "light off", 9)))
  {
    digitalWrite(Light, LOW);
    lcd.setCursor(7, 1);
    lcd.print("        ");
    delay(200);
  }
  else if (!(strncmp(str, "all on", 6)))
  {
    digitalWrite(Light, LOW);
    digitalWrite(Fan, LOW);
    digitalWrite(TV, LOW);
    lcd.setCursor(0, 1);
    lcd.print("ON                ");
    delay(200);
  }
  else if (!(strncmp(str, "all off", 7)))
  {
    digitalWrite(Light, HIGH);
    digitalWrite(Fan, HIGH);
    digitalWrite(TV, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("OFF               ");
    delay(200);
  }
}