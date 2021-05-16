#include <EEPROM.h>
//#include <Wire.h>
#include <Adafruit_VEML6075.h>// library sensor uv
#include <Servo.h>

Adafruit_VEML6075 uv = Adafruit_VEML6075();
Servo myservo1;
Servo myservo2;

bool limit;
int LDR = A0;
int PIR = 13;
int addr;
//byte value;

//awal daun-daun dan jendela
void buka_jendela()
{
  //buka daun-daun
  myservo1.attach(9);
  myservo1.write(90);
  delay(3000);
  myservo1.detach();
  //buka daun-daun

  //buka jendela
  myservo2.attach(6);
  myservo2.write(180);
  delay(10000);
  myservo2.detach();
  //buka jendela
}

void tutup_jendela()
{
  //buka jendela
  myservo2.attach(6);
  myservo2.write(-180);
  delay(10000);
  myservo2.detach();
  //buka jendela

  //buka daun-daun
  myservo1.attach(9);
  myservo1.write(-90);
  delay(3000);
  myservo1.detach();
  //buka daun-daun
}

void hapuseeprom()
{
  for (int addr = 0 ; addr < EEPROM.length() ; addr++) {
    EEPROM.write(addr, 0);
  }
}

void setup() {
  Serial.begin(115200);
  //  Serial.println("VEML6075 Test");

  if (! uv.begin()) {
    Serial.println("Failed VEML6075");
  }
  else {
    Serial.println("Found VEML6075");
  }
  pinMode(PIR, INPUT);
  if (EEPROM.read(addr) == 1) {
    Serial.println("Terbuka");
    limit = true;
    delay(3000);
  }
  if (EEPROM.read(addr) == 0) {
    Serial.println("Tertutup");
    limit = false;
    delay(3000);
  }
}

void loop() {
  int cahaya = analogRead(LDR);
  int keberadaan = digitalRead(PIR);
  Serial.print("UV Index reading:  ||"); Serial.println(uv.readUVI());
  Serial.print("Cahaya:  ||"); Serial.println(cahaya);
  Serial.print("Keberadaan:  ||"); Serial.println(keberadaan);

  //awal mendeteksi keberadaan orang ketika pagi sampai menuju sore
  if (cahaya <= 100 && uv.readUVI() >= 1 && uv.readUVI() <= 9 && digitalRead(PIR) == HIGH && limit == false) { //&& EEPROM.read(address) == -180
    //    Serial.println("Pg Ada Org");
    hapuseeprom();
    EEPROM.write(addr, 1);
    buka_jendela();
    limit = true;
  }
  if (cahaya <= 100 && uv.readUVI() >= 1 && uv.readUVI() <= 9 && digitalRead(PIR) == LOW && limit == true) { //&& EEPROM.read(address) == 180
    //    Serial.println("Pg Tdk Ada Org");
    //    tdkada_orang();
    hapuseeprom();
    EEPROM.write(addr, 0);
    tutup_jendela();
    limit = false;
  }
  //akhir mendeteksi keberadaan orang ketika pagi sampai menuju sore


  //awal mendeteksi cahaya dan uv pada malam hari
  if (cahaya >= 101 && uv.readUVI() < 1 && digitalRead(PIR) == LOW && limit == true) { //&& EEPROM.read(address) == 180
    //    Serial.println("Mlm Tdk Ada Org");
    //    tdkada_orang();
    hapuseeprom();
    EEPROM.write(addr, 0);
    tutup_jendela();
    limit = false;
  }
  if (cahaya >= 101 && uv.readUVI() < 1 && digitalRead(PIR) == HIGH && limit == false) { // && EEPROM.read(address) == -180
    //    Serial.println("Mlm Ada Org");
    //    ada_orang();
    hapuseeprom();
    EEPROM.write(addr, 1);
    buka_jendela();
    limit = true;
  }
}
