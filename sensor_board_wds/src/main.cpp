#include <Arduino.h>
#include "Wire.h"
#include <stdlib.h>


#define POLYNOMIAL_8 0x2F // wielomian 8 bitowy (używany w przemyśle motoryzacyjnym)
const int MPU_ADDR = 0x68; // adres I2C dla MPU-6050

typedef struct data{
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    int16_t temp;
    uint16_t adcValue[5];
};

char str[128];
data sensors;
int sum_x, sum_y, sum_z; 
int loop_counter=0, sum_counter=0;
uint8_t str_len=0;
byte arr[128];
byte sum_crc;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;

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

void read_imu_data()
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); 
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 7*2, true);
    sensors.acc_x = Wire.read()<<8 | Wire.read();
    sensors.acc_y = Wire.read()<<8 | Wire.read();
    sensors.acc_z = Wire.read()<<8 | Wire.read();
    sensors.temp = Wire.read()<<8 | Wire.read();

    sensors.gyro_x = Wire.read()<<8 | Wire.read();
    sensors.gyro_y = Wire.read()<<8 | Wire.read();
    sensors.gyro_z = Wire.read()<<8 | Wire.read();
}
// konfiguracja I2C oraz UART
void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);


  for (int cal_int = 0; cal_int < 1000 ; cal_int ++){                  
    read_imu_data(); 
    //Add the gyro x offset to the gyro_x_cal variable                                            
    gyro_x_cal += sensors.gyro_x;
    //Add the gyro y offset to the gyro_y_cal variable                                              
    gyro_y_cal += sensors.gyro_y; 
    //Add the gyro z offset to the gyro_z_cal variable                                             
    gyro_z_cal += sensors.gyro_z; 
    //Delay 3us to have 250Hz for-loop                                             
    delay(3);                                                          
  }
  gyro_x_cal /= 1000;                                                 
  gyro_y_cal /= 1000;                                                 
  gyro_z_cal /= 1000;

}

void loop()
{

  if(loop_counter%6==0)
  {
    read_imu_data();
    sensors.gyro_x -= gyro_x_cal;                                                 
    sensors.gyro_y -= gyro_y_cal;                                                 
    sensors.gyro_z -= gyro_z_cal;

    sensors.gyro_x /= 131;                                                 
    sensors.gyro_y /= 131;                                                 
    sensors.gyro_z /= 131;

    sum_x += sensors.gyro_x;
    sum_y += sensors.gyro_y;
    sum_z += sensors.gyro_z;
  }
  
  if(loop_counter%18==0)
  {
    read_adc();

    sensors.gyro_x = sum_x;
    sensors.gyro_y = sum_y;
    sensors.gyro_z = sum_z;
    sensors.gyro_x *= 0.064;
    sensors.gyro_y *= 0.064;
    sensors.gyro_z *= 0.064;

    if(sensors.gyro_x>360 || sensors.gyro_x<-360){
      sensors.gyro_x=0;
      sum_x=0;
    }
    if(sensors.gyro_y>360 || sensors.gyro_y<(-360)){
      sensors.gyro_y=0;
      sum_y=0;
    }
    if(sensors.gyro_z>360 || sensors.gyro_z<-360){
      sensors.gyro_z=0;
      sum_z=0;
    }

    // Zapisywanie wszystkich danych do jednego ciagu znaków
    sprintf(str, "%4d%4d%4d%4d%4d%4d%4d%4d", sensors.adcValue[0], sensors.adcValue[1], sensors.adcValue[2], sensors.adcValue[3], sensors.adcValue[4], sensors.gyro_x, sensors.gyro_y, sensors.gyro_z);
    
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
  delay(10);
  
}