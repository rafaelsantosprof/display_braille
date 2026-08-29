#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o vetor de LCDs no endereço 0x27 (padrão do Tinkercad) com 16 colunas e 2 linhas
/*
>> JV: Aqui, eu coloquei um vetor de LCDs, para facilitar a organização.
Uma página pode ter de 33 a 35 caracteres, pelo que eu lembro.
Aí para facilitar a divisão da string, coloquei um vetor lcd, onde
lcd[0] -> primeiro display [letras 0 a 15]
lcd[1] -> segundo display [letras 16 a 31]
lcd[2] -> terceiro display [letras 32 a 34]
Aqui, acho que a macro faz sentido por simplificação.
*/
#define QNT_LCD 3       // Quantidade de LCDs usados
#define TAM_LINHA 35    // Máximo de caracteres por linha
LiquidCrystal_I2C lcd[QNT_LCD] = {
  lcd(0x27, 16, 2),
  lcd(0x28, 16, 2),
  lcd(0x29, 16, 2)
}

// Pinos dos botões (Ponto 1 ao Ponto 6)
const int pinosBotoes[6] = {2, 3, 4, 5, 6, 7};
// Pinos dos botões de ação [Modificar pinagem]
#define pinoConfirma 10
#define pinoApaga 9
#define pinoEspaco 8

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
int linhaAtual = 1; // >> JV: Não sei se vai precisar da linha

// >> JV: Buffer
const MapeamentoBraille bufferLinha[TAM_LINHA] = {0}

void setup() {
  Serial.begin(9600);

  // >> JV: Implementar o loop For para inicializar os 3 displays.
  // for(int i in QNT_LCD); [...]
  /*
  // Inicializa o LCD I2C
  lcd.init();
  lcd.backlight();
  
  // Mensagem inicial na tela
  lcd.setCursor(0, 0);
  lcd.print("Teclado Braille:");
  
  // Posiciona o cursor na segunda linha onde a digitação vai aparecer
  lcd.setCursor(0, 1);
  */

  for (int i = 0; i < 6; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }
}

void loop() {
  byte estadoAtual = lerEstadoBotoes();

  /* >> JV:Aqui, colocar a lógica para processar os demais botões.
  if (digitalRead(pinoConfirma)) {}
  else if (digitalRead(pinoApaga)) {}
  else if (digitalRead(pinoEspaco)) {... podem colocar para adicionar algo no buffer}
  else if (estadoAtual) {... o que voces ja fizeram}
  */

  if (estadoAtual > 0) {
    byte combinacaoFinal = 0;

    // Aguarda soltar os botões acumulando o acorde
    while (lerEstadoBotoes() > 0) {
      combinacaoFinal |= lerEstadoBotoes();
      delay(10);
    }

    processarBraille(combinacaoFinal);
  }
}

byte lerEstadoBotoes() {
  byte mascara = 0;
  for (int i = 0; i < 6; i++) {
    if (digitalRead(pinosBotoes[i]) == LOW) {
      mascara |= (1 << i);
    }
  }
  return mascara;
}

void processarBraille(byte mascara) {
  char letraEncontrada = '?';
  bool encontrada = false;

  for (int i = 0; i < totalLetras; i++) {
    if (tabelaBraille[i].mascara == mascara) {
      // >> JV: Talvez aqui fique melhor passar passar "tabelaBraille[i]" em "letraEncontrada"
      // Porque aí vai facilitar o controle pelo buffer, que será encaminhado pro mecanismo.
      letraEncontrada = tabelaBraille[i].letra;
      encontrada = true;
      break;
    }
  }

  
  if (encontrada) {
    // Imprime no Monitor Serial
    Serial.print("Letra: ");
    Serial.println(letraEncontrada);

    /*
    >> JV: 
    chamar 'adicionarAoBuffer(letraEncontrada)'
    modificar 'escreverNoLCD' [deixei abaixo]
    */

    // Escreve o caractere no Display LCD
    escreverNoLCD(letraEncontrada);
  } else {
    Serial.print("Combinacao nao reconhecida: ");
    Serial.println(mascara, BIN);
  }
}

/* >> JV: acho que o início da função pode ser
void escreverNoLCD() {
  -> percorre o buffer..., cada posição sendo chamada de "c"
  int n = c / 16;
  lcd[n] -> daqui, é só usar as funções de escrita normalmente para cada um dos caracteres do buffer.
  ...
  Divisão de inteiros é truncada em C.
  Essa é uma forma rápida de dizer "os caracteres de 0 a 15 são no display 0", 
    pois qualquer valor de 0 a 15 dividido por 16 vai ser igual a 0
    qualquer valor de 16 a 31 divido por 16 vai ser igual a 1
    e o resto vai ser igual a 2
  ...
  Ou vcs cortam o char em 3 partes e pedem pra cada LCD escrever uma parte. Funciona tmb.
}
*/
void escreverNoLCD(char c) {
  // Se a linha de digitação encher (16 caracteres), limpa a linha e recomeça
  if (colunaAtual >= 16) {
    lcd.setCursor(0, 1);
    lcd.print("                "); // Limpa a linha 2
    colunaAtual = 0;
  }

  lcd.setCursor(colunaAtual, 1);
  lcd.print(c);
  colunaAtual++;
}

/* >> JV: Coloquem essas funções, pra facilitar
void adicionarAoBuffer(MapeamentoBraille simbolo){}
void limparBuffer(){} -> útil para confirmar
*/