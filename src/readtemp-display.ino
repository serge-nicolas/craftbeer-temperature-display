#include <Arduino.h>

#include <U8glib.h>
#include <OneWire.h>


const int SERIAL_SPEED = 115200;

/*
    DS180 Configuration
*/
/* Availaible resolutions */
const byte RESOLUTION_12_BITS = 0b01111111;
const byte RESOLUTION_11_BITS = 0b01011111;
const byte RESOLUTION_10_BITS = 0b00111111;
const byte RESOLUTION_9_BITS = 0b00011111;
/* 1-Wire connected at port */
const byte BROCHE_ONEWIRE = 7;
/* Return code from getTemperature() */
enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reçue invalide
  INVALID_SENSOR  // Capteur invalide (pas un DS18B20)
};

/*
    KY34-B Configuration
*/
/* Display screen iniit KY34-B*/
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI
const int SCREEN_REFRESH_DELAY = 50;

/* String constant */
const String temp = "Temp: ";
const String unit = "C";




/*https://www.carnetdumaker.net/articles/mesurer-une-temperature-avec-un-capteur-1-wire-ds18b20-et-une-carte-arduino-genuino/*/
/* Create the one wire bus */
OneWire ds(BROCHE_ONEWIRE);



/** Change the resolution of the DS180 specified, need hard reset (and comment call) after use */
void changeResolution(const byte addr[], byte resolution) {
  ds.reset();
  ds.select(addr);
  ds.write(0x4E);
  ds.write(0x00);
  ds.write(0x00);
  ds.write(resolution);
  ds.reset();
}

/**
   Read temperature via DS18B20.
   @
*/
byte getTemperature(float *temperature, byte reset_search) {
  byte data[9], addr[8];
  // data[] : Données lues depuis le scratchpad
  // addr[] : Adresse du module 1-Wire détecté

  /* 1-Wire bus reset (needed for first  */
  if (reset_search) {
    ds.reset_search();
  }

  /* Get the first */
  if (!ds.search(addr)) {
    // Pas de capteur
    return NO_SENSOR_FOUND;
  } else {

  }

  /* Check address */
  if (OneWire::crc8(addr, 7) != addr[7]) {
    // Adresse invalide
    return INVALID_ADDRESS;
  }

  /* Check for a real DS18B20 */
  if (addr[0] != 0x28) {
    return INVALID_SENSOR;
  }

  /* Reset le bus 1-Wire et sélectionne le capteur */
  ds.reset();
  ds.select(addr);

  /* Lance une prise de mesure de température et attend la fin de la mesure */
  ds.write(0x44, 1);
  delay(800);

  /* Reset le bus 1-Wire, sélectionne le capteur et envoie une demande de lecture du scratchpad */
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);

  //changeResolution(addr,RESOLUTION_12_BITS);

  /* Lecture du scratchpad */
  for (byte i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  /* Calcul de la température en degré Celsius */
  *temperature = ((data[1] << 8) | data[0]) * 0.0625;

  // Pas d'erreur
  return READ_OK;
}
/** DISPLAY **/
/*
   Setup the KY34-B
*/
void setupDisplay() {
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
}


void drawOnScreen(String str) {
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 20);
  u8g.print(str);
}

void displayOnScreenLoop(String str) {
  u8g.firstPage();
  do {
    drawOnScreen(str);
  } while ( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(SCREEN_REFRESH_DELAY);
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  setupDisplay();

}

void loop() {
  float temperature;
  String message;
  /* read the temp at 1HZ  */
  if (getTemperature(&temperature, true) != READ_OK) {
    message = "Error";
    Serial.println(F("Error when reading temp"));
    return;
  } else {
    Serial.println(temperature, 2);
    String message = temp;
    message += temperature;
    message += unit;
  }


  displayOnScreenLoop(message);

}
