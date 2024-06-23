#define SENSOR_BOIA 12   // Define o pino GPIO12 para o sensor de boia
#define BUZZER_PIN 4     // Define o pino GPIO4 para o buzzer
#define LED_PIN 13    // Define o pino GPIO3 (D0/RX) para o LED azul

// Coeficientes da curva de calibração linear
const float m = 50.0;  // Coeficiente angular (ml/cm)
const float b = 100.0; // Coeficiente linear (ml)

float volumeTotalAdicionado = 0.0; // Volume total de água adicionado em ml

unsigned long tempoInicio = 0;
unsigned long tempoFinal = 0;
bool aguaPresente = false;

void setup() {
  pinMode(SENSOR_BOIA, INPUT_PULLUP); // Configura o pino do sensor de boia como entrada com pull-up interno
  pinMode(BUZZER_PIN, OUTPUT);        // Configura o pino do buzzer como saída
  pinMode(LED_PIN, OUTPUT);           // Configura o pino do LED azul como saída
  Serial.begin(115200);
}

void loop() {
  int estadoSensor = digitalRead(SENSOR_BOIA); // Lê o estado do sensor de boia

  Serial.print("Estado do sensor: ");
  if (estadoSensor == LOW) {  // O estado será LOW quando a boia estiver fechando o circuito (detectando água)
    Serial.println("Água presente");
    tone(BUZZER_PIN, 1000);   // Liga o buzzer com um tom de 1000 Hz
    digitalWrite(LED_PIN, HIGH); // Liga o LED azul

    if (!aguaPresente) {
      tempoInicio = millis();
      aguaPresente = true;
    }

  } else {
    Serial.println("Água ausente");
    noTone(BUZZER_PIN);       // Desliga o buzzer
    digitalWrite(LED_PIN, LOW);  // Desliga o LED azul

    if (aguaPresente) {
      tempoFinal = millis();
      unsigned long tempoTotal = tempoFinal - tempoInicio; // Tempo total em milissegundos
      float comprimentoBoia = tempoTotal / 1000.0; // Comprimento da boia em cm (exemplo)
      float volumeAdicionado = m * comprimentoBoia + b; // Estimativa do volume adicionado
      volumeTotalAdicionado += volumeAdicionado; // Adiciona o volume ao total
      Serial.print("Volume adicionado nesta vez: ");
      Serial.print(volumeAdicionado);
      Serial.println(" ml");
      Serial.print("Volume total adicionado: ");
      Serial.print(volumeTotalAdicionado);
      Serial.println(" ml");
      aguaPresente = false;
    }
  }

  delay(1000);  // Aguarda 1 segundo antes de ler novamente
}