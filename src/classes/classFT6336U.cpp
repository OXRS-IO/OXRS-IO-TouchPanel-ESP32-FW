
#include <classFT6336U.h>
#include <Wire.h>

classFT6336U::classFT6336U(int8_t sda, int8_t scl, uint8_t int_n)
{
  _sdaPin = sda;
  _sclPin = scl;
  _intPin = int_n;
}

void classFT6336U::begin(void)
{
  // start I2C
  Wire.begin(_sdaPin, _sclPin);

  // Int/Polling pin Configuration
  pinMode(_intPin, INPUT);
}

bool classFT6336U::getTouched(void)
{
  return (!digitalRead(_intPin));
}

bool classFT6336U::readTouchPoint(typePoint *ts)
{
  const uint8_t size = 5;
  uint8_t data[size];
  Wire.beginTransmission(I2C_ADDR_FT6336U);
  Wire.write(FT6336U_ADDR_TD_STATUS);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)I2C_ADDR_FT6336U, size);
  for (uint8_t i = 0; i < size; i++)
    data[i] = Wire.read();

  // touched coordinates
  uint16_t tmp;
  tmp = data[1] & 0x0f;
  ts->x = (tmp << 8) + data[2];
  tmp = data[3] & 0x0f;
  ts->y = (tmp << 8) + data[4];

  // number of touches -> > 0 = touch detected
  if (data[0] > 0)
    return true;
  else
    return false;
}
