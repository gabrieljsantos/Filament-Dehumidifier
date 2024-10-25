#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <DHT.h>  // Para o sensor de temperatura e umidade AAM2302B
#include <Keypad.h>

#define DHTPIN 7    // Pino de dados do sensor
#define DHTTYPE DHT22 // Tipo do sensor (AAM2302B é equivalente ao DHT22)

DHT dht(DHTPIN, DHTTYPE);

// Pinos do LCD
Adafruit_PCD8544 display = Adafruit_PCD8544(2, 3, 4, 5, 6);

// Pinos para o teclado
const int row1 = A1; // Linha 1
const int row2 = A2; // Linha 2
const int column1 = A3; // Coluna 1
const int column2 = A4; // Coluna 2

bool SetTL = false; // Inicializa como false

// Teclas do teclado
char keys[2][2] = {
  {'u', 'd'},  // 'u' para cima, 'd' para baixo
  {'y', 'n'}   // 'y' para sim, 'n' para não
};

byte rowPins[2] = {row1, row2};     // Conecte os pinos das linhas
byte columnPins[2] = {column1, column2}; // Conecte os pinos das colunas

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, 2, 2);

// Variáveis de estado dos botões
unsigned long startTime;
float tempLimit = 37.5;  // Limite de temperatura
float tempLimit2 = 43.5;  // Limite de temperatura
float tempLimitBackup = tempLimit;  // Limite de temperatura
bool warm_up = false;
bool force_heater_shutdown = false;

void setup() {
  Serial.begin(9600);
  dht.begin();
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  
  pinMode(8, OUTPUT); // Aquecedor + Led Vermelho
  pinMode(9, OUTPUT); // Led indicador de status
  
  // Armazena o tempo de início
  startTime = millis();
  
  // Configurando linhas do teclado como entrada com pull-up
  pinMode(row1, INPUT_PULLUP);
  pinMode(row2, INPUT_PULLUP);
}

float calculateSaturationVaporPressure(float temp) {
  return 6.112 * exp((17.67 * temp) / (temp + 243.5));
}

float calculateAbsoluteHumidity(float temp, float relativeHumidity) {
  float e_s = calculateSaturationVaporPressure(temp);  // Pressão de saturação
  float e = e_s * (relativeHumidity / 100.0);  // Pressão parcial de vapor
  return (216.7 * e) / (temp + 273.15);  // Umidade absoluta em g/m³
}

void LedStatus(String Status){
  // Gradiente de brilho suave de 0 a 255 em 1 segundo
  for (int i = 0; i <= 1000; i++) {
    analogWrite(9, (255 * i) / 1000);
    delay(1);  // Totaliza 1000 ms (1 segundo)
  }
  
  // Comparação de strings com if-else
  if (Status == "regular") {
    for (int i = 0; i < 2; i++) {  // Pisca duas vezes com intervalo de 250 ms
      digitalWrite(9, HIGH);
      delay(250);
      digitalWrite(9, LOW);
      delay(250);
    }
  } 
  else if (Status == "fhs_activated") {  // Pisca rápido 4 vezes (shutdown ativado)
    for (int i = 0; i < 4; i++) {
      digitalWrite(9, HIGH);
      delay(125);
      digitalWrite(9, LOW);
      delay(125);
    }
  } 
  else if (Status == "fhs_disabled") {  // Liga no meio do brilho e pisca rápido 4 vezes
    analogWrite(9, 125);  // Brilho médio
    for (int i = 0; i < 4; i++) {
      digitalWrite(9, HIGH);
      delay(125);
      digitalWrite(9, LOW);
      delay(125);
    }
  } 
  else if (Status == "temperatura 1") {
    // Define o comportamento para "temperatura 1"
    analogWrite(9, 200);  // Brilho constante em nível 200 (ajustável)
    delay(1000);
  } 
  else if (Status == "temperatura 2") {
    // Define o comportamento para "temperatura 2"
    analogWrite(9, 50);  // Brilho constante em nível 50 (mais baixo)
    delay(1000);
  }
}


void loop() {
  // Leitura dos dados do sensor
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();  // Umidade relativa já correta

  // Controle do aquecedor
  int tolerance = 0.5;
  if (!force_heater_shutdown) { // Se não for forçar o desligamento
    analogWrite(9, 125); // Liga o LED D9
    digitalWrite(8, LOW);
    if ((tempLimit >= temp + tolerance)) {
      digitalWrite(8, HIGH); // Liga o aquecedor
    } else if (tempLimit <= temp - tolerance) {
      digitalWrite(8, LOW); // Desliga o aquecedor
    }
  } else {
    digitalWrite(9, LOW); // Desliga o LED D9
    digitalWrite(8, LOW); // Desliga o aquecedor se for forçar o desligamento
  }

  // Cálculo da umidade absoluta
  float absoluteHumidity = calculateAbsoluteHumidity(temp, humidity);

  unsigned long elapsedMillis = millis() - startTime;
  unsigned long elapsedSeconds = elapsedMillis / 1000;

  // Calcula dias, horas, minutos e segundos
  unsigned int days = elapsedSeconds / 86400;
  elapsedSeconds %= 86400;
  unsigned int hours = elapsedSeconds / 3600;
  elapsedSeconds %= 3600;
  unsigned int minutes = elapsedSeconds / 60;
  unsigned int seconds = elapsedSeconds % 60;

  display.clearDisplay();

  display.setTextColor(BLACK); 
  display.setTextSize(1);  // Fonte pequena (tamanho 1)
  display.setCursor(0, 0);
  display.print("<  *v*  *v*  >");
  // Exibe os valores de temperatura, umidade e limites
  display.setCursor(0, 8);  // Posição para evitar a linha danificada

  display.print("T  :");
  display.print(temp);
  display.println(" C");

  display.print("RHd:");
  display.print(humidity);
  display.println(" %");

  display.print("AHd:");
  display.print(absoluteHumidity);
  display.println(" g/m3");
  
  display.print("ST :");
  if (SetTL == false) {
    display.setTextColor(BLACK);  // Cores normais
    display.print(tempLimit);
    display.println(" C");
  } else {
    display.setTextColor(WHITE, BLACK);  // Inverte as cores
    display.print(tempLimit);
    display.println(" C");
    display.setTextColor(BLACK);  // Volta para cores normais após a exibição
  }
  

  // Exibe o tempo de execução no formato dd:hh:mm:ss
  display.print("RT:");
  if (days < 10) display.print('0');
  display.print(days);
  display.print(':' );
  
  if (hours < 10) display.print('0');
  display.print(hours);
  display.print(':' );
  
  if (minutes < 10) display.print('0');
  display.print(minutes);
  display.print(':' );
  
  if (seconds < 10) display.print('0');
  display.print(seconds);

  display.display();

  // Leitura do teclado
  char key = keypad.getKey();
  
  // Verifica se as teclas 'u' (up) e 'd' (down) estão pressionadas
  if (keypad.isPressed('u') && keypad.isPressed('d')) { // Alterna o estado do aquecedor
    force_heater_shutdown = !force_heater_shutdown; // Inverte o estado
    Serial.print("Heater shutdown forced: ");
    Serial.println(force_heater_shutdown);
  }

  // Verifica se as teclas 'y' (yes) e 'd' (down) estão pressionadas
  if (keypad.isPressed('y') && keypad.isPressed('d')) {
    // Incrementa o limite de temperatura
    tempLimit += 0.5;
    Serial.print("New temperature limit: ");
    Serial.println(tempLimit);
    SetTL = true;
    LedStatus("temperatura 1");
  }
  
  if (keypad.isPressed('n') && keypad.isPressed('d')) {
    // Decrementa o limite de temperatura
    tempLimit -= 0.5;
    Serial.print("New temperature limit: ");
    Serial.println(tempLimit);
    SetTL = true;
    LedStatus("temperatura 2");
  }

  // Chama a função LedStatus com o status apropriado
  if (force_heater_shutdown) {
    LedStatus("fhs_activated");
  } else {
    LedStatus("regular");
  }
  
  delay(1000); // Aguarda 1 segundo entre as leituras
}
