void read_masse(){
    //adc_current_in = analogRead(ADC_I); // read the value from the amp
    adc_current_in = random(0,1023);
    Vmes = adc_current_in * 5.00 / 1023.00; 
    masse = calcul_masse (Vmes);
}


float calcul_masse (float Vmes){
  float Ib = Vmes/Kc; //courent bobine
  float F = Ib * Kf;
  float m = 1000*F/g;  
  return m;
}


void tarre(){
  masse_tarre = masse;
  masse = 0;
  }

int compter (int piece){
  return (masse/poidsPieces[piece]);
  }

int authentifier(int piece){
  masse_save = masse;
  if(masse_save <= poidsPieces[piece] + incertitude && masse_save >= poidsPieces[piece] - incertitude){return 1;}
  else {return 0;}
  }
