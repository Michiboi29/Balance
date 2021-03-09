#include <LiquidCrystal.h>
#include <PID_v2.h>
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel, buttons and other inputs
int lcd_key     = 0;
int adc_key_in  = 0;
int adc_current_in = 0;
#define ADC_I      10
#define PI_INPUT   11
#define PI_OUTPUT  10
#define btnRIGHT   0
#define btnUP      1
#define btnDOWN    2
#define btnLEFT    3
#define btnSELECT  4
#define btnNONE    5
#define N_OPTIONS  5
#define N_OPTIONS2 6
float poidsPieces[N_OPTIONS2] = {2.35, 3.95, 1.75, 4.4, 6.27, 7.3};

//var affichage
char* menu[N_OPTIONS] = {
"Masse(g)             ",
"Masse(once)          ",
"Tarre                ",
"Compter              ",
"Authentifie         "};
int optionNum= 0;
int optionSelect = 0;
char* option = menu[optionNum];
char* menu2[N_OPTIONS2] = {
"0.01$           ",
"0.05$           ",
"0.10$           ",
"0.25$           ",
"1$              ",
"2$              "};
int optionNum2= 0;
int optionSelect2 = 0;
char* option2 = menu2[optionNum2];

// var calcul
float Kc = 0.8464;
float g = 9.81;
float Kf = 0.949;
float masse = 0;
float incertitude = 0.3;
float masse_once = masse * 0.03527396195;
float Vmes;
float masse_tarre;
int compte;
float masse_save;
int auth;

// var regulateur (a changer !!!)
float v0 = 0;
float E = 69;
float J = 420;
float a = 0.69;
float l = 1.42;
float R_eq = 1;
float R_b = 1.234;
float L = 0.000019;
float Kamp = 2.61;
float Kvdo = 3;
float Kvd = 0.19;
float di = 69;

// var regulateur 2.0
double Kp = 2, Ki = 5, Kd = 0;
unsigned long lastTime_main = 0, SampleTime_main = 1000;
double min_out = 0, max_out = 4.8;
double curr_output = 0.00, setpoint = 0.00;
PID_v2 pi(Kp, Ki, Kd, PID::Reverse); 


void setup() {
 Serial.begin(9600);
 Serial.println("allo");
 lcd.begin(16, 2);              
 lcd.setCursor(0, 0);
 lcd.print("Created By");
 lcd.setCursor(0,1);
 lcd.print("Etienne Michaud");
 delay(2000);
 lcd.clear();
 lcd.setCursor(0,0);
 // regulateur 
 pi.SetOutputLimits(min_out, max_out);
 pi.SetSampleTime(SampleTime_main);
 pi.Start(analogRead(PI_INPUT),  // input
              curr_output,                 // current output
              setpoint);              // setpoint ==> val initial !
}
String show = String(masse) + "g                ";

void loop() {
 // affichage
 lcd.setCursor(0,1);          
 lcd.print(show);      
 lcd.setCursor(0,0);            
 lcd_key = read_LCD_buttons();  // read the buttons
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     cycleOption();
     lcd.print(option);
     break;
     }
   case btnLEFT:
     {
     cycleOptionReverse();
     lcd.print(option);
     break;
     }
   case btnUP:
     {
     if(optionNum == 3 || optionNum == 4){cycleOption2();}
     lcd.print(option);
     break;
     }
   case btnDOWN:
     {
     if(optionNum == 3 || optionNum == 4){cycleOption2Reverse();}
     lcd.print(option);
     break;
     }
   case btnSELECT:
     {
       if (optionNum == 0){show = String(masse) + "g              ";}
       else if (optionNum == 1){masse_once = masse * 0.03527396195; show = String(masse_once) + "once           ";}
       else if (optionNum == 2){tarre();}
       else if (optionNum == 3){optionSelect2 = optionNum2;}
       else if (optionNum == 4){optionSelect2 = optionNum2;}
       if (optionNum != 2){optionSelect = optionNum;}
       lcd.print(option);
       break;
     }
   case btnNONE:
   {
   lcd.print(option);
   if (optionNum == 3 || optionNum == 4){
      lcd.setCursor(11,0);
      lcd.print(option2);
      lcd.setCursor(0,0);
   }
   if (optionSelect == 0){show = String(Vmes) + "V " + String(masse) + "g           ";}
   else if (optionSelect == 1){masse_once = masse * 0.03527396195; show = String(masse_once) + "once            ";}
   else if (optionSelect == 3){
    compte = compter (optionSelect2);
    show = String(compte) + " x " + String(menu2[optionSelect2]) + "      ";
   }
   else if (optionSelect == 4){
    auth = authentifier (optionSelect2);
    if (auth){show = "Oui                    ";}
    else{show = "Non                     ";}
   }      
   break;
   }
 }

 // monitoring + regulateur
 unsigned long now_main = millis();
 unsigned long timeChange_main = (now_main - lastTime_main);
 if(timeChange_main>=SampleTime_main){
  read_masse();
  const double input = analogRead(PI_INPUT);
  const double output = pi.Run(input*5.00/1023.00);
  const int out_bit = int(output/5.00 *255);
  Serial.print(String(input) + "  ");
  Serial.print(String(output) + "  ");
  Serial.println(out_bit);
  analogWrite(PI_OUTPUT, out_bit);
  lastTime_main = now_main;
 }
}
