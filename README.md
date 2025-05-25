graph TD
    ESP[ESP32-S3]
    A[Sensor A (Entrada)]
    B[Sensor B (Salida)]
    OLED[Pantalla OLED/LCD]
    LEDV[LED Verde]
    LEDR[LED Rojo]
    BUZ[Buzzer/Altavoz]
    Person1((Persona))
    Person2((Persona))

    Person1 --> A -->|GPIO| ESP
    Person2 --> B -->|GPIO| ESP

    OLED -->|I2C/SPI| ESP
    LEDV -->|GPIO| ESP
    LEDR -->|GPIO| ESP
    BUZ -->|GPIO| ESP
