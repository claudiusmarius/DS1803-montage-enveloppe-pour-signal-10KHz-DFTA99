  // ===========================================================================================================================================
  // Double potentiomètre numérique utilisant un DS1803 et un ATtiny85
  // Par Claude DUFOURMONT
  // claude.dufourmont@laposte.net
  // SVP : N'OUBLIEZ PAS DE LIKER LA VIDEO ET DE VOUS ABONNER - MERCI
  // Chaine YouTube : https://www.youtube.com/c/ClaudeDufourmont
  // Vidéo : https://youtu.be/2uEe2bbEKXY
  // Diminution StepTime le 21/08/23
  // ===========================================================================================================================================
  // Datasheet DS1803 : https://www.analog.com/media/en/technical-documentation/data-sheets/ds1803.pdf
  // Pour info : le DS1803 possède 256 positions.(2 fois).
  // IMPERATIF : NOK si pas de résistances PULL UP Sur SDA et SCK
  // on fait des bonds superieurs ou égaux à 19 mV (5000/255).
  // ===============================================Standard Resistance Values DEBUT=============================================================
  // DS1803-010 10kΩ
  // DS1803-050 50kΩ
  // DS1803-100 100kΩ
  // =================================================Standard Resistance Values FIN==============================================================
  
  // ====================================================ADRESSES DEBUT===========================================================================
  //  A2  A1  A0  Adresse I2C
  //  0   0   0   0x28
  //  0   0   1   0x29
  //  0   1   0   0x2A
  //  0   1   1   0x2B
  //  1   0   0   0x2C
  //  1   0   1   0x2D
  //  1   1   0   0x2E
  //  1   1   1   0x2F
  // =====================================================ADRESSES FIN============================================================================

  //  ===================================================WIRE COMMAND WORDS DEBUT=================================================================  
  //  COMMAND                   COMMAND VALUE
  //  Write Potentiometer-0     10101001
  //  Write Potentiometer-1     10101010
  //  Write Both Potentiometers 10101111
  //  IMPORTANT : mettre un B devant le mot pour bien faire apparaître qu'il sagit d'un Byte, un Octet
  //  =====================================================WIRE COMMAND WORDS FIN=================================================================

  // Système volatiles : pas de mémorisation des dernières valeurs suite à coupure
    
  #include <TinyWireM.h>
  #include <Adafruit_NeoPixel.h>
  
  #define DS1803_ADDRESS 0x2F                       // A0,A1,A2 de DS1803 = HIGH
  #define VR A3
  #define LED_PIN 1
  #define BrocheBuzzer 4
  #define NUM_LEDS 16
  #define BRIGHTNESS 4 
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
  
  int positionPot0 = 0;
  int positionPot1 = 0;
 
  float Vselect;

  bool UPP0 = LOW;
  bool UPP1 = LOW;
  bool DOWNP0 = LOW;
  bool DOWNP1 = LOW;
  bool RAS = HIGH;

  void setup() 
  {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  
  TinyWireM.begin();
 
  pinMode (VR, INPUT);
  pinMode (BrocheBuzzer, OUTPUT);
  delay(300);
    
  strip.begin();
  strip.clear();
  strip.show(); 
  for (int i = 0; i < NUM_LEDS; i++) 
  {
  strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show(); // Afficher les modifications sur la barre de LEDs
  delay(300);
  
  digitalWrite (BrocheBuzzer, HIGH);
  delay(10); 
  digitalWrite (BrocheBuzzer, LOW);
  delay(80);

  digitalWrite (BrocheBuzzer, HIGH);
  delay(10); 
  digitalWrite (BrocheBuzzer, LOW);
  delay(80);

  digitalWrite (BrocheBuzzer, HIGH);
  delay(10); 
  digitalWrite (BrocheBuzzer, LOW);
  delay(80);
  }
  
  void loop() 
  {
  if (positionPot0 <1)
  {
  INIT0();
  }

  if (positionPot1 <1)
  {
  INIT1();
  }

  Vselect = analogRead(A3);
  
  if (Vselect <200)
  {
  UPP0 = HIGH;
  DOWNP0 = RAS = UPP1 = DOWNP1 = LOW;
  delay(5);
  }

  if (Vselect >= 200 && Vselect <280)
  {
  DOWNP0 = HIGH;
  UPP0 = RAS = UPP1 = DOWNP1 = LOW;
  delay(5);
  }

  if (Vselect >= 280 && Vselect <540)
  {
  RAS = HIGH;
  UPP0 = UPP1 = DOWNP0 = DOWNP1 = LOW;
  delay(5);
  }
 
  if (Vselect >= 540 && Vselect <620)
  {
  DOWNP1 = HIGH;
  UPP0 = RAS = DOWNP0 = UPP1 = LOW;
  delay(5);
  }

  if (Vselect >= 620 && Vselect <690)
  {
  RAS = HIGH;
  UPP0 = DOWNP0 = UPP1 = DOWNP1 = LOW;
  delay(5);
  }
 
  if (Vselect >= 690 && Vselect <760)
  {
  UPP1 = HIGH;
  UPP0 = DOWNP1 = RAS = DOWNP0 = LOW;
  delay(5);
  }
  
  if (Vselect >= 760)
  {
  RAS = HIGH;
  UPP0 = DOWNP0 = UPP1 = DOWNP1 = LOW;
  delay(5);
  }

  if (UPP0 == HIGH) 
  {
  DOWNP0 = RAS = UPP1 = DOWNP1 = LOW;
  incrementerPosition0();
  UPP0=LOW;
  }

  if (DOWNP0 == HIGH) 
  {
  UPP0 = RAS = UPP1 = DOWNP1 = LOW;
  decrementerPosition0();
  DOWNP0 = LOW;
  }

  if (UPP1 == HIGH) 
  {
  UPP0 = RAS = DOWNP0 = DOWNP1 = LOW;
  incrementerPosition1();
  UPP1 =LOW;
  }

  if (DOWNP1 == HIGH) 
  {
  UPP0 = UPP1 = RAS = DOWNP0=LOW;
  decrementerPosition1();
  DOWNP1 =LOW;
  }
  }
 
  void incrementerPosition0() 
  {
  if (positionPot0 < 255) 
  {
  positionPot0++;
  //delay(200);
  //delay(50);
  delay(5);
  TinyWireM.beginTransmission(DS1803_ADDRESS);
  TinyWireM.send(B10101001);
  TinyWireM.send(positionPot0);
  MAJBarrette0();
  TinyWireM.endTransmission();
  }
  }

  void decrementerPosition0() 
  {
  if (positionPot0 > 0) 
  {
  positionPot0--;
  //delay(200);
  //delay(50);
  delay(5);
  TinyWireM.beginTransmission(DS1803_ADDRESS);
  TinyWireM.send(B10101001);
  TinyWireM.send(positionPot0);
  MAJBarrette0();
  TinyWireM.endTransmission();
  }
  }

  void incrementerPosition1() 
  {
  if (positionPot1 < 255) 
  {
  positionPot1++;
  //delay(200);
  //delay(50);
  delay(5);
  TinyWireM.beginTransmission(DS1803_ADDRESS);
  TinyWireM.send(B10101010);
  TinyWireM.send(positionPot1);
  MAJBarrette1();
  TinyWireM.endTransmission();
  }
  }

  void decrementerPosition1() 
  {
  if (positionPot1 > 0) 
  {
  positionPot1--;
  //delay(200);
  //delay(50);
  delay(5);
  TinyWireM.beginTransmission(DS1803_ADDRESS);
  TinyWireM.send(B10101010);
  TinyWireM.send(positionPot1);
  MAJBarrette1();
  TinyWireM.endTransmission();
  }
  }

  void MAJBarrette0()
  {
  for (int i = 0; i < 8; i++) 
  {
  if (positionPot0 & (1 << (7 - i))) 
  {
  strip.setPixelColor(i, strip.Color(0, 255, 0)); // Allumer la LED correspondante en vert
  } 
  else 
  {
  strip.setPixelColor(i, strip.Color(0, 0, 0)); // Éteindre la LED
  }
  }
  strip.show(); // Afficher les modifications sur la barre de LEDs
  //delay(300);
  //delay(50);
  delay(5); 
  }

  void MAJBarrette1()
  {
  for (int i = 8; i < NUM_LEDS; i++) 
  {
  
  if (positionPot1 & (1 << (15 - i))) 
  {
  strip.setPixelColor(i, strip.Color(255, 0, 0)); // Allumer la LED correspondante en vert
  } 
  else 
  {
  strip.setPixelColor(i, strip.Color(0, 0, 0)); // Éteindre la LED
  }
  }
  strip.show(); // Afficher les modifications sur la barre de LEDs
  //delay(300);
  //delay(50);
  delay(5); 
  }

  void INIT0 ()
  {
  positionPot0 = 0;
  TinyWireM.beginTransmission(DS1803_ADDRESS);
  TinyWireM.send(B10101001);
  TinyWireM.send(positionPot0);
  TinyWireM.endTransmission();
  for (int i = 0; i <7; i++)
  {
  strip.setPixelColor( i, strip.Color(0, 0, 0)); // Éteindre la LED
  strip.show(); // Afficher les modifications sur la barre de LEDs
  }
  //delay(300);
  //delay(50);
  delay(5);
  }

  void INIT1 ()
  {
  positionPot1 = 0;
  TinyWireM.beginTransmission(DS1803_ADDRESS);
  TinyWireM.send(B10101010);
  TinyWireM.send(positionPot1);
  TinyWireM.endTransmission();
  for (int i = 8; i <15; i++)
  {
  strip.setPixelColor( i, strip.Color(0, 0, 0)); // Éteindre la LED
  strip.show(); // Afficher les modifications sur la barre de LEDs
  }
  //delay(300);
  //delay(50);
  delay(5);
  }
  
  
