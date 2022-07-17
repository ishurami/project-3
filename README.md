## Board

```
Generic ESP8266 Module (WeMos D1 R1)
```

## ESP8266 Board Manager URL

```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

## Pin
### Vibration Sensor (SW-420)
| SW-420    | Micro                 |
| --------- | --------------------- |
| VCC       | 5V                    |
| GND       | GND                   |
| DO        | GPIO 03 / RX <- DO    |

### IR Speed Sensor (LM393)
| LM393     | Micro                 |
| --------- | --------------------- |
| VCC       | 5V                    |
| GND       | GND                   |
| DO        | GPIO 05 / D15/SCL/D3  |

### Capacitor 104

## Info
- Only GPIO 16 that cannot be implemented as interrupt pin.
- Serial monitor show nothing when GPIO 01 become input pin.
- While uploading program, plug out pin GPIO 03 for program to be uploaded successfully.