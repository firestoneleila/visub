#include <Servo.h>
#include "U8glib.h"

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
#define INTERRUPT_PIN 2


Servo ESC1;
Servo ESC2;
Servo ESC3;

#define LEFT_MOTOR 9
#define MIDDLE_MOTOR 10
#define RIGHT_MOTOR 11

#define RELAY_PIN 8
#define LIPO_CELL1 14 //A0
#define LIPO_CELL2 15 //A1
#define LIPO_CELL3 16 //A2

#define KILL_SWITCH 17//A3

#define RES_DIV_1 1
#define RES_DIV_2 0.50 //10k and 10k
#define RES_DIV_3 0.642 //18k and 10k
//18k and 10k resistor dividers
//Vout = Vin*(18k)/(10k+18k)
//18k/28k = 0.642

#define BUZZER_PIN 3

#define BUTTON_TOP 4
#define BUTTON_MIDDLE 6
#define BUTTON_BOTTOM 7

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);

void setup() {

  setup_oled();
  u8g.firstPage();
  do {
    draw();
  }
  while ( u8g.nextPage() );

  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);
  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);


  pinMode(BUTTON_TOP,INPUT);
  pinMode(BUTTON_MIDDLE,INPUT);
  pinMode(BUTTON_BOTTOM,INPUT);
  pinMode(KILL_SWITCH,INPUT);
  
  pinMode(BUZZER_PIN,OUTPUT);

  ESC1.attach(LEFT_MOTOR, 1000, 2000);
  ESC2.attach(MIDDLE_MOTOR, 1000, 2000);
  ESC3.attach(RIGHT_MOTOR, 1000, 2000);

  pinMode(RELAY_PIN, OUTPUT); // Relay
  digitalWrite(RELAY_PIN, HIGH);
  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  delay(5000);

  for (int signal = 1500; signal < 1600; signal += 1) {
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
    u8g.setHiColorByRGB(255, 255, 255);
  }


}

byte check_buttons()
{
  /*
     #define BUTTON_TOP 4
     #define BUTTON_MIDDLE 6
     #define BUTTON_BOTTOM 7
  */
  //total number of button combinations 2^3=8 states
  return digitalRead(BUTTON_TOP) + 2 * digitalRead(BUTTON_MIDDLE) + 4 * digitalRead(BUTTON_BOTTOM);

}

double check_battery()
{
  double total_voltage = analogRead(LIPO_CELL3) * 5 / 1023 / RES_DIV_3;
  return total_voltage;
}

double check_cell(byte cell_number)
{
  double cell_voltage_1, cell_voltage_2, cell_voltage_3;

  cell_voltage_1 = analogRead(LIPO_CELL1) * 5 / 1023 / RES_DIV_1;
  cell_voltage_2 = analogRead(LIPO_CELL2) * 5 / 1023 / RES_DIV_2 - cell_voltage_1;
  cell_voltage_3 = analogRead(LIPO_CELL3) * 5 / 1023 / RES_DIV_3 - cell_voltage_2 - cell_voltage_1;

  if (cell_number == 1)
  {
    return  cell_voltage_1;
  }

  else if(cell_number == 2)
    {
      return  cell_voltage_2;
    }

  else if(cell_number == 3)
    {
      return  cell_voltage_3;
    }

  else
  {
    return -1;
  }


}
