float math_round (float value, unsigned int decimal) {

  long intValue = value * pow(10, (decimal + 1));
  int resto = intValue % 10;

  //Serial.print("r: ");
  //Serial.println(resto);

  if (resto > 5) {
    intValue = intValue + (10 - resto);
  }

  else if (resto < -5){
    intValue = intValue - (10 + resto);
  }

  return (float) (intValue/10) / pow(10, decimal);

}
