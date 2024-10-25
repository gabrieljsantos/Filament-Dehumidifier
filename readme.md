# Manual do Projeto Filament-Dehumidifier Arduino-NANO

## Descrição do Projeto

Este projeto utiliza um sensor de temperatura e umidade AAM2302B (DHT22) para monitorar e controlar a temperatura em um ambiente específico. As leituras são exibidas em um display LCD PCD8544, permitindo que o usuário ajuste os limites de temperatura utilizando um teclado matricial. O aquecedor é acionado ou desligado com base nas leituras do sensor.

## Componentes Utilizados

- **Microcontrolador**: Arduino (por exemplo, Arduino Nano)
- **Sensor de Temperatura e Umidade**: AAM2302B (DHT22)
- **Display LCD**: Adafruit PCD8544
- **Teclado Matricial**: 2x2 (usando multiplexação para reduzir o número de pinos e permitir mais botões)
- **Relé**: Para controlar o aquecedor
- **LED 1**: Para indicar o status do aquecedor
- **LED 2**: Para indicar status gerais
- **Transistor regulador de tensão**: Entrada 12V, saída 5V
- **Cooler de computador**: Especificamente de fonte de alimentação
- **Aquecedor de air fryer de 120V (resistência)**: Ligado em 12V
- **Caixa de isopor**: Para isolamento

## Conexões Físicas

### Pinagem

- **Sensor DHT**:
  - Pino de dados conectado ao pino **7** do Arduino.

- **Display LCD PCD8544**:
  - SCK: pino **2**
  - MOSI: pino **3**
  - DC: pino **4**
  - CE: pino **5**
  - RST: pino **6**

- **Teclado Matricial**:
  - Linha 1: pino **A1**
  - Linha 2: pino **A2**
  - Coluna 1: pino **A3**
  - Coluna 2: pino **A4**

- **Relé e LED**:
  - Relé do aquecedor: pino **8**
  - LED indicador: pino **9**

## Funcionamento do Código

1. **Inicialização**:
   - O código inicializa o sensor, o display e os pinos do teclado e do relé.
   - O tempo de início do sistema é registrado.

2. **Leitura de Dados**:
   - Em um loop contínuo, o sistema lê a temperatura e a umidade do sensor.
   - A umidade absoluta é calculada com base nas leituras de temperatura e umidade.

3. **Controle do Aquecedor**:
   - O aquecedor é acionado se a temperatura medida estiver abaixo do limite especificado.
   - O LED indicador no pino D9 fornece feedback visual sobre o estado do aquecedor.

4. **Ajuste de Limites**:
   - O usuário pode ajustar o limite de temperatura pressionando as teclas do teclado matricial:
     - **'u' (Seta para Cima)**: Aumenta o limite de temperatura em 0,5 °C.
     - **'d' (Seta para Baixo)**: Diminui o limite de temperatura em 0,5 °C.
     - **'y' (OK)**: Confirma a ação.
     - **'n' (Retorno)**: Retorna a ação.

5. **Indicações Visuais**:
   - O LED pisca de diferentes maneiras para indicar o estado do sistema:
     - **Regular**: Pisca duas vezes.
     - **Shutdown Ativado**: Pisca rapidamente quatro vezes.
     - **Temperatura 1**: Brilho constante em nível alto.
     - **Temperatura 2**: Brilho constante em nível baixo.

## Interações do Usuário

- O usuário pode interagir com o sistema utilizando os botões do teclado matricial. Os rótulos dos botões são:
  - **'y'**: **OK** (✔️ confirma a ação)
  - **'n'**: **↩** (cancela a ação)
  - **'u'**: **▲** (aumenta o limite de temperatura)
  - **'d'**: **▼** (diminui o limite de temperatura)

## Manual de Operação do Sistema

### Introdução

Este projeto é um controlador de temperatura e umidade baseado no Arduino, usando um display Nokia 5110 e um sensor de temperatura e umidade (DHT22). O sistema permite o controle manual dos limites de temperatura para o aquecimento e exibe valores de temperatura, umidade relativa, umidade absoluta e o tempo de execução total no display.

### Hardware Utilizado
- **Arduino**: Para processar dados e controlar o sistema.
- **Sensor de Temperatura e Umidade AAM2302B (DHT22)**: Medições de temperatura e umidade.
- **Display Nokia 5110**: Exibição das informações do sistema.
- **Teclado de 4 botões**: Controle da interface.
- **Aquecedor e LED**: Para controle de aquecimento baseado nos limites configurados.
- **Relé**: Ativado/desativado conforme a temperatura.

### Descrição das Telas

#### Tela Principal
A tela principal exibe as seguintes informações:
- **Temperatura (T)**: Mostra a temperatura atual em graus Celsius (°C).
- **Umidade Relativa (RHd)**: Exibe a umidade relativa do ambiente em porcentagem (%).
- **Umidade Absoluta (AHd)**: Mostra a umidade absoluta em gramas por metro cúbico (g/m³).
- **Limite de Temperatura (ST)**: Exibe o limite atual de temperatura para o aquecedor. Se a configuração do limite de temperatura estiver ativa, a exibição deste valor será destacada.
- **Tempo de Execução (RT)**: Mostra o tempo de execução total do sistema no formato **dd:hh:mm:ss** (dias, horas, minutos, segundos).

### Controles do Teclado

- **Seta para Cima ('u')**: Utilizada para aumentar o valor do limite de temperatura durante a configuração.
- **Seta para Baixo ('d')**: Utilizada para diminuir o valor do limite de temperatura durante a configuração.
- **OK ('y')**: Utilizado para confirmar ou alternar a configuração de limite de temperatura.
- **Retorno ('n')**: Utilizado para cancelar ou retornar o valor do limite de temperatura ao seu valor anterior.

### Funções Especiais

- **Forçar Desligamento do Aquecedor**: 
  - Pressione simultaneamente as teclas **'u'** e **'d'** (correspondência: **▲** + **▼**), ( **ok** confirma a ação)
  - **'n'**: **↩** (cancela a ação)
  - O estado de desligamento será exibido no monitor serial.

- **Trocar Limite de Temperatura**: 
  - Pressione simultaneamente as teclas **'y'** e **'d'** (correspondência: **ok** + **▼**), para alternar o limite de temperatura para um valor secundário predefinido (43.5 °C).
  - Essa ação será confirmada no monitor serial com a nova temperatura limite.

- **Alterar o Limite de Temperatura**:
  - Pressione **'y'** (**ok**) para ativar a configuração do limite de temperatura. O valor de limite exibido será destacado.
  - Use as teclas **'u'** (**▲**) e **'d'** (**▼**) para ajustar o valor do limite em incrementos de 0,5 °C.
  - Pressione **'n'** (**↩**) para cancelar e retornar o limite de temperatura ao valor original.

### Ajuste da Temperatura
O aquecedor será ligado e desligado automaticamente com base nos limites de temperatura configurados, mantendo uma tolerância de ±0,5 °C. O LED conectado ao pino D9 indicará o estado do aquecedor: aceso quando o aquecedor estiver em funcionamento e apagado quando estiver desligado.

## Conclusão

Este projeto é uma solução eficaz para o controle de temperatura, permitindo monitoramento em tempo real e ajustes fáceis. O uso de um display LCD e um teclado matricial fornece uma interface intuitiva para o usuário.

---