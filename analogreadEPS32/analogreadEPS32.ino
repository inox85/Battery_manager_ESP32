#include <driver/adc.h>
#include <esp_adc_cal.h>

#define ADC_CHANNEL ADC1_CHANNEL_0  // GPIO36
#define ADC_ATTEN ADC_ATTEN_DB_0     // Attenuazione a 0 dB (range fino a 1.1V)
#define ADC_WIDTH ADC_WIDTH_BIT_12   // Risoluzione a 12 bit

esp_adc_cal_characteristics_t *adc_chars;

void setup() {
  Serial.begin(115200);
  
  // Allocazione dinamica della struttura tramite calloc
  adc_chars = (esp_adc_cal_characteristics_t *) calloc(1, sizeof(esp_adc_cal_characteristics_t));
  
  // Caratterizzazione dell'ADC: controlla se esistono dati calibrati negli eFuse
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH, ESP_ADC_CAL_VAL_EFUSE_VREF, adc_chars);
  
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
      Serial.print("eFuse Vref: ");
      Serial.println(adc_chars->vref);
  } 
  else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
      Serial.print("Two Point calibration: ");
      Serial.println(adc_chars->vref);
  } 
  else {
      Serial.print("Default calibration: ");
      Serial.println(adc_chars->vref);
  }

  delay(1000);

  // Configurazione dell'ADC
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);
}

void loop() {
  // Lettura singola dell'ADC
  uint32_t adc_reading = adc1_get_raw(ADC_CHANNEL);
  // Conversione della lettura in millivolt
  uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);

  Serial.print("Raw: ");
  Serial.print(adc_reading);
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  Serial.println(" mV");
  
  delay(1000);
}
