#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o LCD no endereço 0x27 (padrão do Tinkercad) com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pinos dos botões (Ponto 1 ao Ponto 6)
const int pinosBotoes[6] = {2, 3, 4, 5, 6, 7};

struct MapeamentoBraille {
  byte mascara;
  char letra;
};

// Tabela Braille (P1=bit0, P2=bit1, P3=bit2, P4=bit3, P5=bit4, P6=bit5)
const MapeamentoBraille tabelaBraille[] = {
  {0b000001, 'A'}, {0b000011, 'B'}, {0b001001, 'C'}, {0b011001, 'D'},
  {0b010001, 'E'}, {0b001011, 'F'}, {0b011011, 'G'}, {0b010011, 'H'},
  {0b001010, 'I'}, {0b011010, 'J'}, {0b000101, 'K'}, {0b000111, 'L'},
  {0b001101, 'M'}, {0b011101, 'N'}, {0b010101, 'O'}, {0b001111, 'P'},
  {0b011111, 'Q'}, {0b010111, 'R'}, {0b001110, 'S'}, {0b011110, 'T'},
  {0b100101, 'U'}, {0b100111, 'V'}, {0b011010, 'W'}, {0b101101, 'X'},
  {0b111101, 'Y'}, {0b110101, 'Z'}
};

const int totalLetras = sizeof(tabelaBraille) / sizeof(tabelaBraille[0]);

// Variáveis de controle da tela
int colunaAtual = 0;
int linhaAtual = 1;