#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "pio_matrix.pio.h"
#include "init_GPIO.h" // Biblioteca de inicialização dos botões
#include "frames.h" // Biblioteca contendo os frames dos números
#include "config_leds.h" // Biblioteca para configuração dos LEDs

// Definições dos pinos
#define BTN_A 5
#define BTN_B 6
#define LED_RED 13

// Debounce e tempo de pisca do LED RGB
#define DEBOUNCE_TIME 150  // 150ms para debounce
#define LED_BLINK_TIME 200 // LED pisca 5x por segundo (200ms ligado / 200ms desligado)

// Variáveis globais
volatile int pos = 0;
volatile bool botao_a_pressionado = false;
volatile bool botao_b_pressionado = false;
volatile bool led_estado = false;

// Lista dos frames dos números 0-9
double* frames[] = {Frame_0, Frame_1, Frame_2, Frame_3, Frame_4,
                    Frame_5, Frame_6, Frame_7, Frame_8, Frame_9};

// Função de interrupção para os botões
void irq_botao(uint gpio, uint32_t events) {
    if (gpio == BTN_A && !botao_a_pressionado) {
        botao_a_pressionado = true;
        pos = (pos + 1) % 10;  // Incrementa o número (volta para 0 após 9)
    } else if (gpio == BTN_B && !botao_b_pressionado) {
        botao_b_pressionado = true;
        pos = (pos == 0) ? 9 : pos - 1;  // Decrementa (não deixa negativo)
    }
}

// Timer para gerenciar debounce dos botões
bool resetar_botoes(struct repeating_timer *t) {
    botao_a_pressionado = false;
    botao_b_pressionado = false;
    return true;
}

// Timer para piscar o LED vermelho
bool piscar_led(struct repeating_timer *t) {
    led_estado = !led_estado; // Alterna o estado do LED
    gpio_put(LED_RED, led_estado);
    return true;
}

// Função principal
int main() {
    stdio_init_all();
    init_GPIO();

    // Configuração dos pinos
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    // Configuração dos botões com interrupção
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &irq_botao);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &irq_botao);

    // Configuração dos timers (correção na função add_repeating_timer_ms)
    struct repeating_timer debounce_timer;
    struct repeating_timer led_timer;
    add_repeating_timer_ms(DEBOUNCE_TIME, resetar_botoes, NULL, &debounce_timer);
    add_repeating_timer_ms(LED_BLINK_TIME, piscar_led, NULL, &led_timer);

    // Configuração do PIO
    PIO pio = pio0;
    uint32_t valor_led = 0;
    double r = 0.0, g = 0.0, b = 0.0;
    set_sys_clock_khz(128000, false); // Configura clock para 128 MHz

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Inicializa matriz de LEDs com o primeiro número
    setar_leds_azul(frames[pos], valor_led, pio, sm, r, g, b);

    while (true) {
        // Atualiza a matriz de LEDs se houver mudança
        setar_leds_azul(frames[pos], valor_led, pio, sm, r, g, b);

        // Exibe a frequência do clock no terminal serial
        printf("\nFrequência do clock: %ld Hz\r\n", clock_get_hz(clk_sys));

        sleep_ms(100); // Pequeno delay para otimizar o loop
    }
}
