#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Arduino.h>

#define DHTPIN 21
#define DHTTYPE DHT22
#define N_HUMIDITY_HISTORY 10
#define N_TEMPERATURE_HISTORY 10

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

const char* ssid = "Luna";
const char* password = "@brigadeiro!";
int humidity_history[2][N_HUMIDITY_HISTORY] = {{0}};
unsigned long humidity_history_recording_indexer = 0;
int temperature_history[2][N_TEMPERATURE_HISTORY] = {{0}};
unsigned long temperature_history_recording_indexer = 0;

const char* HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <title>Filament Dehumidifier</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #1e1e1e;
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }
    .container {
      background-color: #2e2b2b;
      padding: 20px;
      border-radius: 8px;
      box-shadow: 0 0 10px rgba(0,0,0,0.5);
      text-align: center;
      width: 80%;
    }
    h1 {
      color: #fff;
    }
    table {
      width: 100%;
      border-collapse: collapse;
      margin-bottom: 20px;
    }
    th, td {
      border: 1px solid #444;
      padding: 8px;
      color: #ddd;
    }
    th {
      background-color: #3e3e3e;
    }
    .chart-container {
      width: 45%;
      margin: 10px;
      float: left;
    }
    .ip-link {
      position: absolute;
      top: 10px;
      right: 10px;
      color: #ddd;
      text-decoration: none;
    }
    .defineTempoAlvo,
    .defineTemperaturaAlvo,
    .defineUmidadeAlvo,
    .defineVentilacaoSet {
      margin-left: 10px;
      margin-right: 10px;
    }
  </style>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <script>
    function updateData(data) {
      document.getElementById('umidade').innerText = data.umidade.toFixed(2) + ' %';
      document.getElementById('temperatura').innerText = data.temperatura.toFixed(2) + ' °C';
      document.getElementById('temp_desejada').innerText = data.temp_desejada + ' °C';
      document.getElementById('umidade_desejada').innerText = data.umidade_desejada + ' %';
      document.getElementById('tempo_atividade').innerText = data.tempo_atividade + ' s';
      document.getElementById('tempo_total').innerText = data.tempo_total + ' s';
      document.getElementById('tempo_aquecimento').innerText = data.tempo_aquecimento + ' s';

      updateGraphics(data.humidity_history, data.temperature_history);
    }

    function updateGraphics(humidityHistory, temperatureHistory) {
      var humidityLabels = [];
      var humidityData = [];
      humidityHistory.forEach(function(item) {
        humidityLabels.push(item.time);
        humidityData.push(item.humidity);
      });

      var temperatureLabels = [];
      var temperatureData = [];
      temperatureHistory.forEach(function(item) {
        temperatureLabels.push(item.time);
        temperatureData.push(item.temperature);
      });

      var ctxHumidity = document.getElementById('humidityGraphic').getContext('2d');
      var chartHumidity = new Chart(ctxHumidity, {
        type: 'line',
        data: {
          labels: humidityLabels,
          datasets: [{
            label: 'Histórico de Umidade',
            data: humidityData,
            backgroundColor: 'rgba(54, 162, 235, 0.2)',
            borderColor: 'rgba(54, 162, 235, 1)',
            borderWidth: 1
          }]
        },
        options: {
          scales: {
            y: {
              min: 0,
              max: 100,
              ticks: {
                stepSize: 20
              }
            }
          }
        }
      });

      var ctxTemperature = document.getElementById('temperatureGraphic').getContext('2d');
      var chartTemperature = new Chart(ctxTemperature, {
        type: 'line',
        data: {
          labels: temperatureLabels,
          datasets: [{
            label: 'Histórico de Temperatura',
            data: temperatureData,
            backgroundColor: 'rgba(255, 99, 132, 0.2)',
            borderColor: 'rgba(255, 99, 132, 1)',
            borderWidth: 1
          }]
        },
        options: {
          scales: {
            y: {
              min: 0,
              max: 100,
              ticks: {
                stepSize: 20
              }
            }
          }
        }
      });
    }

    function fetchData() {
      var xhr = new XMLHttpRequest();

      xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE) {
          if (xhr.status === 200) {
            var data = JSON.parse(xhr.responseText);
            updateData(data);
          } else {
            console.error('Erro ao obter dados do servidor: ' + xhr.status);
          }
        }
      };

      xhr.open('GET', '/data', true);
      xhr.send();
    }

    setInterval(fetchData, 300); // Atualiza a cada 5 segundos
  </script>
</head>
<body>
  <a class="ip-link" href="http://##ESP32_IP##/data">{IP do ESP32}/data</a>
  <div class="container">
    <h1>Filament Dehumidifier</h1>
    <table>
      <tr>
        <th colspan="3">......</th>
      </tr>
      <tr>
        <th>Dados do Sensor DHT</th>
        <th>Configuração</th>
        <th>Tempo</th>
      </tr>
      <tr>
        <td rowspan="2">Umidade <span id="umidade">{umidade}</span></td>
        <td>Temp. desejada <span id="temp_desejada">{temp_desejada}</span></td>
        <td rowspan="2">Tempo de atividade <span id="tempo_atividade">{tempo_atividade}</span></td>
      </tr>
      <tr>
        <td>Umidade desejada <span id="umidade_desejada">{umidade_desejada}</span></td>
      </tr>
      <tr>
        <td rowspan="2">Temperatura <span id="temperatura">{temperatura}</span></td>
        <td>Tempo total <span id="tempo_total">{tempo_total}</span></td>
        <td rowspan="2">Tempo de aquecimento <span id="tempo_aquecimento">{tempo_aquecimento}</span></td>
      </tr>
      <tr>
        <td></td>
      </tr>
    </table>
    <div id="SetsConfigs" style="display: grid; grid-template-columns: repeat(3, 1fr); gap: 20px; width: 100%; margin-top: 20px;">
      <!-- Bloco dos controles de configuração -->
      <div style="display: flex; flex-direction: column;">
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <select id="tempoEscala" style="flex: 1; padding: 5px;">
            <option value="horas">Horas</option>
            <option value="minutos">Minutos</option>
            <option value="segundos">Segundos</option>
          </select>
          <input type="text" id="tempoAlvo" placeholder="Tempo" class="defineTempoAlvo" style="flex: 2; margin-left: 10px; padding: 5px;">
          <button class="defineTempoAlvo" style="flex: 1; margin-left: 10px; padding: 5px;">Enviar Tempo</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <input type="text" id="temperaturaAlvo" placeholder="Temperatura em °C" class="defineTemperaturaAlvo" style="flex: 2; padding: 5px;">
          <button class="defineTemperaturaAlvo" style="flex: 1; margin-left: 10px; padding: 5px;">Enviar Temperatura</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <input type="text" id="umidadeAlvo" placeholder="Umidade em %" class="defineUmidadeAlvo" style="flex: 2; padding: 5px;">
          <button class="defineUmidadeAlvo" style="flex: 1; margin-left: 10px; padding: 5px;">Enviar Umidade</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <input type="text" id="VentilacaoSet" placeholder="Ventilação de 0 a 255" style="flex: 2; padding: 5px;">
          <button class="defineVentilacaoSet" style="flex: 1; margin-left: 10px; padding: 5px;">Enviar Ventilação</button>
        </div>
      </div>
    
      <!-- Bloco dos perfis de material -->
      <div style="display: flex; flex-direction: column;">
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <button onclick="definirPerfil('PLA')" style="flex: 1; padding: 10px;">PLA</button>
          <button onclick="definirPerfil('Food')" style="flex: 1; margin-left: 10px; padding: 10px;">Food</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <button onclick="definirPerfil('ABS')" style="flex: 1; padding: 10px;">ABS</button>
          <button onclick="definirPerfil('PETG')" style="flex: 1; margin-left: 10px; padding: 10px;">PETG</button>
        </div>
      </div>
    
      <!-- Bloco dos controles de aquecimento e ventilação -->
      <div style="display: flex; flex-direction: column;">
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <button onclick="aquecer()" style="flex: 1; padding: 10px;">Aquecer</button>
          <button onclick="resfriar()" style="flex: 1; margin-left: 10px; padding: 10px;">Resfriar</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <button onclick="ligarVentilacao()" style="flex: 1; padding: 10px;">On Ventilação</button>
          <button onclick="desligarVentilacao()" style="flex: 1; margin-left: 10px; padding: 10px;">Off Ventilação</button>
        </div>
      </div>
    </div>
    
     

    <!-- Gráficos -->
    <div class="chart-container">
      <canvas id="humidityGraphic"></canvas>
    </div>
    <div class="chart-container">
      <canvas id="temperatureGraphic"></canvas>
    </div>
    <div style="clear: both;"></div>
  </div>

  <script>
    fetchData(); // Chama fetchData() uma vez para carregar os dados inicialmente
  </script>
</body>
</html>

  
)rawliteral";



String tempo_de_atividade_ddhhmmss(unsigned long time_ms) {
    // Calcula o tempo decorrido em dias, horas, minutos e segundos
    unsigned long segundos = time_ms / 1000;
    unsigned long minutos = segundos / 60;
    unsigned long horas = minutos / 60;
    unsigned long dias = horas / 24;

    segundos %= 60;
    minutos %= 60;
    horas %= 24;

    // String para armazenar o tempo formatado
    char tempo_atividade_formatado[20]; // Ajuste o tamanho conforme necessário

    // Formata o tempo decorrido no formato dd.hh:mm:ss
    sprintf(tempo_atividade_formatado, "%02lu.%02lu:%02lu:%02lu", dias, horas, minutos, segundos);

    // Retorna o tempo formatado como uma String
    return String(tempo_atividade_formatado);
}

void handleRoot() {
  String htmlPage = String(HTML_PAGE);
  htmlPage.replace("##ESP32_IP##", String(WiFi.localIP()));
  server.send(200, "text/html", htmlPage);
}

void handleData() {
  Serial.println("Medindo....");
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  unsigned long tempo_atividade = millis();

  humidity_history[0][humidity_history_recording_indexer] = humidity;
  humidity_history[1][humidity_history_recording_indexer] = tempo_atividade;
  humidity_history_recording_indexer = (humidity_history_recording_indexer + 1) % N_HUMIDITY_HISTORY;


  // Atualiza o histórico de umidade
  for (int n = 0; n < N_HUMIDITY_HISTORY - 1; n++) {
      humidity_history[0][n] = humidity_history[0][n + 1];
      humidity_history[1][n] = humidity_history[1][n + 1];
  }
  humidity_history[0][N_HUMIDITY_HISTORY - 1] = humidity;
  humidity_history[1][N_HUMIDITY_HISTORY - 1] = tempo_atividade;

  // Atualiza o histórico de temperatura
  for (int n = 0; n < N_TEMPERATURE_HISTORY - 1; n++) {
      temperature_history[0][n] = temperature_history[0][n + 1];
      temperature_history[1][n] = temperature_history[1][n + 1];
  }
  temperature_history[0][N_TEMPERATURE_HISTORY - 1] = temperature;
  temperature_history[1][N_TEMPERATURE_HISTORY - 1] = tempo_atividade;
  temperature_history_recording_indexer = (temperature_history_recording_indexer + 1) % N_TEMPERATURE_HISTORY;

  StaticJsonDocument<1024> doc;

  doc["umidade"] = humidity;
  doc["temperatura"] = temperature;
  doc["temp_desejada"] = 60.0; // Valores desejados fictícios
  doc["umidade_desejada"] = 10.0;
  doc["tempo_atividade"] = tempo_de_atividade_ddhhmmss(tempo_atividade);
  doc["tempo_total"] = 20;
  doc["tempo_aquecimento"] = 567;

  JsonArray humidityHistoryArray = doc.createNestedArray("humidity_history");
  for (int i = 0; i < N_HUMIDITY_HISTORY; i++) {
    JsonObject historyItem = humidityHistoryArray.createNestedObject();
    historyItem["humidity"] = humidity_history[0][i];
    historyItem["time"] = humidity_history[1][i] / 1000;
  }

  JsonArray temperatureHistoryArray = doc.createNestedArray("temperature_history");
  for (int i = 0; i < N_TEMPERATURE_HISTORY; i++) {
    JsonObject historyItem = temperatureHistoryArray.createNestedObject();
    historyItem["temperature"] = temperature_history[0][i];
    historyItem["time"] = temperature_history[1][i] / 1000;
  }

  String output;
  serializeJson(doc, output);
  server.send(200, "application/json", output);
}

void setup() {
  Serial.begin(115200);
  //delay(100);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}
