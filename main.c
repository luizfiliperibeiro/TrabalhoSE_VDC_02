 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <string.h>
 #include "pico/stdlib.h"
 #include "hardware/adc.h"
 #include "hardware/i2c.h"
 #include "lib/ssd1306.h"
 #include "lib/font.h"
 #include "lib/ws2812.h"
 #define I2C_PORT i2c1
 #define I2C_SDA 14
 #define I2C_SCL 15
 #define endereco 0x3C
 #define ADC_PIN 28 // GPIO para o voltímetro
 #define Botao_A 5  // GPIO para botão A

 int R_conhecido = 10000;    // Resistor conhecido
 float R_x = 0.0;           // Resistor desconhecido
 float ADC_VREF = 3.31;     // Tensão de referência do ADC
 int ADC_RESOLUTION = 4095; // Resolução do ADC (12 bits)

 ws2812_t matriz_leds;      // Estrutura para a matriz WS2812
 
 // Trecho para modo BOOTSEL com botão B
 #include "pico/bootrom.h"
 #define botaoB 6
 void gpio_irq_handler(uint gpio, uint32_t events)
 {
   reset_usb_boot(0, 0);
 }

 float encontrar_valor_E24(float resistencia) {
  const float E24_series[] = {
      10, 11, 12, 13, 15, 16, 18, 20,
      22, 24, 27, 30, 33, 36, 39, 43,
      47, 51, 56, 62, 68, 75, 82, 91
  };

  // Se for muito próximo de um múltiplo de 100, 1000, etc., trata como tal
  int potencias[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
  for (int i = 0; i < 7; i++) {
      float valor = potencias[i];
      if (fabs(resistencia - valor) <= (valor * 0.05)) { // tolerância de 5%
          return valor;
      }
  }

  int multiplicador = 0;

  // Normaliza para entre 10 e 100
  while (resistencia >= 100) {
      resistencia /= 10;
      multiplicador++;
  }
  while (resistencia < 10) {
      resistencia *= 10;
      multiplicador--;
  }

  // Encontra o valor mais próximo na série E24
  float menor_diferenca = 1000.0;
  float valor_encontrado = 0.0;
  for (int i = 0; i < 24; i++) {
      float diferenca = fabs(resistencia - E24_series[i]);
      if (diferenca < menor_diferenca) {
          menor_diferenca = diferenca;
          valor_encontrado = E24_series[i];
      }
  }

  // Reconstrói o valor final com o multiplicador
  for (int i = 0; i < multiplicador; i++) {
      valor_encontrado *= 10;
  }
  for (int i = 0; i > multiplicador; i--) {
      valor_encontrado /= 10;
  }

  return valor_encontrado;
}

const char* cores[] = {
  "Preto", "Marrom", "Vermelho", "Laranja", "Amarelo",
  "Verde", "Azul", "Violeta", "Cinza", "Branco"
};

void determinar_cores(float resistencia, char *cor1, char *cor2, char *cor_multiplicador) {
  int expoente = 0;

  // Normaliza para dois dígitos (ex: 4700 -> 47)
  while (resistencia >= 100) {
      resistencia /= 10;
      expoente++;
  }

  int valor = (int)(resistencia + 0.5); // Arredonda
  int primeiro_digito = valor / 10;
  int segundo_digito = valor % 10;

  strcpy(cor1, cores[primeiro_digito]);
  strcpy(cor2, cores[segundo_digito]);
  strcpy(cor_multiplicador, cores[expoente]);
}

// Converte nome da cor para valores RGB
void cor_para_rgb(const char* cor, uint8_t *r, uint8_t *g, uint8_t *b) {
  if (strcmp(cor, "Preto") == 0) { *r = 0; *g = 0; *b = 0; }
  else if (strcmp(cor, "Marrom") == 0) { *r = 101; *g = 67; *b = 33; }
  else if (strcmp(cor, "Vermelho") == 0) { *r = 255; *g = 0; *b = 0; }
  else if (strcmp(cor, "Laranja") == 0) { *r = 255; *g = 165; *b = 0; }
  else if (strcmp(cor, "Amarelo") == 0) { *r = 255; *g = 255; *b = 0; }
  else if (strcmp(cor, "Verde") == 0) { *r = 0; *g = 255; *b = 0; }
  else if (strcmp(cor, "Azul") == 0) { *r = 0; *g = 0; *b = 255; }
  else if (strcmp(cor, "Violeta") == 0) { *r = 148; *g = 0; *b = 211; }
  else if (strcmp(cor, "Cinza") == 0) { *r = 128; *g = 128; *b = 128; }
  else if (strcmp(cor, "Branco") == 0) { *r = 255; *g = 255; *b = 255; }
}

// Atualiza a matriz com as cores do resistor
void desenhar_resistor(ws2812_t *matriz, const char* cor1, const char* cor2, const char* cor_multiplicador) {
    uint8_t r, g, b;
    ws2812_clear(matriz);

    // Primeira faixa (LEDs 0 a 7)
    cor_para_rgb(cor1, &r, &g, &b);
    for (int i = 0; i < 8; i++) {
        ws2812_set_pixel(matriz, i, r, g, b);
    }

    // Segunda faixa (LEDs 8 a 15)
    cor_para_rgb(cor2, &r, &g, &b);
    for (int i = 8; i < 16; i++) {
        ws2812_set_pixel(matriz, i, r, g, b);
    }

    // Terceira faixa (LEDs 16 a 24)
    cor_para_rgb(cor_multiplicador, &r, &g, &b);
    for (int i = 16; i < 25; i++) {
        ws2812_set_pixel(matriz, i, r, g, b);
    }

    ws2812_show(matriz);
}

 int main()
 {
   gpio_init(Botao_A);
   gpio_set_dir(Botao_A, GPIO_IN);
   gpio_pull_up(Botao_A);
 
   // I2C Initialisation. Using it at 400Khz.
   i2c_init(I2C_PORT, 400 * 1000);
 
   gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
   gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
   gpio_pull_up(I2C_SDA);                                        // Pull up the data line
   gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
   ssd1306_t ssd;                                                // Inicializa a estrutura do display
   ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
   ssd1306_config(&ssd);                                         // Configura o display
   ssd1306_send_data(&ssd);                                      // Envia os dados para o display
 
   // Limpa o display. O display inicia com todos os pixels apagados.
   ssd1306_fill(&ssd, false);
   ssd1306_send_data(&ssd);
 
   adc_init();
   adc_gpio_init(ADC_PIN); // GPIO 28 como entrada analógica

   ws2812_init(&matriz_leds, 7, 25); // GPIO 7, 25 LEDs (5x5)
 
   float tensao;
   char str_x[5]; // Buffer para armazenar a string
   char str_y[5]; // Buffer para armazenar a string
 
   bool cor = true;
   while (true)
   {
     adc_select_input(2); // Seleciona o ADC para eixo X. O pino 28 como entrada analógica
 
     float soma = 0.0f;
     for (int i = 0; i < 500; i++)
     {
       soma += adc_read();
       sleep_ms(1);
     }
     float media = soma / 500.0f;
 
       // Fórmula simplificada: R_x = R_conhecido * ADC_encontrado /(ADC_RESOLUTION - adc_encontrado)
       R_x = (R_conhecido * media) / (ADC_RESOLUTION - media);

       // Cálculo do valor comercial para as cores
       float R_x_comercial = encontrar_valor_E24(R_x);

       char cor1[10];
       char cor2[10];
       char cor_multiplicador[10];
       determinar_cores(R_x_comercial, cor1, cor2, cor_multiplicador);

     sprintf(str_x, "%1.0f", media); // Converte o inteiro em string
     sprintf(str_y, "%1.0f", R_x);   // Converte o float em string
 
     // cor = !cor;
     //  Atualiza o conteúdo do display com animações
     ssd1306_fill(&ssd, !cor);                          // Limpa o display
     ssd1306_draw_string(&ssd, "Cores:", 10, 10);
     ssd1306_draw_string(&ssd, cor1, 55, 10);
     ssd1306_draw_string(&ssd, cor2, 55, 20);
     ssd1306_draw_string(&ssd, cor_multiplicador, 55, 30);
     ssd1306_draw_string(&ssd, "ADC", 13, 41);          // Desenha uma string
     ssd1306_draw_string(&ssd, "Resisten.", 50, 41);    // Desenha uma string
     ssd1306_line(&ssd, 44, 37, 44, 60, cor);           // Desenha uma linha vertical
     ssd1306_draw_string(&ssd, str_x, 8, 52);           // Desenha uma string
     ssd1306_draw_string(&ssd, str_y, 59, 52);          // Desenha uma string
     ssd1306_send_data(&ssd);                           // Atualiza o display

     // Atualiza a matriz de LEDs
     desenhar_resistor(&matriz_leds, cor1, cor2, cor_multiplicador);
     
     sleep_ms(700);
   }
 }