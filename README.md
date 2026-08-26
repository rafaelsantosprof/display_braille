# ⌨️ Teclado Braille Arduino — Impressora LASI

Sistema de entrada com 6 botões (cela Braille) com decodificação por máscara de bits e exibição em LCD I2C (16x2). Projeto desenvolvido pela **LASI**.

---

## 🔌 Conexões (Pinout)

| Componente | Arduino Uno | Modo / Protocolo |
| :--- | :---: | :--- |
| Ponto 1 | D2 | `INPUT_PULLUP` (GND) |
| **Ponto 2** | **D3** | `INPUT_PULLUP` (GND) |
| **Ponto 3** | **D4** | `INPUT_PULLUP` (GND) |
| **Ponto 4** | **D5** | `INPUT_PULLUP` (GND) |
| **Ponto 5** | **D6** | `INPUT_PULLUP` (GND) |
| **Ponto 6** | **D7** | `INPUT_PULLUP` (GND) |
| **LCD SDA** | **A4** | Barramento I2C |
| **LCD SCL** | **A5** | Barramento I2C |

---
*Como Testar*

x Carregue o código no Arduino Uno ou simule no Tinkercad.

x Certifique-se de instalar a biblioteca LiquidCrystal_I2C.

RESUMO

Detecção de Gatilho: O ciclo loop monitora as entradas. Quando qualquer botão é pressionado, a rotina de captura é iniciada.

Acumulação de Acorde e Debounce: Como os dedos humanos não fecham os contatos elétricos no exato mesmo microssegundo, um laço while mantém a leitura aberta. Ele acumula as entradas acionadas usando o operador OU binário (|=) com pausas de 10 ms até que todos os botões sejam soltos.

Busca Indexada (Lookup Table): O valor binário final é comparado com uma tabela contendo os 26 padrões do alfabeto Braille armazenados como literais binários (por exemplo, 0b000001 equivale à letra A).

Renderização e Gestão de Tela: Se a combinação existir no dicionário, o caractere correspondente é impresso no LCD. O código atualiza o índice da coluna e limpa a linha automaticamente assim que atinge o limite de 16 caracteres.
