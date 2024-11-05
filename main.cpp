#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Definir pines de los componentes
LiquidCrystal_I2C lcd(0x20, 16, 2);  // Crear el objeto LCD con dirección 0x20 y tamaño de 16x2
const int ledVerdePin = 11;    // Pin del LED verde
const int ledAmarilloPin = 10; // Pin del LED amarillo
const int ledRojoPin = 9;      // Pin del LED rojo
const int motorPin = 5;        // Pin del transistor que controla el motor

// Definir límites de temperatura (en grados Celsius)
const float tempVerde = 25.0;     // Temperatura mínima para enfriar hasta LED verde
const float tempAmarillo = 30.0;  // Temperatura máxima para LED amarillo
const float tempRojo = 35.0;      // Temperatura crítica para encender el LED rojo y el motor

// Variable global para almacenar la temperatura simulada
float temperaturaSimulada = 20.0;  // Temperatura inicial

// Variable para controlar el ciclo de aumento o disminución de temperatura
bool enfriando = false;

void setup() {
  pinMode(ledVerdePin, OUTPUT);
  pinMode(ledAmarilloPin, OUTPUT);
  pinMode(ledRojoPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  
  // Inicializar el LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Temperatura");
  lcd.setCursor(0, 1);
  
  Serial.begin(9600);  // Inicializar comunicación serial para depuración
}

void loop() {
  // Mostrar la temperatura simulada en la consola serial y en la pantalla LCD
  Serial.print("Temperatura: ");
  Serial.print(temperaturaSimulada);
  Serial.println(" C");
  
  lcd.clear();
  lcd.print("Temp: ");
  lcd.print(temperaturaSimulada);
  lcd.print(" C");
  
  // Controlar el ciclo de temperatura y el estado de los LEDs y el motor
  if (!enfriando) {  // Ciclo de aumento de temperatura
    if (temperaturaSimulada < tempVerde) {
      // LED verde encendido, temperatura normal
      digitalWrite(ledVerdePin, HIGH);
      digitalWrite(ledAmarilloPin, LOW);
      digitalWrite(ledRojoPin, LOW);
      digitalWrite(motorPin, LOW);  // Motor apagado
    } 
    else if (temperaturaSimulada >= tempVerde && temperaturaSimulada < tempAmarillo) {
      // LED amarillo encendido, temperatura media
      digitalWrite(ledVerdePin, HIGH);
      digitalWrite(ledAmarilloPin, HIGH);
      digitalWrite(ledRojoPin, LOW);
      digitalWrite(motorPin, LOW);  // Motor apagado
    } 
    else if (temperaturaSimulada >= tempAmarillo && temperaturaSimulada < tempRojo) {
      // LED rojo encendido, temperatura alta
      digitalWrite(ledVerdePin, HIGH);
      digitalWrite(ledAmarilloPin, HIGH);
      digitalWrite(ledRojoPin, HIGH);
      digitalWrite(motorPin, LOW);  // Motor apagado
    } 
    else if (temperaturaSimulada >= tempRojo) {
      // Al alcanzar el límite superior, activar el motor y comenzar a enfriar
      digitalWrite(ledVerdePin, HIGH);
      digitalWrite(ledAmarilloPin, HIGH);
      digitalWrite(ledRojoPin, HIGH);
      digitalWrite(motorPin, HIGH);  // Motor encendido para enfriar
      enfriando = true;
    }
    
    // Incrementar la temperatura en cada ciclo si estamos en el modo de aumento
    if (!enfriando) {
      temperaturaSimulada += 2;  // Ajusta este valor para controlar la velocidad de aumento
    }
  } else {  // Ciclo de enfriamiento
    if (temperaturaSimulada > tempVerde) {
      // Reducir la temperatura para simular el enfriamiento
      temperaturaSimulada -= 2;  // Ajusta este valor para controlar la velocidad de enfriamiento

      // Controlar los LEDs según la temperatura en modo enfriamiento
      if (temperaturaSimulada < tempVerde) {
        digitalWrite(ledVerdePin, HIGH);
        digitalWrite(ledAmarilloPin, LOW);
        digitalWrite(ledRojoPin, LOW);
      } 
      else if (temperaturaSimulada >= tempVerde && temperaturaSimulada < tempAmarillo) {
        digitalWrite(ledVerdePin, HIGH);
        digitalWrite(ledAmarilloPin, HIGH);
        digitalWrite(ledRojoPin, LOW);
      } 
      else if (temperaturaSimulada >= tempAmarillo) {
        digitalWrite(ledVerdePin, HIGH);
        digitalWrite(ledAmarilloPin, HIGH);
        digitalWrite(ledRojoPin, HIGH);
      }
    } 
    else {
      // Cuando la temperatura baja a tempVerde o menos, apagar el motor y reiniciar el ciclo
      digitalWrite(motorPin, LOW);  // Motor apagado
      enfriando = false;  // Reiniciar el ciclo de aumento de temperatura
    }
  }

  // Limitar la temperatura dentro de un rango razonable
  if (temperaturaSimulada < 0) temperaturaSimulada = 0;
  if (temperaturaSimulada > 50) temperaturaSimulada = 50;

  // Esperar un segundo antes del siguiente ciclo
  delay(1000);
}