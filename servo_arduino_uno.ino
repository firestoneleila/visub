

#include <Servo.h>
#include "U8glib.h"

Servo ESC1; 
Servo ESC2;
Servo ESC3;

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);

void setup() {

  setup_oled();
  u8g.firstPage();  
  do {
    draw();
  }
  while( u8g.nextPage() );
  
  ESC1.attach(9,1000,2000); // Servo/Motor
  ESC2.attach(10,1000,2000); // Servo/Motor
  ESC3.attach(11,1000,2000); // Servo/Motor
  
  

  
  pinMode(8, OUTPUT); // Relay
  digitalWrite(8, HIGH);
  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  delay(5000);

  for (int signal = 1500; signal<1600; signal+=1) {
      ESC1.writeMicroseconds(signal);
      ESC2.writeMicroseconds(signal);
      ESC3.writeMicroseconds(signal);
      delay(10);
  }

  delay(2000);
  
  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  


  
}

void loop() { 
  

}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22, "hello");
  
}

void setup_oled(void) {
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
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
    u8g.setHiColorByRGB(255,255,255);
  }
  

}
