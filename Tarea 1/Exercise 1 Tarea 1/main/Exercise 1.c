#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"

#define led1 2 //pin salida
uint8_t led_level = 0;  //indicador de estado
static const char *tag = "Main"; 
TimerHandle_t xTimers; //variable global
int interval = 500; //intervalo de tiempo entre evento
int timerId = 1; //funcion iniciadora

esp_err_t init_led(void); //declaramos la funcion para inicio
esp_err_t blink_led(void); //declaramos la funcion para blink
esp_err_t set_timer(void); //declaramos la funcion de timer

void vTimerCallback(TimerHandle_t pxTimer)
{
    ESP_LOGI(tag, "Event was called from timer"); //cada vez que termine de contar
    blink_led();
}

void app_main(void)
{
    init_led();
    set_timer();
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led1);// reseteamos el pin
    gpio_set_direction(led1, GPIO_MODE_OUTPUT); //le ponemos la direccion y el tipo de ato
    return ESP_OK; //es de buena practica
}
esp_err_t blink_led(void)
{
    led_level = !led_level; //invertirle el valor
    gpio_set_level(led1, led_level); //le asisgnamos un pin y el tipo de led
    return ESP_OK;
}

esp_err_t set_timer(void)
{
    ESP_LOGI(tag, "Timer init configuration");
    xTimers = xTimerCreate("Timer",                   // Just a text name, not used by the kernel.
                           (pdMS_TO_TICKS(interval)), // funcion que cambia de ms a ticks
                           pdTRUE,                    // The timers will auto-reload themselves when they expire.
                           (void *)timerId,           // Assign each timer a unique id equal to its array index.
                           vTimerCallback             // Each timer calls the same callback when it expires.
    );

    if (xTimers == NULL)
    {
        // The timer was not created.
        ESP_LOGE(tag, "The timer was not created."); //time no se ejecuto
    }
    else
    {
        if (xTimerStart(xTimers, 0) != pdPASS) //si no inicia el timer
        {
            // The timer could not be set into the Active state.
            ESP_LOGE(tag, "The timer could not be set into the Active state");
        }
    }

    return ESP_OK;
}