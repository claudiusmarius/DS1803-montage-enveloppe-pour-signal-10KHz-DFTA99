  //RampeXIAOPlaquetteAD98330000005
  //https://youtu.be/XTWUN7pIVek
  //https://www.youtube.com/c/ClaudeDufourmont
  // SCLK et SDATA peuvent être commun à l'ensemble des AD9833
  // IMPERATIF : NOK si pas de résistances PULL UP Sur SDA et SCK su DS1803
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
  
  #include <SPI.h>
  #include <MD_AD9833.h>
  #include <Wire.h>
      
  #define DATA_PIN_1  2   // Broche DATA pour AD9833 1
  #define CLK_PIN_1   3   // Broche CLK pour AD9833 1
  #define FSYNC_PIN_1 1   // Broche FSYNC pour AD9833 1

  #define FSYNC_PIN_2 7   // Broche FSYNC pour AD9833 2
  #define FSYNC_PIN_3 10  // Broche FSYNC pour AD9833 3

  MD_AD9833 AD1(DATA_PIN_1, CLK_PIN_1, FSYNC_PIN_1);
  MD_AD9833 AD2(DATA_PIN_1, CLK_PIN_1, FSYNC_PIN_2);
  MD_AD9833 AD3(DATA_PIN_1, CLK_PIN_1, FSYNC_PIN_3);

  //==============================================================================================================================================
  // IMPORTANT : Pour gagner des ports, DATA et CLK peuvent être communs aux 3 AD9833
  //==============================================================================================================================================
  
  #define DS1803_ADDRESS 0x28
  
  void setup() 
  {
  Wire.begin();
 
  pinMode(A8, INPUT);
   
  analogReadResolution(10);    
  
  float frequency1 = 900;
  float frequency2 = 1.5;
  float frequency3 = 50;
  
  AD1.begin();
  AD1.setMode(MD_AD9833::MODE_SINE);
  AD1.setFrequency(MD_AD9833::CHAN_0, frequency1);
    
  AD2.begin();
  AD2.setMode(MD_AD9833::MODE_TRIANGLE);
  AD2.setFrequency(MD_AD9833::CHAN_0, frequency2);

  AD3.begin();
  AD3.setMode(MD_AD9833::MODE_TRIANGLE);
  AD3.setFrequency(MD_AD9833::CHAN_0, frequency3);
  }

  void loop() 
  {
  potar ();
  }
  
  void potar ()
  {
  int analogVoltage = analogRead (A8);
  //int analogVoltageBis = ((analogVoltage - (0.05/512))/0.45)*255/512;
  //int analogVoltageBis = (analogVoltage*1023/256/0.9);
  int analogVoltageBis = (analogVoltage/0.9);
  //  ----------------------------------------------------------------------------
  // Formule conversion analogVoltageBis bidouiller pour retomber sur mes pattes
  // Je laisse les puristes trover la bonne formule
  //  ----------------------------------------------------------------------------
  
  Wire.beginTransmission(DS1803_ADDRESS);
  Wire.write(B10101001);
  Wire.write(analogVoltageBis);
  Wire.endTransmission();
  }
