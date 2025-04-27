# Ohmímetro com Reconhecimento Automático do Código de Cores de Resistores

Este sistema utiliza a placa BitDogLab (RP2040) para medir o valor de resistores e identificar automaticamente o código de cores associado, exibindo as informações em um display OLED.

---

## 🎯 Objetivo Geral

Desenvolver um ohmímetro digital capaz de:
- Realizar a medição de resistência elétrica usando ADC.
- Calcular o valor da resistência conectada.
- Identificar o valor comercial mais próximo na série E24 (5%).
- Determinar as cores correspondentes às faixas do resistor.
- Exibir o valor medido e as cores no display OLED SSD1306.

---

## ⚙️ Descrição Funcional

- **Medição da resistência**: utilizando um divisor de tensão e leitura analógica via ADC.
- **Identificação do valor comercial**: o valor medido é aproximado para a série E24.
- **Determinação das cores**: baseado no valor comercial, são atribuídas as cores de acordo com a norma de resistores de 4 faixas.
- **Exibição**: no display OLED são mostrados:
  - Valor real da resistência medida.
  - As cores correspondentes ao resistor.

---

## 🛠️ Periféricos Utilizados

- **ADC**: Leitura da tensão no divisor resistivo para calcular a resistência desconhecida.
- **I2C**: Comunicação com o display OLED SSD1306.
- **Display OLED SSD1306**: Exibição dos valores e cores.
- **Botões**: Um botão para função de reset em BOOTSEL.
- **GPIOs**: Configuradas para botões, SDA, SCL e entrada analógica.

---

## 🧩 Organização do Código

- **main.c**: Arquivo principal do sistema, contendo a lógica de medição, cálculo e exibição.
- **lib/ssd1306/**: Biblioteca para controle do display OLED.
- **CMakeLists.txt**: Arquivos de configuração para compilação utilizando o Pico SDK.

O código está organizado e comentado para facilitar o entendimento e possíveis manutenções futuras.

---

## 🚀 Como Executar

1. Conectar a placa BitDogLab via USB.
2. Compilar o projeto usando o Pico SDK (`cmake` + `make`).
3. Gravar o arquivo `.uf2` gerado na placa.
4. Conectar um resistor entre os terminais de medição.
5. Observar no display o valor medido e o código de cores correspondente.

---

## 📹 Demonstração

---

## 📂 Links Úteis

- [Documentação Pico SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Biblioteca SSD1306 usada no projeto](https://github.com/afiskon/stm32-ssd1306)

---

> Projeto desenvolvido por **Luiz Filipe Ribeiro de Jesus**  
> Polo: **Vitória da Conquista**  
> Data: **Abril de 2025**