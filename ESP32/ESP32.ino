#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define DHTPIN 21
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

const char* ssid = "Luna";
const char* password = "@brigadeiro!";
const char HTML_PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Dados do Sensor DHT</title>
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
  </style>
  <script>
    // Função para atualizar dados na página
    function updateData(data) {
      document.getElementById('umidade').innerText = data.umidade + ' %';
      document.getElementById('temperatura').innerText = data.temperatura + ' °C';
      document.getElementById('temp_desejada').innerText = data.temp_desejada + ' °C';
      document.getElementById('umidade_desejada').innerText = data.umidade_desejada + ' %';
      document.getElementById('tempo_atividade').innerText = data.tempo_atividade + ' s';
      document.getElementById('tempo_total').innerText = data.tempo_total + ' s';
      document.getElementById('tempo_aquecimento').innerText = data.tempo_aquecimento + ' s';
    }

    // Função para fazer requisição AJAX para obter dados do servidor
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

    // Atualiza os dados a cada 5 segundos
    setInterval(fetchData, 5000);
  </script>
</head>
<body>
  <div class="container">
    <h1>Dados do Sensor DHT</h1>
    <table>
      <tr>
        <th colspan="3">Título</th>
      </tr>
      <tr>
        <th>##DHT</th>
        <th>##Configuração</th>
        <th>##Tempo</th>
      </tr>
      <tr>
        <td rowspan="2">Humidade <span id="umidade">{umidade}</span></td>
        <td>Temp. desejada <span id="temp_desejada">{temp_desejada}</span></td>
        <td rowspan="2">Tempo de atividade <span id="tempo_atividade">{tempo_atividade}</span></td>
      </tr>
      <tr>
        <td>Humidade desejada <span id="umidade_desejada">{umidade_desejada}</span></td>
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
  </div>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleData() {
  StaticJsonDocument<200> doc;
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  doc["umidade"] = humidity;
  doc["temperatura"] = temperature;
  doc["temp_desejada"] = 25.0; // Valores desejados fictícios
  doc["umidade_desejada"] = 60.0;
  doc["tempo_atividade"] = 10.0;
  doc["tempo_total"] = 00;
  doc["tempo_aquecimento"] = 5.0;
  
  String output;
  serializeJson(doc, output);
  server.send(200, "application/json", output);
}

void setup() {
  Serial.begin(115200);
  delay(100);
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
