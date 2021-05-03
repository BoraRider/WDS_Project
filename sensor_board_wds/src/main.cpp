#include <Arduino.h>
#include "Wire.h"
#include <stdlib.h>

#define POLYNOMIAL_8 0x2F // wielomian 8 bitowy (używany w przemyśle motoryzacyjnym)
const int MPU_ADDR = 0x68; // adres I2C dla MPU-6050

typedef struct data{
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    uint16_t adcValue[5];
};

char str[128];
data sensors;
int sum_x, sum_y, sum_z; 
int loop_counter=0, sum_counter=0;
uint8_t str_len=0;
byte arr[128];
byte sum_crc;

// tworzenie sumy kontrolnej
byte CRC8_SingleByte(byte CRC_prev, byte Data)
{
  CRC_prev ^= Data;
  for(byte Idx=0; Idx<8; ++Idx)
  {
    if((CRC_prev & 0x80) != 0)
    {
      CRC_prev = (byte)((CRC_prev << 1) ^ POLYNOMIAL_8);
    }
    else
    {
      CRC_prev = (byte)(CRC_prev << 1);
    }
  }
  return CRC_prev;
}

byte CRC8_DataArray(byte *pData, byte Len)
{
  byte CRC_final = 0xff;
  for(byte Idx=0; Idx < Len; ++Idx)
  {
    CRC_final = CRC8_SingleByte(CRC_final, pData[Idx]);
  }
  return CRC_final;
}

// source: https://www.elektroda.pl/rtvforum/topic1768322.html#8510622
/*
byte create_crc(byte *data, uint8_t n){
    byte suma=0;
    for(int i = 0; i<n; i++){
        suma += (data[i] & POLYNOMIAL_8);
    }
    return ~suma;
}
*/
// konwersja tablicy char do tablicy unsigned int 8-bit (byte)
void string2ByteArray(char* input, byte* output)
{
    uint8_t loop = 0;
    uint8_t i = 0;
    
    while(input[loop] != '\0')
    {
        output[i++] = input[loop++];
    }
}

// czytanie wartości z przetwornika ADC
void read_adc()
{
  sensors.adcValue[0] = analogRead(A1);
  sensors.adcValue[1] = analogRead(A2);
  sensors.adcValue[2] = analogRead(A3);
  sensors.adcValue[3] = analogRead(A6);
  sensors.adcValue[4] = analogRead(A7);
}

// konfiguracja I2C oraz UART
void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

}
void loop(){

  read_adc();

  // tworzenie sumy 3 pomiarów
  if(loop_counter%6==0)
  {  
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); 
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 7*2, true);
    sum_x += Wire.read()<<8 | Wire.read();
    sum_y += Wire.read()<<8 | Wire.read();
    sum_z += Wire.read()<<8 | Wire.read();
    sum_counter++;
  }
  
  // wyznaczanie średniej z 3 pomiarów
  if(sum_counter == 2)
  {
    sum_counter = 0;
    sensors.gyro_x = sum_x / 3;
    sensors.gyro_y = sum_y / 3;
    sensors.gyro_z = sum_z / 3;
    sum_x = 0;
    sum_y = 0;
    sum_z = 0;
  }
  
  
  
  if(loop_counter%18==0)
  {
    // Zapisywanie wszystkich danych do jednego ciagu znaków
    sprintf(str, "%4d%4d%4d%4d%4d%6d%6d%6d", sensors.adcValue[0], sensors.adcValue[1], sensors.adcValue[2], sensors.adcValue[3], sensors.adcValue[4], sensors.gyro_x, sensors.gyro_y, sensors.gyro_z);
    
    // zamiana ciągu znaków na ciąg bajtów
    string2ByteArray(str, arr);
    str_len = strlen(str);
    // liczenie sumy kontrolnej
    sum_crc = CRC8_DataArray(arr, str_len);

    // wyświetlanie wartości
    Serial.print(str);
    Serial.print("\t");
    Serial.println(sum_crc);
  }

  loop_counter++;
  delay(1);
  
}
