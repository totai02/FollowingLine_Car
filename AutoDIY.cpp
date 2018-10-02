#include "AutoDIY.h"

LiquidCrystal lcd(1, 0, A0, A1, A2, A3);   
Servo servo;

int  ngatu, retrai, rephai, adc[4], ng_adc[4];
int min_adc[4], max_adc[4];
int nho, tmp;
int k, error_p;

void put_text(char *text, int x, int y) {
  lcd.setCursor(x, y); 
  lcd.print(text);
}

void lcd_clear() {
  lcd.clear();
}

void set_ngatu(int n) {
  ngatu = n;
}

int get_ngatu()
{
  return ngatu;
}

void init_adc() {
  pinMode(ADC,INPUT_PULLUP); 
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  analogReference(EXTERNAL);
}

void set_adc() {
  int i;

  if ((digitalRead(ADC) == 0) && (tmp == 0)) {
    while (digitalRead(ADC) == 0) {
      read_adc();
      for(i = 0; i < 4; i++) {
        min_adc[i] = adc[i];
      }
    }
    tmp = 1;
  }
  
  if ((digitalRead(ADC) == 0) && (tmp == 1)) {
    while(digitalRead(ADC)==0) {
      read_adc();
      for (i = 0; i < 4; i++) {
        max_adc[i] = adc[i];                                             
      }
    }
    tmp = 2;
  }
  
  if (tmp == 2) {
    for (i = 0; i < 4; i++) { 
      ng_adc[i] = (min_adc[i] + max_adc[i]) / 2;
      EEPROM.put(i*2, ng_adc[i]);
    }
    tmp = 3;
  }
}

void get_adc() {
  int i;
  for (i = 0; i < 4; i++) {
    EEPROM.get(i * 2, ng_adc[i]);
  }
}

void read_adc() {
  int i;
  for(i = 0; i < 4; i++) adc[i] = analogRead(i + 4);
}
                
void read_line(void)    // thuc hien viec doc
{
    int i,j;
    k = 0;
    
    read_adc();
    
    if(adc[0] < ng_adc[0])  k |= 0b1000;   //thuc hien cong don gia tri                  
    if(adc[1] < ng_adc[1])  k |= 0b0100;
    if(adc[2] < ng_adc[2])  k |= 0b0010;
    if(adc[3] < ng_adc[3])  k |= 0b0001;
    
    if ((adc[0] < ng_adc[0]) && (adc[1] < ng_adc[1]) && (adc[2] < ng_adc[2]) && (adc[3] < ng_adc[3]) && (nho != 1)) {
      ngatu++;
      nho = 1;
    } else if ((adc[0]<ng_adc[0])&&(adc[1]<ng_adc[1])&&(nho!=2)){
      retrai = 1;
      nho = 2;
    } else if ((adc[3] < ng_adc[3]) && (adc[2] < ng_adc[2]) && (nho != 2)) {
      rephai = 1;
      nho = 2;
    } else if ((analogRead(0) > ng_adc[0]) || (analogRead(1) > ng_adc[1]) || (analogRead(2) > ng_adc[2]) || (analogRead(3) > ng_adc[3])) {
      nho=0;
    } 
}

void clearr(void) {
  retrai = 0;
  rephai = 0;
}

int middle(int input, int low_limit, int high_limit){
  if (input > low_limit) {
    if (input < high_limit) {
      return input;
    } else {
      return high_limit;
    }
  } else {
    return low_limit;   
  }     
}

void do_line(int kp, int kd, int spd) {
  int pTerm, dTerm;
  int error = 100;  
  int pd_change;
  int v_trai, v_phai;       
                                       
  read_line();   
  
  if (k == 0b0001) error = -3;
  else if (k == 0b0011) error = -2;
  else if (k == 0b0010) error = -1;
  else if (k == 0b0110) error =  0 ;
  else if (k == 0b0100) error =  1 ;
  else if (k == 0b1000) error =  3 ;
  else if (k == 0b1100) error =  2 ;
  else if ((error < -3) || (error > 3)) error = error_p;

  dTerm = kd * (error - error_p);
  pTerm = kp * error;
  pd_change = middle(pTerm + dTerm, -255, 255);

  v_trai = middle(spd + pd_change, 0, 255);
  v_phai = middle(spd - pd_change + 20, 0, 255); 

  control_motor(0, 0, v_phai);     //Dieu khien motor khi do duong
  control_motor(1, 0, v_trai);
  
  k = 0b0000;
  error_p = error;
}

void init_pin(void) {
  pinMode(DIR1,OUTPUT);  //DIR1;
  pinMode(PWM1,OUTPUT);  //PWM1
  pinMode(PWM2,OUTPUT);  //PWM2
  pinMode(DIR2,OUTPUT);  //DIR2

  pinMode(CT1,INPUT_PULLUP);
  pinMode(CT2,INPUT_PULLUP);
  
  pinMode(TRIG,OUTPUT);   
  pinMode(ECHO,INPUT);

  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);

  servo.attach(SERVO);
  lcd.begin(16,2);
}

void control_motor(int dc, int dir, int spd) {
  switch (dc){
    case 0: {
      if (dir == 1) {
        digitalWrite(DIR1, 0);
        analogWrite(PWM1, spd);
      } else {
        digitalWrite(DIR1,1);
        analogWrite(PWM1, 255-spd);  
      }
      break;
    }
    case 1: {
      if (dir == 1) {
        digitalWrite(DIR2, 0);
        analogWrite(PWM2, spd);
      } else {
        digitalWrite(DIR2,1);
        analogWrite(PWM2, 255-spd);  
      }
      break;
    }
  }
}

int get_distance() {
    unsigned long duration; 
    int distance;           
    
    digitalWrite(TRIG,LOW);   
    delayMicroseconds(2);
    digitalWrite(TRIG,HIGH);  
    delayMicroseconds(10);  
    digitalWrite(TRIG,LOW);  
    
    duration = pulseIn(ECHO,HIGH);  
    distance = int(duration/2/29.412);
    
    return distance;
}

void display_adc(int a,int b, int c, int d) {
  lcd.setCursor(0, 1);   lcd.print(a);    
  lcd.setCursor(4, 1);   lcd.print(b);   
  lcd.setCursor(8, 1);   lcd.print(c);   
  lcd.setCursor(12, 1);  lcd.print(d);                    
}
