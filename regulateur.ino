
float regulateur_pos(float vin){
  // Détermination de l'erreur
  float errV = v0 - vin;
  float errX = (errV / Kvdo)/Kvd;
  // Force à ajouter (à enlever si negative)
  float F = (errX * 6*J*E)/(3*l*pow(a,2) - pow(a,3));
  // Courrant nécessaire à ajouter à la bobine
  float I = F/Kf;
  // tension qui doi aller dans la bobine
  float V = (R_b + R_eq) * I + L * di;
  // tension qui doit aller dans l'ampli
  float Vo  = V/Kamp;
  
  float Vout = Vo + v0;
  //analogWrite(PWM, (Vout/5) * 255);
  
  return Vout;
}
