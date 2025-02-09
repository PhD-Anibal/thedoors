// Anibal Maldonado: - https://github.com/PhD-Anibal
#include <stdio.h>
#include "pico/stdlib.h"
#include <hardware/pio.h>           // Biblioteca para controle dos periféricos PIO
#include "hardware/clocks.h"        // Biblioteca para gerenciamento dos relógios do hardware
#include "hardware/gpio.h"
#include "doors.pio.h" // Biblioteca PIO para manipulação de LEDs WS2818B
#include "numeros.h"        
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include <ctype.h> // Biblioteca necessária para isdigit() e converter em número

// Definições para o Display ssd1306
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
ssd1306_t ssd; // Inicializa a estrutura do display
bool cor = true;

// Definições de constantes
#define LED_COUNT 25                // Número total de LEDs na matriz
#define LED_PIN 7                   // Pino GPIO utilizado para os LEDs
#define DEBOUNCE_DELAY 150  // Tempo de debounce para evitar múltiplas detecções de um único toque
volatile uint32_t last_interrupt_time=0; // Armazena o tempo da última interrupção

const uint led_pin_green = 11; // Pino do LED RGB (cor verde)
const uint led_pin_blue = 12; // Pino do LED RGB (cor azul)
const uint led_pin_red = 13; // Pino do LED RGB (cor vermelha)
const uint button_a = 5; // Pino do botão A
const uint button_b = 6; // Pino do botão B
static volatile uint a = 1 ; // Variável de controle
static void gpio_irq_handler(uint gpio,uint32_t events);

// Variáveis para o state machine
PIO pio = pio0; 
uint sm;
bool ok;
uint offset;

// Anibal Maldonado - Índices organizados da matriz para desenho na ordem desejada
int ordem[] = {0,1, 2, 3, 4,9 , 8, 7,6,5, 10,11,12,13,14,19,18,17,16,15,20,21,22,23,24}; 
int direcao=0;

// Variável para controle do estado do LED
bool led_blue_on = false;
bool led_green_on = false;

// Função para definir a intensidade das cores do LED na matriz
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

// Função para desenhar um padrão de LED na matriz usando PIO
void desenho_pio(int desenho[][25], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b, int k) {
        for (int16_t i = 0; i < LED_COUNT; i++) {
                switch (desenho[k][ordem[24-i]]) {
                    case 0: 
                        valor_led = matrix_rgb(b=0.0, r=0.0, g=0.0); // LED apagado
                        break;
                    case 1: 
                        valor_led = matrix_rgb(b=0.2, r=0.0, g=0.0); // LED azul aceso não muito intenso
                        break;
                }
                pio_sm_put_blocking(pio, sm, valor_led);
        }
}

// Função de interrupção para tratar eventos nos botões
void gpio_irq_handler(uint gpio,uint32_t events)
{
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;

    uint32_t current_time=to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual em milissegundos
    if(current_time-last_interrupt_time>DEBOUNCE_DELAY){
        last_interrupt_time=current_time; // Atualiza o tempo da última interrupção
        cor = !cor;
        if(gpio==button_a){
            led_green_on = !led_green_on;
            gpio_put(led_pin_green,led_green_on);
            printf(led_green_on ? "Acabou de ligar o led verde\n" : "Acabou de desligar o led verde\n");
            
            ssd1306_fill(&ssd, !cor); // Limpa o display
            ssd1306_draw_string(&ssd, "Apertou o Botao A", 5, 10); // Desenha uma string      
            ssd1306_send_data(&ssd); // Atualiza o display          
        }
        if(gpio==button_b){
            led_blue_on = !led_blue_on;
            gpio_put(led_pin_blue,led_blue_on);
            printf(led_blue_on ? "Acabou de ligar o led azul\n" : "Acabou de desligar o led azul\n");
            
            ssd1306_fill(&ssd, !cor); // Limpa o display
            ssd1306_draw_string(&ssd, "Apertou o Botao B", 5, 10); // Desenha uma string      
            ssd1306_send_data(&ssd); // Atualiza o display
            //sleep_ms(500);            
        }
        printf((led_green_on && led_blue_on) ? "e estão os dois LEDs ligados\n" : "");
        printf((!led_green_on && !led_blue_on) ? "e estão os dois LEDs desligados\n" : "");
    }
}

void config() // Inicializa o serial, configura GPIOs, ativa PIO para os LEDs e define interrupções nos botões.
{
    stdio_init_all(); // Inicializa a comunicação serial
    //I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    // Configuração dos LEDs RGB
    gpio_init(led_pin_green);
    gpio_set_dir(led_pin_green, GPIO_OUT);
    gpio_init(led_pin_blue);
    gpio_set_dir(led_pin_blue, GPIO_OUT);
    //gpio_init(led_pin_red);    // não pede no projeto ativar ele pois não se usa
    //gpio_set_dir(led_pin_red, GPIO_OUT);
    
    // Configuração dos botões
    gpio_init(button_a);
    gpio_set_dir(button_a, GPIO_IN);
    gpio_init(button_b);
    gpio_set_dir(button_b, GPIO_IN);
    gpio_pull_up(button_a);
    gpio_pull_up(button_b);

    // Configuração do periférico PIO
    offset= pio_add_program(pio, &doors_program);
    sm = pio_claim_unused_sm(pio, true);
    doors_program_init(pio, sm, offset, LED_PIN);

    // Configuração da porta i2c para o Display
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

int main() {
    
    config(); // função de configuração

    // Apaga todos os LEDs da matriz ao iniciar
    uint32_t valor_led = matrix_rgb(0.0, 0.0, 0.0);
    for (int i = 0; i < LED_COUNT; i++) {
        pio_sm_put_blocking(pio, sm, valor_led);
    }

    // Configuração das interrupções nos botões
    gpio_set_irq_enabled_with_callback(button_a,GPIO_IRQ_EDGE_FALL, true,&gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_b,GPIO_IRQ_EDGE_FALL, true,&gpio_irq_handler);

    while (true) {  // Loop infinito mantendo o programa em execução
        if(stdio_usb_connected()){
            char c;
            if(scanf("%c",&c)==1){
                ssd1306_fill(&ssd, !cor); // Limpa o display

                char str[2] = {c, '\0'};  // Cria uma string de 2 caracteres (o char + terminador null)
                if (isdigit(c)) {  // Verifica se o caractere é um número (0-9)
                    int numero = c - '0';  // Converte para o inteiro 5
                    desenho_pio(desenho, valor_led, pio, sm, .1, .1, .1, numero);
                }
                ssd1306_draw_string(&ssd, str, 5, 10); // Desenha uma string      
                ssd1306_send_data(&ssd); // Atualiza o display                
            }
        }
        sleep_ms(10);
    }
    
    return 0;
}
