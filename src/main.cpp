#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o LCD no endereço 0x27 (padrão do Tinkercad) com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);