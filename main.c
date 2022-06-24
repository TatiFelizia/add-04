#include <stdio.h>
#include "pico/stdlib.h"


/* Elijo un pin de Trigger y Echo */
const uint TRIGGER = 27;
const uint ECHO = 26;

const uint LED1 = 28;
const uint LED2= 22;
const uint LED3 = 21;

float ultrasonic_get_distance_cm(void) {
    /* Escribo un 1 en el Trigger */
  gpio_put(TRIGGER, true);
  /* Espero 10 us con el pulso encendido */
  sleep_us(10);
  /* Escribo un 0 en el Trigger */
  gpio_put(TRIGGER, false);
  /* Espero a que el pulso llegue al Echo */
  while(!gpio_get(ECHO));
  /* Mido el tiempo de cuando llega el pulso */
  absolute_time_t from = get_absolute_time();
  /* Espero a que el pulso del Echo baje a cero */
  while(gpio_get(ECHO));
  /* Mido el tiempo cuando termina el pulso */
  absolute_time_t to = get_absolute_time();
  /* Saco la diferencia de tiempo (el ancho del pulso) en us */
  int64_t pulse_width = absolute_time_diff_us(from, to);
  /* Calculo la distancia */
  float distance = pulse_width / 59.0;
  return distance;
}
void ultrasonic_init(uint8_t trigger, uint8_t echo){
  /* Habilito el pin de Trigger */
  gpio_init(trigger);  
  /* Configuro el Trigger como salida */
  gpio_set_dir(trigger, true);
  /* Pongo un 0 en el pin de Trigger */
  gpio_put(trigger, false);
  /* Habilito el pin de Echo */
  gpio_init(echo);
  /* Configuro el Echo como entrada */
  gpio_set_dir(echo, false);
}

int main() {
  gpio_init(LED1);
  gpio_init(LED2);
  gpio_init(LED3);

  gpio_set_dir(LED1, true);
  gpio_set_dir(LED2, true);
  gpio_set_dir(LED3, true);

  /* Habilito el USB */
  stdio_init_all();
  sleep_ms(1000);
  printf("Listo!\r\n");
  ultrasonic_init(TRIGGER, ECHO);

  while (true) {

    float distance=ultrasonic_get_distance_cm();
    /* Muestro por consola */
    printf("Distance: %.2f cm\r\n", distance);
    /* Espero medio segundo */

    if (distance < 10) {
      gpio_put(LED1, true);
      gpio_put(LED2, false);
      gpio_put(LED3, false);
    }
    else if (distance < 50) {
      gpio_put(LED2, true);
      gpio_put(LED1, false);
      gpio_put(LED3, false);
    }
    else if (distance < 100) {
      gpio_put(LED3, true);
      gpio_put(LED1, false);
      gpio_put(LED2, false);
    }
    sleep_ms(500);
  }
}