#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "ws2812.pio.h"

// Definição dos pinos
#define LED_RGB_RED_PIN 13
#define LED_RGB_GREEN_PIN 12
#define LED_RGB_BLUE_PIN 13
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define WS2812_PIN 7

// Definição do número máximo de LEDs
#define MAX_LEDS 25

// Variáveis globais
volatile int number = 0;  // Número exibido na matriz de LEDs

// Variáveis para controle dos LEDs WS2812
static int sm = 0;  // Máquina de estado do PIO
static PIO pio = pio0;  // Bloco PIO
static uint32_t grb[MAX_LEDS];  // Array para armazenar as cores dos LEDs

// Função para inicializar o LED RGB
void init_rgb_led() {
    gpio_init(LED_RGB_RED_PIN);
    gpio_init(LED_RGB_GREEN_PIN);
    gpio_init(LED_RGB_BLUE_PIN);
    gpio_set_dir(LED_RGB_RED_PIN, GPIO_OUT);
    gpio_set_dir(LED_RGB_GREEN_PIN, GPIO_OUT);
    gpio_set_dir(LED_RGB_BLUE_PIN, GPIO_OUT);
}

// Função para piscar o LED vermelho
void blink_red_led() {
    gpio_put(LED_RGB_RED_PIN, 1);
    sleep_ms(100);
    gpio_put(LED_RGB_RED_PIN, 0);
    sleep_ms(100);
}

// Função para inicializar os botões
void init_buttons() {
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);
}

// Função de interrupção direta
void button_isr(uint gpio, uint32_t events) {
    static uint32_t last_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time - last_time < 200) return; // Debouncing de 200ms
    last_time = current_time;

    number = (gpio == BUTTON_A_PIN) ? (number + 1) % 10 : (number - 1 + 10) % 10;

    printf("Número atualizado: %d\n", number);
    display_number(number);
}

// Função para converter porcentagens de cores RGB para o formato GRB
static void rgb_to_grb(uint32_t porcentColors[][3]) {
    for (int i = 0; i < MAX_LEDS; i++) {
        uint8_t r = porcentColors[i][0] ? 255 * (porcentColors[i][0] / 100.0) : 0;
        uint8_t g = porcentColors[i][1] ? 255 * (porcentColors[i][1] / 100.0) : 0;
        uint8_t b = porcentColors[i][2] ? 255 * (porcentColors[i][2] / 100.0) : 0;
        grb[i] = (g << 16) | (r << 8) | b;
    }
}

// Função para inicializar a matriz de LEDs WS2812
void init_ws2812() {
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
    for (int i = 0; i < MAX_LEDS; i++) {
        pio_sm_put_blocking(pio, sm, 0);
    }
}

// Função para exibir um número na matriz de LEDs WS2812
void display_number(int number) {
    const uint32_t numbers[10][25] = {
        {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0}, // 0
        {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0}, // 1
        {0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0}, // 2
        {0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0}, // 3
        {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0}, // 4
        {0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0}, // 5
        {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0}, // 6
        {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0}, // 7
        {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0}, // 8
        {0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0}  // 9
    };

    // Cores para cada número (R, G, B)
    const uint32_t colors[10][3] = {
        {100, 0, 0},   // Vermelho para 0
        {0, 100, 0},   // Verde para 1
        {0, 0, 100},   // Azul para 2
        {100, 100, 0}, // Amarelo para 3
        {100, 0, 100}, // Magenta para 4
        {0, 100, 100}, // Ciano para 5
        {100, 20, 0},  // Laranja para 6
        {20, 0, 100},  // Roxo para 7
        {100, 100, 100}, // Branco para 8
        {40, 40, 40}   // Cinza para 9
    };

    uint32_t porcentColors[MAX_LEDS][3] = {0};
    for (int i = 0; i < MAX_LEDS; i++) {
        if (numbers[number][i]) {
            porcentColors[i][0] = colors[number][0]; // Vermelho
            porcentColors[i][1] = colors[number][1]; // Verde
            porcentColors[i][2] = colors[number][2]; // Azul
        }
    }

    rgb_to_grb(porcentColors);
    for (int i = 0; i < MAX_LEDS; i++) {
        pio_sm_put_blocking(pio, sm, grb[i] << 8u);
    }
    sleep_us(10);
    return;
}

int main() {
    stdio_init_all();
    init_rgb_led();
    init_buttons();
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_isr);
    init_ws2812();

    
    while (1) {
        if(number == 0){
            display_number(number);
        }
        blink_red_led();
    }

    return 0;
}