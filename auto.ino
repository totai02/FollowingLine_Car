#include "AutoDIY.h"

//============dinh nghia su dung LCD1602====================        
                       
int ntcount, pos;

//=====================================================================

void setup(void){                               

  put_text("ROBOTIC-STARTER", 0, 0);
  delay(1000); 
  lcd_clear();
  put_text("AUTODIY", 0, 0); 
  if (digitalRead(9) == 0) { 
    put_text(" SAN TRANG", 0, 0);
  } else { 
    put_text(" SAN DEN ", 0, 0); 
  }
  
  init_pin();
  init_adc();
  get_adc();
  set_ngatu(0);
  clearr();
  ntcount = 0;
  //display_adc(ng_adc[0],ng_adc[1],ng_adc[2],ng_adc[3]);
}
void loop(void){
  
  set_adc();
  
  int i = 0;
  
  if(digitalRead(CT1) == 0) {
    while(true){ 
      i++;
      do_line(85, 60, 220);
      
      if (get_ngatu() >= 1) {
        set_ngatu(0);
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
