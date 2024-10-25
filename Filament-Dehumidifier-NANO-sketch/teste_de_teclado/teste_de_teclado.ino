#include <Keypad.h>

// Pinos do teclado
const int row1 = A1; // Linha 1
const int row2 = A2; // Linha 2
const int column1 = A3; // Coluna 1
const int column2 = A4; // Coluna 2

// Mapeamento das teclas
char keys[2][2] = {
  {'u', 'd'},  // 'u' para cima, 'd' para baixo
  {'y', 'n'}   // 'y' para sim, 'n' para não
};

// Conectar os pinos de linha e coluna
byte rowPins[2] = {row1, row2};     
byte columnPins[2] = {column1, column2};

// Criar o objeto Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, 2, 2);

void setup() {
  // Inicializar o monitor serial
  Serial.begin(9600);
  Serial.println("Teste do teclado iniciado. Pressione uma tecla:");

  // Configurar os pinos das linhas como entrada com pull-up
  pinMode(row1, INPUT_PULLUP);
  pinMode(row2, INPUT_PULLUP);
}

void loop() {
  // Ler a tecla pressionada
  char key = keypad.getKey();

  // Se uma tecla foi pressionada, exibir no monitor serial
  if (key) {
    Serial.print("Tecla pressionada: ");
    Serial.println(key);
  }

  delay(100); // Pequeno delay para evitar leituras duplicadas
}
