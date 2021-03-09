// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 return btnNONE;  // when all others fail, return this...
}

void cycleOption(){
  optionNum++;
  if(optionNum >= N_OPTIONS){optionNum= 0;}
  option = menu[optionNum];
  delay(250);
}
void cycleOptionReverse(){
  optionNum--;
  if(optionNum < 0){optionNum= N_OPTIONS - 1;}
  option = menu[optionNum];
  delay(250);
}
void cycleOption2(){
  optionNum2++;
  if(optionNum2 >= N_OPTIONS2){optionNum2= 0;}
  option2 = menu2[optionNum2];
  delay(250);
}
void cycleOption2Reverse(){
  optionNum2--;
  if(optionNum2 < 0){optionNum2= N_OPTIONS2 - 1;}
  option2 = menu2[optionNum2];
  delay(250);
}
