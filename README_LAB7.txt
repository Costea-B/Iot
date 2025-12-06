# Lab 7.1: Comunicație I²C cu FreeRTOS - Arduino Mega 2560

## Descriere
Acest proiect implementează o comunicație I²C între două microcontrolere Arduino Mega 2560:
- **SLAVE**: Citește date de la senzorul ultrasonic HC-SR04 folosind FreeRTOS
- **MASTER**: Interoghează slave-ul periodic și afișează datele primite prin Serial

## Structura Proiectului

```
lab0/
├── src/
│   ├── slave_main.cpp      # Cod pentru Arduino SLAVE
│   ├── master_main.cpp     # Cod pentru Arduino MASTER
│   └── main.cpp            # Cod original (nefolosit în acest lab)
├── platformio.ini          # Configurare cu 3 environment-uri
├── SCHEMA_CONEXIUNI_I2C.txt # Schema detaliată de conexiuni hardware
└── README_LAB7.txt         # Acest fișier
```

## Cerințe Hardware

- 2x Arduino Mega 2560
- 1x Senzor ultrasonic HC-SR04
- 2x Rezistențe 4.7kΩ (pull-up pentru I²C)
- 1x Breadboard
- Fire jumper
- 2x Cabluri USB

## Instalare și Compilare

### Pas 1: Programarea SLAVE

1. Conectează Arduino Mega SLAVE la PC prin USB
2. În terminal PlatformIO, rulează:
   ```
   pio run -e slave -t upload
   ```
3. Verifică funcționarea prin Serial Monitor (9600 baud):
   ```
   pio device monitor -e slave -b 9600
   ```

### Pas 2: Programarea MASTER

1. Deconectează SLAVE și conectează Arduino Mega MASTER la PC
2. În terminal PlatformIO, rulează:
   ```
   pio run -e master -t upload
   ```
3. Verifică funcționarea prin Serial Monitor (9600 baud):
   ```
   pio device monitor -e master -b 9600
   ```

## Conexiuni Hardware

### SLAVE (Arduino Mega cu HC-SR04)
- Pin 8 → HC-SR04 TRIG
- Pin 9 → HC-SR04 ECHO
- Pin 20 (SDA) → Master Pin 20 + Rezistență 4.7kΩ la 5V
- Pin 21 (SCL) → Master Pin 21 + Rezistență 4.7kΩ la 5V
- 5V → HC-SR04 VCC
- GND → GND comun (SLAVE + MASTER + HC-SR04)

### MASTER (Arduino Mega)
- Pin 20 (SDA) → Slave Pin 20
- Pin 21 (SCL) → Slave Pin 21
- GND → GND comun (OBLIGATORIU!)

**IMPORTANT**: GND-ul comun între Master și Slave este esențial pentru comunicația I²C!

## Funcționare

### SLAVE
- **Task 1 (ReadUltrasonic)**: Citește distanța de la HC-SR04 la fiecare 500ms
- **Task 2 (UpdateBuffer)**: Actualizează buffer-ul I²C cu date noi la fiecare 100ms
- **Callback I²C**: Răspunde automat la cereri de la Master cu pachetul de date

### MASTER
- Interoghează SLAVE-ul la fiecare 1000ms
- Validează datele primite (header și checksum)
- Afișează distanța măsurată în Serial Monitor
- **Comportament adițional**: Alertă când obiect < 10 cm

## Format Pachet I²C

```c
typedef struct {
    uint8_t head;       // Header: 0xAA
    uint8_t length;     // Lungime payload: 2 bytes
    uint16_t distance;  // Distanța măsurată (cm)
    uint8_t checksum;   // XOR: head ^ length ^ (distance>>8) ^ (distance&0xFF)
} DataPacket;
```

## Monitorizare Serial

### Output SLAVE
```
SLAVE: Initializare...
SLAVE: I2C initializat la adresa 0x08
SLAVE: Taskuri FreeRTOS create
SLAVE: Distanta citita: 25 cm
SLAVE: Trimis pachet I2C - Distanta: 25 cm, Checksum: 0x8F
```

### Output MASTER
```
MASTER: Initializare...
MASTER: I2C initializat ca master
========================================
MASTER: Pachet I2C valid primit
  Header:   0xAA
  Length:   2
  Distance: 25 cm
  Checksum: 0x8F [OK]
========================================
```

## Depanare (Troubleshooting)

### Nu se primesc date
- Verifică GND comun între Master și Slave
- Verifică conexiunile SDA/SCL (Pin 20/21)
- Verifică rezistențele pull-up (4.7kΩ)

### Checksum invalid
- Verifică calitatea firelor și conexiunilor
- Adaugă condensatori de decuplare (0.1µF)

### Senzor returnează 9999
- Verifică conexiunile TRIG/ECHO
- Verifică alimentarea HC-SR04 (5V/GND)

## Caracteristici Implementate (pentru notare)

✓ **50%** - Comunicație I²C funcțională cu date reale de la HC-SR04  
✓ **10%** - Taskuri FreeRTOS cu priorități și sincronizare  
✓ **10%** - Structură pachet cu HEAD, PAYLOAD, CHECKSUM validat  
✓ **10%** - Arhitectură clară și modulară  
✓ **10%** - Schema completă de conexiuni hardware  
✓ **10%** - Comportament adițional: alertă proximitate + detectare eroare senzor  

## Adresa I²C Slave

- Adresa: `0x08`
- Modificabilă în ambele fișiere prin constanta `I2C_SLAVE_ADDRESS`

## Note Importante

1. **FreeRTOS pentru Slave**: Asigură-te că biblioteca FreeRTOS este instalată
2. **GND comun**: Esențial pentru comunicația I²C!
3. **Pull-up rezistențe**: Necesare pentru stabilitatea I²C (4.7kΩ)
4. **Serial Monitor**: Folosește 9600 baud pentru ambele plăci
5. **Timeout HC-SR04**: Configurat la 30ms pentru a evita blocarea

## Autor

Lab 7.1 - IoT - ANU IV  
Implementare pentru Arduino Mega 2560 cu FreeRTOS
