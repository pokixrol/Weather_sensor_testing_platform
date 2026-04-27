#include <SparkFun_AS3935.h>
#include <Wire.h>

#define AS3935_ADDR 0x03
#define INDOOR 0x12 
#define OUTDOOR 0x0E
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01
 
SparkFun_AS3935 lightning(AS3935_ADDR);
 
const int pinPreruseni = 4; 
int hodnota = 0;
 
void setup() {
  // Interrupt pin pro detekci, HIGH = aktivni preruseni
  pinMode(pinPreruseni, INPUT); 
 
  Serial.begin(115200); 
  Serial.println("AS3935 Inicializace"); 
 
  Wire.begin(21,22); 
 
  if( !lightning.begin() ) 
  { 
    Serial.println ("Problem s inicializaci. Zkontrolujte propojeni."); 
    while(1); 
  }
  else
    Serial.println("Detektor blesku pripraven");
 
  //lightning.setIndoorOutdoor(OUTDOOR);
}
 
void loop() {
  if(digitalRead(pinPreruseni) == HIGH) // Preruseni - blesk, sum nebo ruseni detekovano
  {
    hodnota = lightning.readInterruptReg(); 
    
    if(hodnota == NOISE_INT){
      Serial.println("Detekovan sum!"); 
      //lightning.setNoiseLevel(Hodnota); // hodnota od 1 do 7
    }
    else if(hodnota == DISTURBER_INT){
      Serial.println("Detekovano ruseni!"); 
      //lightning.watchdogThreshold(Hodnota);  // hodnota od 1 do 10
 
      //lightning.maskDisturber(true); // hodnota true nebo false
    }
    else if(hodnota == LIGHTNING_INT){
      Serial.println("Detekovan BLESK!"); 
 
      // Cteni odhahodave vzdalenosti blesku
      byte vzdalenostBlesku = lightning.distanceToStorm(); 
      Serial.print("Priblizne: "); 
      Serial.print(vzdalenostBlesku); 
      Serial.println("km daleko!"); 
 
      // Cteni odhadovane energie blesku
      long energieBlesku = lightning.lightningEnergy(); 
      Serial.print("Odhadovana energie blesku: "); 
      Serial.println(energieBlesku); 
    }
  }
  delay(100);
}