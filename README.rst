

I2C LM75A y BMP280
################

Descripción
********

Este es un ejemplo para el sensor CJMCU-75 LM75A y el sensor GY-BMP280-3.3, via protocolo I2C, provisto por el sistema operativo Zephyr.


El slave address del LM75A es 0x48, donde A0, A1, y A2 están conectados a tierra (GND).
El slave address del BMP280 es 0x76.

Pruebas del ejemplo
********************

Comando a correr::
    
    rm -rf build/
    west build -p auto -b esp32
    west flash

Algunas veces es necesario hacer el flash mientras se mantiene el botón *Boot* para que el programa sea cargado.

En otra terminal::
    
    sudo minicom ttyUSB0 -s

Y el botón *En* para reiniciar el programa y su resultado.

Rutas de espressif en Zephyr
********************

En el directorio se encuentra el archivo CMakeLists.txt, allí se pueden colocar las variables de entorno para indicar las rutas del Toolchain de espressif - esp32.::

    set(ESPRESSIF_TOOLCHAIN_PATH "~/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/")
    set(ZEPHYR_TOOLCHAIN_VARIANT "espressif")

Por alguna razón, la siguiente variable debe esportarse expliícitamente en la terminal en que se trabaja::

    export ESP_IDF_PATH="${HOME}/esp/esp-idf"


Adicionalmente, este ejemplo contiene en /boards un archivo .overlay. Este permite sobreescribir algunas características, por lo que debe llevar el mismo nombre del archivo que se encuentra en el directorio de Zephyr/boards. O colocar un nombre, adicionando al archivo::

    set(DTC_OVERLAY_FILE "${CMAKE_CURRENT_SOURCE_DIR}/boards/esp32v1.overlay")






