// Anibal Maldonado: Código principal  - https://github.com/PhD-Anibal
// Anibal Maldonado: LED Vermelho ao apertar 'A' - https://github.com/PhD-Anibal

#include <stdio.h>
#include "pico/stdlib.h"
#include <hardware/pio.h>           // Biblioteca para manipulação de periféricos PIO
#include "hardware/clocks.h"        // Biblioteca para controle de relógios do hardware
#include "hardware/gpio.h"
#include "Interrupto.pio.h" // Biblioteca PIO para controle de LEDs WS2818B
#include "numeros.h"

const uint led_pin_red = 13; // porta do pino 13 LED RGB Vermelho
const uint button_a = 5;
const uint button_b = 6;
static volatile uint a = 1 ;
static void gpio_irq_handler(uint gpio,uint32_t events);

//para o state machine
PIO pio = pio0; 
uint sm;
bool ok;
uint offset;


// Definições de constantes
#define LED_COUNT 25                // Número de LEDs na matriz
#define LED_PIN 7                   // Pino GPIO conectado aos LEDs
#define FPS 10  // frames por segundo, indica que o LED vai piscar 10 vezes por segundo
const int interval=1000/FPS; //intervalo de 100 ms

// Anibal Maldonado - Índices da minha matriz de desenho na ordem desejada
int ordem[] = {0,1, 2, 3, 4,9 , 8, 7,6,5, 10,11,12,13,14,19,18,17,16,15,20,21,22,23,24}; 
int direcao=0;


//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//imprimir valor binário
void imprimir_binario(int num) {
 int i;
 for (i = 31; i >= 0; i--) {
  (num & (1 << i)) ? printf("1") : printf("0");
 }
}

void desenho_pio(int desenho[][25], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b, int k) {
    //for (int16_t k = 1; k < 10; k++) {
        for (int16_t i = 0; i < LED_COUNT; i++) {
                switch (desenho[k][ordem[24-i]]) {
                    case 0: 
                        valor_led = matrix_rgb(b=0.0, r=0.0, g=0.0);
                        break;
                    case 1: 
                        valor_led = matrix_rgb(b=1, r=0.0, g=0.0);
                        break;
                    case 2:
                        valor_led = matrix_rgb(b=0.0, r=1, g=0.0);
                        break;
                    case 3:
                        valor_led = matrix_rgb(b=0.0, r=0.0, g=1);
                        break;
                    case 4:
                        valor_led = matrix_rgb(b=0.2, r=1, g=0.9);
                        break;
                }

                pio_sm_put_blocking(pio, sm, valor_led);

        }
        imprimir_binario(valor_led);
}

void gpio_irq_handler(uint gpio,uint32_t events)
{
    
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;
    
   //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    //ok = set_sys_clock_khz(128000, false);

    //if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

    direcao=(gpio==button_a)?((direcao<9)?direcao+1:9):((direcao>0)?direcao-1:0);
    desenho_pio(desenho, valor_led, pio, sm, r, g, b,direcao);
 
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial

    gpio_init(led_pin_red); // Inicializa o pino do LED vermelho
    gpio_set_dir(led_pin_red, GPIO_OUT); // Configura como saída
    gpio_init(button_a); // Inicializa o pino do LED vermelho
    gpio_set_dir(button_a, GPIO_IN); // Configura como saída
    gpio_init(button_b); // Inicializa o pino do LED vermelho
    gpio_set_dir(button_b, GPIO_IN); // Configura como saída
    gpio_pull_up(button_a);
    gpio_pull_up(button_b);

    //configurações da PIO
    offset=offset = pio_add_program(pio, &interrupto_program);
    sm = pio_claim_unused_sm(pio, true);
    interrupto_program_init(pio, sm, offset, LED_PIN);

    //stdio_init_all(); // Inicializa a comunicação serial (opcional)
    gpio_set_irq_enabled_with_callback(button_a,GPIO_IRQ_EDGE_FALL, true,&gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_b,GPIO_IRQ_EDGE_FALL, true,&gpio_irq_handler);
    
    
    
    while (true) {
        //em 1 segundo completa 10 ciclos de acender e apagar
        gpio_put(led_pin_red, true); // Acende o LED vermelho
        sleep_ms(interval/2);             // Mantém aceso por um intervalo de 50 milisegundos
        gpio_put(led_pin_red, false); // Apaga o LED vermelho
        sleep_ms(interval/2);             // Mantém apagado por um intervalo de 50 milisegundos
    }

    return 0;
}
