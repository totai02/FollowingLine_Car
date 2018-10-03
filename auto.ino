#include "AutoDIY.h"

//============dinh nghia su dung LCD1602====================

int ntcount, pos;

//=====================================================================

void setup(void) {
  lcd.begin(16, 2);
  lcd.print("ROBOTIC-STARTER");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AUTODIY");
  if (digitalRead(9) == 0) {
    lcd.print(" SAN TRANG");
  } else {
    lcd.print(" SAN DEN ");
  }

  init_pin();
  init_adc();
  get_adc();
  ngatu = 0;
  clearr();
  ntcount = 0;
  display_adc();
}
void loop(void) {

  set_adc();

  int i = 0;

  if (digitalRead(CT1) == 0) {
    while (true) {
      i++;
      do_line(80, 50, 200);

      if (ngatu >= 1) {
        ngatu = 0;
        control_motor(0, 0, 0);
        control_motor(1, 0, 0);
        break;
      }

      //      if (get_distance() <= 15) {
      //        ngatu = 0;
      //        control_motor(0, 0, 0);
      //        control_motor(1, 0, 0);
      //        servo.write(10);
      //        delay(1000);
      //        servo.write(150);
      //      }
    }
  }
}
