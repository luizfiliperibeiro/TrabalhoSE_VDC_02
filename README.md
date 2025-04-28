# Ohmímetro com Reconhecimento Automático do Código de Cores de Resistores

Este sistema utiliza a placa BitDogLab (RP2040) para medir o valor de resistores e identificar automaticamente o valor comercial mais próximo da série E24. As informações são exibidas em um display OLED e também de forma gráfica por meio de uma matriz de LEDs WS2812.

---

## 🎯 Objetivo Geral

Desenvolver um ohmímetro digital capaz de:
- Realizar a medição de resistência elétrica usando ADC.
- Calcular o valor da resistência conectada através de um divisor de tensão.
- Identificar o valor comercial mais próximo (série E24, 5% de tolerância).
- Determinar as cores correspondentes às faixas do resistor.
- Exibir o valor real da resistência medida e o código de cores no display OLED SSD1306.
- Representar graficamente o código de cores utilizando uma matriz de LEDs 5x5 WS2812.

---

## ⚙️ Descrição Funcional

O sistema funciona através da leitura analógica da tensão em um divisor resistivo, composto por um resistor de referência de 10kΩ e o resistor desconhecido.

Funcionalidades principais:
- **Medição precisa da resistência** via ADC do RP2040.
- **Aproximação para a série E24** para identificação comercial do resistor.
- **Determinação automática das faixas de cores** (1ª, 2ª e multiplicador).
- **Exibição no display OLED**:
  - Valor real da resistência medida.
  - Código de cores (nomes das cores).
- **Exibição gráfica na matriz de LEDs WS2812**:
  - Três grupos verticais de LEDs acesos com as cores correspondentes às faixas do resistor.

O sistema atualiza a leitura e a representação a cada 700 ms, permitindo medição contínua e prática de resistores variados.

---

## 🛠️ Periféricos Utilizados

- **ADC (Conversor Analógico-Digital)**: para ler a tensão no divisor resistivo.
- **I2C (Interface de Comunicação Serial)**: para controlar o display OLED SSD1306.
- **PIO (Programmable Input/Output)**: para controlar a matriz de LEDs WS2812.
- **GPIOs**:
  - Entrada analógica no pino 28 (ADC0).
  - Comunicação I2C nos pinos 14 (SDA) e 15 (SCL).
  - Controle da matriz de LEDs no pino 7 (WS2812).
  - Botão de reset/BOOTSEL no pino 6.

---

## 🧩 Organização do Código

- **main.c**: Arquivo principal com toda a lógica de medição, cálculo e exibição.
- **lib/ssd1306/**: Biblioteca para o controle do display OLED.
- **lib/ws2812/**: Biblioteca para controle da matriz de LEDs WS2812 utilizando PIO.
- **lib/ws2812.pio**: Arquivo de programa PIO para comunicação WS2812.

O projeto é modularizado, facilitando a manutenção e a extensão futura.

---

## 🚀 Como Executar

1. Conectar a placa BitDogLab via USB.
2. Compilar o projeto utilizando o CMake e o Pico SDK.
3. Gravar o arquivo `.uf2` gerado na placa.
4. Conectar o resistor a ser medido no circuito de entrada.
5. Visualizar:
   - No display OLED: o valor da resistência e as cores calculadas.
   - Na matriz de LEDs: a representação gráfica do resistor com as cores das faixas.

---

## 📹 Demonstração

[Vídeo de Demonstração](https://drive.google.com/file/d/10dzwdQU0tOD7F-sywhAbXlK5gJWKHhZF/view?usp=sharing)

---

## 📂 Links Úteis

- [Documentação Pico SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Documentação WS2812](https://cdn-shop.adafruit.com/datasheets/WS2812.pdf)

---

> Projeto desenvolvido por **Luiz Filipe Ribeiro de Jesus**  
> Polo: **Vitória da Conquista**  
> Data: **Abril de 2025**