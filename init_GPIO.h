#ifndef INIT_GPIO_H  
#define INIT_GPIO_H
#include "pico/stdlib.h"

#define OUT_PIN 7
#define BTN_A 5
#define BTN_B 6
#define JSTICK 22

void init_GPIO(){
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);
}

#endif