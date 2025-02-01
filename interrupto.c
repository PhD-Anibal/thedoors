// Anibal Maldonado: - https://github.com/PhD-Anibal
#include <stdio.h>
#include "pico/stdlib.h"
#include <hardware/pio.h>           // Biblioteca para controle dos periféricos PIO
#include "hardware/clocks.h"        // Biblioteca para gerenciamento dos relógios do hardware
#include "hardware/gpio.h"
#include "Interrupto.pio.h" // Biblioteca PIO para manipulação de LEDs WS2818B
#include "numeros.h"        
#include "hardware/timer.h" // Biblioteca para controle de temporizadores de hardware

// Definições de constantes
#define LED_COUNT 25                // Número total de LEDs na matriz
#define LED_PIN 7                   // Pino GPIO utilizado para os LEDs
#define DEBOUNCE_DELAY 100  // Tempo de debounce para evitar múltiplas detecções de um único toque
volatile uint32_t last_interrupt_time=0; // Armazena o tempo da última interrupção

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
bool led_on = false;

// Função de callback para o temporizador de hardware
// Alterna o estado do LED vermelho a cada chamada
bool repeating_timer_callback(struct repeating_timer *t) {
    led_on = !led_on;
    gpio_put(led_pin_red,led_on);
    return true; // Mantém o temporizador ativo
}

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
        direcao=(gpio==button_a)?((direcao<9)?direcao+1:9):((direcao>0)?direcao-1:0);
        desenho_pio(desenho, valor_led, pio, sm, r, g, b,direcao);    
    }
}

void config() // Inicializa o serial, configura GPIOs, ativa PIO para os LEDs e define interrupções nos botões.
{
    stdio_init_all(); // Inicializa a comunicação serial

    // Configuração do LED vermelho
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);
    
    // Configuração dos botões
    gpio_init(button_a);
    gpio_set_dir(button_a, GPIO_IN);
    gpio_init(button_b);
    gpio_set_dir(button_b, GPIO_IN);
    gpio_pull_up(button_a);
    gpio_pull_up(button_b);

    // Configuração do periférico PIO
    offset= pio_add_program(pio, &interrupto_program);
    sm = pio_claim_unused_sm(pio, true);
    interrupto_program_init(pio, sm, offset, LED_PIN);
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
        
    struct repeating_timer timer;
    // Configura um temporizador para alternar o LED vermelho a cada 0.1 segundo
    add_repeating_timer_ms(100, repeating_timer_callback, NULL, &timer);

    while (true) {
        // Loop infinito mantendo o programa em execução
    }
    
    return 0;
}
