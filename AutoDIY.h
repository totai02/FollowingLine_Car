#include<LiquidCrystal.h> 
#include <EEPROM.h>
#include <Servo.h>                                         
#include <stdlib.h>
#include "Arduino.h"

#define PWM1 6
#define DIR1 7
#define PWM2 5
#define DIR2 4
#define SERVO 8
#define ADC   12
#define CT1  10
#define CT2  11
#define TRIG 3
#define ECHO 2

void init_adc();
void set_adc();
void get_adc();
void read_adc();
void read_line(void);
void clearr(void);
int middle(int input, int low_limit, int high_limit);
void do_line(int kp, int kd, int spd);
void init_pin(void);
void control_motor(int dc, int dir, int spd);
int get_distance();
void display_adc(int a,int b, int c, int d);
void put_text(char *text, int x, int y);
void lcd_clear();
void set_ngatu(int n);
int get_ngatu();
