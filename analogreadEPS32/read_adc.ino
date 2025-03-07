float average;
int prev;

float adc_readings(adc1_channel_t ch, int readings, int discards, int round) { 

  long values[readings];
  long sum_values = 0;
  int n = 0;

  for (int i = 0; i < readings; i++) {

    values[i] = adc1_get_raw(ch);
    n++;

    delay(1);
  }

  /*for (int i = 0; i < readings ; i++) {
     Serial.print(i);
     Serial.print(";");
     Serial.println(values[i]);
    }*/

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n - 1; j++) {
      if (values[j] > values[j + 1]) {
        prev = values[j];
        values[j] = values[j + 1];
        values[j + 1] = prev;
      }
    }
  }

  /*for (int i = 0; i < readings; i++) {
     Serial.print(i);
     Serial.print(";");
     Serial.println(values[i]);
    }*/

  for (int i = discards; i < readings - discards; i++) {
    sum_values += values[i];
  }

  average = float(sum_values) / float(readings - discards * 2);

  //Serial.print("avg: ");
  //Serial.println(average);

  return math_round(average, round);
}