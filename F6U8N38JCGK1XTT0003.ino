  // Datasheet DS1803 : https://www.analog.com/media/en/technical-documentation/data-sheets/ds1803.pdf
  // TEST BASIQUE : Actionnement du potentiomètre 0 (uniquement)
  // Arduino NANO
  // Pour info : le DS1803 possède 256 positions.(2 fois).
  // Fonctionne le 05/06/23 à 14H33
  
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
  #include <Wire.h>

  //#define DS1803_ADDRESS 0x28
  //#define DS1803_ADDRESS 0x29
  #define DS1803_ADDRESS 0x2F
  
  void setup() 
  {
  Wire.begin();
  Serial.begin(9600);
  pinMode(A3, INPUT);                               // Le curseur W0 est envoyé sur sur A3
  }

  void loop() 
  {
  for (int i = 0; i <= 255; i++) 
  {
  Wire.beginTransmission(DS1803_ADDRESS);
  Wire.write(B10101001);                            // Voir Datasheet table 1 page 5, correspond au potentiometre 0
  Wire.write(i);
  float analogVoltage = analogRead(A3); 
  Serial.println(analogVoltage*5/1023);             // Valeur convertie en volts (hypothèse Vcc égal exactement 5V)
  delay(200);
  Wire.endTransmission();
  }

  for (int i = 255; i >= 0; i--) 
  {
  Wire.beginTransmission(DS1803_ADDRESS);
  Wire.write(B10101001);
  Wire.write(i);
  float analogVoltage = analogRead(A3); 
  Serial.println(analogVoltage*5/1023);
  delay(200);
  Wire.endTransmission();
  }
  }
