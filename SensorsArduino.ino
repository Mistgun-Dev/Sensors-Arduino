#include "pins_arduino.h"
#include "Servo.h"

const byte SERVO_SIG_PIN = 9;    // Broche SIGNAL du servoMoteur
const byte TRIGGER_PIN   = A4;    // Broche TRIGGER Ultrason
const byte ECHO_PIN      = A5;    // Broche ECHO Ultrason
const byte PUSH_PIN      = 13;    // Broche PUSH Infrarouge
const byte LED_RIGHT     = 11;
const byte LED_LEFT      = 10;
const byte BUZZER = A1;
bool SHORTCUT_MODE = false;
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s
const float SOUND_SPEED = 340.0 / 1000;
byte code = 0;
static int newMode = 0;
int angleServo = 0;
Servo MyServo;
int goServo = 0;
unsigned long previousMillis = 0;                // sert a définir l'intervalle de temp des mouvement du servo Moteur, (non bloquant)
unsigned int joystickX = 0;
unsigned int joystickY = 0;

#define myDigitalRead(pin) ((*portInputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin)) ? HIGH : LOW)
#define BUTTON_1              17
#define BUTTON_2              18
#define BUTTON_3              19
#define BUTTON_4              20
#define BUTTON_5              21
#define BUTTON_6              22
#define BUTTON_QUIT_SHORTCUT  9
#define BUTTON_UP             0
#define BUTTON_RIGHT          2
#define BUTTON_DOWN           1
#define BUTTON_LEFT           3
#define BUTTON_MIDDLE         68

int angle = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(PUSH_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_RIGHT, OUTPUT); 
  pinMode(LED_LEFT, OUTPUT); 
  pinMode(BUZZER, OUTPUT); 
  
  MyServo.attach (SERVO_SIG_PIN);
  MyServo.write(90);
}

void readJoystick()
{
  /*
  joystickX = analogRead(AXE_X);
  joystickY = analogRead(AXE_Y);

  Serial.print(F("\nX = "));
  Serial.print(joystickX);

  Serial.print(F("\n  Y = "));
  Serial.print(joystickY);
 */
}

void getDistance()
{
  static int printDistance = 0;

  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);

  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
  float distance_cm = distance_mm / 10.0;
  
  int delayBlink;
  if(distance_cm > 100)
    delayBlink = 250;
  else if(distance_cm > 75 && distance_cm < 100)
    delayBlink = 200;
  else if(distance_cm > 50 && distance_cm < 75)
    delayBlink = 150;
  else
    delayBlink = distance_cm*2;
    
  digitalWrite(LED_LEFT, HIGH);
  delay(delayBlink);
  digitalWrite(LED_LEFT, LOW);
  delay(delayBlink*2);
  digitalWrite(LED_RIGHT, HIGH);
  delay(delayBlink);
  digitalWrite(LED_RIGHT, LOW);

  digitalWrite(BUZZER, HIGH);// Faire du bruit
  delay(delayBlink);
  digitalWrite(BUZZER, LOW);// Silence
  delay(delayBlink*2);
  
  /* Affiche les résultats en mm, cm et m */
  if (printDistance++ % 10 == 0)
  {
    Serial.print(F("Distance: "));
    Serial.print(distance_mm / 10.0, 2);
    Serial.println(F("cm, "));
  }
}

void event(int code)
{
  //Si le bouton de SHORTCUT
  if(SHORTCUT_MODE == true)
  {   
     if(code == BUTTON_QUIT_SHORTCUT)
     {
          SHORTCUT_MODE = false;
          Serial.println("Exit du mode Raccourcis !");
          code = -1;
     }
     else
     {
        if(newMode)
        {
          Serial.println(code);
          newMode = 0;
        }
     }

    return;
  }
  
  switch (code)
  {
    case BUTTON_1 :
      if (newMode)
      {
        if(code <= 3 || code == 68)
          delay(50);
        Serial.println("Le capteur ultrason a été activé !\n");
        newMode = 0;
      }
      getDistance();
      break;
    case BUTTON_2 :
      if (newMode)
      {
        Serial.println("\nLe capteur ultrason a été arrêté !\n");
        newMode = 0;
      }
      break;
    case BUTTON_3 :
        if (newMode)
        {
          Serial.println("\nLancement du mode raccourci !\n");
          SHORTCUT_MODE = true;
          newMode = 0;
        }
     case BUTTON_RIGHT :
      if (newMode)
        {
          if(SHORTCUT_MODE == false)
          {
              // Fait bouger le bras de 0° à 180°
              MyServo.write(135);
              delay(50);
              newMode = 0;
          }
        }
      break;
     case BUTTON_LEFT :
      if (newMode)
        {
          if(SHORTCUT_MODE == false)
          {
              // Fait bouger le bras de 0° à 180°
              MyServo.write(180);
              delay(50);
              newMode = 0;
          }
        }
      break;
  }

  code = -1;
}

void loop()
{ 
    MyServo.write(90);

  static int i = 0;
  int pshState = myDigitalRead(PUSH_PIN);
  
  if (pshState == HIGH && !i)
  {
    code  = myDigitalRead (2)
            | myDigitalRead (3) << 1
            | myDigitalRead (4) << 2
            | myDigitalRead (5) << 3
            | myDigitalRead (6) << 4
            | myDigitalRead (7) << 5
            | myDigitalRead (8) << 6
            | myDigitalRead (12) << 7;
      
    newMode = 1;
    if(code > 3 && code != 68) //sert a éviter la répétition de la touche, sauf pour les cas 0, 1 2 3
      i++;
  }
  if (pshState == LOW )
  {
    i = 0;   
  }   
  //setAngleServoMoteur();
  event(code);
  delay(20);
}
