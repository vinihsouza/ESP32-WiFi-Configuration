# ESP32 WiFi Configuration

## Descrição
Este é um código-fonte para um dispositivo ESP32 que pode ser configurado como um ponto de acesso WiFi (Access Point - AP) ou se conectar a uma rede WiFi existente. Ele também utiliza a biblioteca NTPClient para obter a hora de um servidor NTP (Network Time Protocol).

## Configuração Inicial
Antes de executar o código, certifique-se de instalar as seguintes bibliotecas:

- [EEPROM](https://www.arduino.cc/en/Reference/EEPROM)
- [NTPClient](https://github.com/arduino-libraries/NTPClient)
- [WebServer](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/libraries/WebServer.md)
- [WiFi](https://www.arduino.cc/en/Reference/WiFi)

Você pode instalá-las usando o Gerenciador de Bibliotecas da Arduino IDE.

## Parâmetros de Configuração
- **ssidAP:** SSID (nome) do ponto de acesso quando configurado como AP.
- **password:** Senha do ponto de acesso quando configurado como AP.
- **ntpServer:** Servidor NTP utilizado para sincronização de hora.
- **gmtOffset_sec:** Fuso horário em segundos (0 para GMT).
- **daylightOffset_sec:** Horário de verão em segundos (0 para desativar).
- **eepromSize:** Tamanho total da EEPROM.
- **ssidAddress:** Endereço de memória para armazenar o SSID na EEPROM.
- **passAddress:** Endereço de memória para armazenar a senha na EEPROM.

## Funcionalidades
1. **Modo AP (Access Point):** Se as credenciais WiFi não estiverem disponíveis, o dispositivo é configurado como um AP, permitindo a configuração das credenciais via navegador.
2. **Modo Cliente WiFi:** Se as credenciais WiFi estão disponíveis, o dispositivo se conecta à rede WiFi configurada.
3. **Página de Configuração Web:** Uma página web simples permite a configuração do SSID e da senha quando em modo AP.

## Endpoints Web
1. **"/":** Página inicial ao acessar o dispositivo.
2. **"/save":** Endpoint para salvar as credenciais fornecidas via página de configuração web.

## Como Utilizar
1. Carregue o código no seu dispositivo ESP32 usando a Arduino IDE.
2. Abra o Monitor Serial para visualizar mensagens de depuração.
3. No primeiro uso ou se as credenciais não estiverem disponíveis, o dispositivo será configurado como um AP. Conecte-se à rede WiFi fornecida pelo dispositivo.
4. Abra um navegador e acesse "http://192.168.1.1" para configurar as credenciais WiFi.
5. Após configurar as credenciais, o dispositivo tentará se conectar à rede WiFi fornecida.

## Observações
- Certifique-se de que o dispositivo ESP32 está conectado à sua máquina para visualizar mensagens no Monitor Serial.
- As credenciais WiFi são armazenadas na EEPROM, permitindo a persistência entre reinicializações.

Este código pode ser uma base para projetos que exigem configuração flexível de rede WiFi em dispositivos ESP32. Sinta-se à vontade para adaptar conforme suas necessidades.
