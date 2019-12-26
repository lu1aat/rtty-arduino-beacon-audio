# rtty-arduino-beacon-audio

Implementacion de una baliza RTTY en Arduino, solo audio. Permite poner un texto que va a ser modulado en RTTY (default 45.5 Baudios) con un tono en un PIN del microcontrolador. Se puede conectar un parlante a este PIN y escuchar el audio.

Es un proyecto simple y basico para experimentar con Arduino en el ambito de la radioaficion. Es una adaptacion de otro proyecto mas simple [cw-arduino-beacon](https://github.com/lu1aat/cw-arduino-beacon/).

Si es lo primero que haces con Arduino, antes tenes que leer un tutorial de Arduino https://www.arduino.cc/en/Guide/HomePage y hacer los ejercicios basicos (titilar, leer un boton, etc). 


## Instrucciones

* Bajar este repositorio o el archivo `rtty-arduino-beacon-audio.ino`.
* Abrir el archivo con la IDE de Arduino (https://www.arduino.cc/en/main/software).
* Subir el programa al microcontrolador.
* Conectar un parlante piezoelectrico al PIN 13 del arduino y a algun PIN de GND (tierra).

La baliza deberia comenzar a transmitir inmediatamente y hacer pausas de un minuto entre transmisiones.

<img src="https://github.com/lu1aat/cw-arduino-beacon/raw/master/diagram-connections.png">

<img src="https://github.com/lu1aat/rtty-arduino-beacon-audio/raw/master/image_fl-digi.png">

## Configurando la baliza

Si no modificamos nada, la baliza comenzara a transmitir con los parametros por defecto. Para modificar estos parametros hay que usar la IDE de arduino, editar el codigo y subirlo nuevamente al microcontrolador.

### Mensaje

El mensaje por defecto, que solo admite mayusculas y numeros, esta definido en la linea:

```c++
char MESSAGE[] = "CQ DE N0CAL";
```

Cambiando el valor entre comillas se modifica el mensaje de la baliza.

### Tono

Algunos parametros de los tonos se pueden cambiar en:

```c++
const int markFreqHz = 2125;        // Frecuencia de la marca (MARK)
const int shiftFreqHz = 170;        // Variacion de frecuencia del espacio (SPACE) respecto de la marca (MARK)
const int spaceFreqHz = markFreqHz - shiftFreqHz;
const int toneDurationMs = 22;      // Duracion de los tonos, 22 hace que sean 45.5 baudios
```

Las frecuencias estan expresadas en Hz. La marca (MARK) es la frecuencia superior y espacio (SPACE) inferior. 


### Intervalo

El tiempo de espera en segundos, entre trasmision y transmision, puede ser cambiando desde:

```c++
const int SLEEP_SEC = 60;
```

### PIN de transmision

El PIN de salida de audio esta defindo en:

```c++
const int OUT_PIN = 13;
```

En el Arduino UNO ese PIN es el mismo que se utiliza para el LED que trae incorporado, esto facilita hacer pruebas.


## Ayuda

- LU1AAT, Andres
- lu1aat.andres @ gmail.com
