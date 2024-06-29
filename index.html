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
  </style>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <script>
    //setInterval(fetchData, 500); // Atualiza a cada 0.5 segundos
    function updateData(data) {
      document.getElementById('umidade').innerText = data.umidade.toFixed(2) + ' %';
      document.getElementById('temperatura').innerText = data.temperatura.toFixed(2) + ' °C';
      document.getElementById('temp_desejada').innerText = data.temp_desejada + ' °C';
      document.getElementById('umidade_desejada').innerText = data.umidade_desejada + ' %';
      document.getElementById('tempo_atividade').innerText = data.tempo_atividade + ' s';
      document.getElementById('tempo_total').innerText = data.tempo_total + ' s';
      document.getElementById('tempo_aquecimento').innerText = data.tempo_aquecimento + ' s';

      updateCharts(data.humidity_history, data.temperature_history);
    }

    function updateCharts(humidityHistory, temperatureHistory) {
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

      var ctxHumidity = document.getElementById('humidityChart').getContext('2d');
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

      var ctxTemperature = document.getElementById('temperatureChart').getContext('2d');
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
    function enviarTempoAlvo() {
      var tempoAlvo = document.getElementById('tempoAlvo').value;
      console.log('Tempo alvo enviado: ' + tempoAlvo);
      // Adicione aqui o código para enviar o tempo alvo ao servidor
    }

    function enviarTemperaturaAlvo() {
      var temperaturaAlvo = document.getElementById('temperaturaAlvo').value;
      console.log('Temperatura alvo enviada: ' + temperaturaAlvo);
      // Adicione aqui o código para enviar a temperatura alvo ao servidor
    }

    function aquecer() {
      console.log('Aquecendo...');
      // Adicione aqui o código para iniciar o aquecimento
    }

    function definirPerfil(perfil) {
      console.log('Perfil definido: ' + perfil);
      // Adicione aqui o código para definir o perfil de temperatura
    }
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
    <div style="display: flex;">
      <div id="blocoA" style="display: flex; flex-direction: column;">
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <select id="tempoEscala">
            <option value="horas">Horas</option>
            <option value="minutos">Minutos</option>
            <option value="segundos">Segundos</option>
          </select>
          <input type="text" id="tempoAlvo" placeholder="Tempo" style="margin-left: 10px; margin-right: 10px;">
          <button onclick="enviarTempoAlvo()">Enviar Tempo</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <input type="text" id="temperaturaAlvo" placeholder="Temperatura em °C" style="margin-right: 10px;">
          <button onclick="enviarTemperaturaAlvo()">Enviar Temperatura</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <input type="text" id="umidadeAlvo" placeholder="Umidade em %" style="margin-right: 10px;">
          <button onclick="enviarUmidadeAlvo()">Enviar Umidade</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <div style="margin-left: 10px; margin-right: 10px;">
            <canvas id="humidityChart"></canvas>
          </div>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <input type="text" id="ventilacaoAlvo" placeholder="Ventilação em %">
          <button onclick="enviarVentilacaoAlvo()">Enviar Ventilação</button>
        </div>
      </div>
      
      <div id="blocoB" style="display: flex; flex-direction: column; width: 120%; margin-left: 20px;">
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <button onclick="definirPerfil('PLA')" style="width: 50%;">PLA</button>
          <button onclick="definirPerfil('Food')" style="width: 50%;">Food</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <button onclick="definirPerfil('ABS')" style="width: 50%;">ABS</button>
          <button onclick="definirPerfil('PETG')" style="width: 50%;">PETG</button>
        </div>
      </div>
      
      <div id="blocoC" style="display: flex; flex-direction: column; width: 160%; margin-left: 20px;">
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <button onclick="aquecer()" style="width: 50%;">Aquecer</button>
          <button onclick="resfriar()" style="width: 50%;">Resfriar</button>
        </div>
        <div style="display: flex; align-items: center; margin-bottom: 10px;">
          <button onclick="ligarVentilacao()" style="width: 50%;">On Ventilação</button>
          <button onclick="desligarVentilacao()" style="width: 50%;">Off Ventilação</button>
        </div>
      </div>
    </div>
    
    
    <!-- Gráficos -->
    <div class="chart-container">
      <canvas id="humidityChart"></canvas>
    </div>
    <div class="chart-container">
      <canvas id="temperatureChart"></canvas>
    </div>
    <div style="clear: both;"></div>
  </div>
</body>
</html>
)rawliteral";