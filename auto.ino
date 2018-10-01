#include<LiquidCrystal.h> 
#include <EEPROM.h>
#include <Servo.h>                                         
#include <stdlib.h>

#include "AutoDIY.cpp"

//LiquidCrystal lcd(RS, E, D4, D5, D6, D7); 

//============dinh nghia su dung LCD1602====================        
                        
LiquidCrystal lcd(1, 0, A0, A1, A2, A3);   
Servo servo;
int ntcount, pos;

//=====================================================================

void display_adc(int a,int b, int c, int d){
  lcd.setCursor(0, 1);   lcd.print(a);    
  lcd.setCursor(4, 1);   lcd.print(b);   
  lcd.setCursor(8, 1);   lcd.print(c);   
  lcd.setCursor(12, 1);  lcd.print(d);                    
}

void setup(void){                                
  servo.attach(SERVO);
  
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);

  lcd.begin(16,2);
  lcd.print("ROBOTIC-STARTER"); delay(1000); lcd.clear();
  lcd.setCursor(0,0); 
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
  servo.write(150); 
  //display_adc(ng_adc[0],ng_adc[1],ng_adc[2],ng_adc[3]);
}
void loop(void){
  
  set_adc();
  
  int i = 0;
  
  if(digitalRead(CT1) == 0) {
    while(true){ 
      i++;
      do_line(85, 60, 220);
      
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
