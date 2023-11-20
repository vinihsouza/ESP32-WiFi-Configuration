#include <EEPROM.h>                
#include <NTPClient.h>              //Instalar lib NTPClient
#include <WebServer.h>                             
#include <WiFi.h>
#include <WiFiUdp.h>                

const char* ssidAP = "VINICIUS-AP"; // SSID do ponto de acesso
const char* password = "00000000";  // Senha do ponto de acesso
const char* ntpServer;              // Servidor NTP

const long gmtOffset_sec = -10800;  // Fuso horário em segundos (0 para GMT)
const int daylightOffset_sec = 0;   // Horário de verão em segundos (0 para desativar)
const int eepromSize = 64;          // Tamanho total da EEPROM
const int ssidAddress = 0;          // Endereço de memória para o SSID
const int passAddress = 32;         // Endereço de memória para a senha

char ssid[32];                      // SSID com no máximo 31 caracteres
char pass[32];                      // Senha com no máximo 31 caracteres

IPAddress ip(192, 168, 1, 1);       // Endereço IP do ponto de acesso
IPAddress gateway(192, 168, 1, 1);  // Gateway do ponto de acesso
IPAddress mask(255, 255, 255, 0);   // Máscara de sub-rede do ponto de acesso
WebServer server(80);               // Servidor Web
WiFiUDP ntpUDP;                     // Inicializando classe UDP para ntpUDP

NTPClient timeClient(ntpUDP, "pool.ntp.org", gmtOffset_sec); //Objeto NTPClient configurado com o servidor NTP e o deslocamento do fuso horário

/**
 * @brief Manipulador para requisições bem-sucedidas.
 */
void handle_OnConnect() {
  server.send(200, "text/html", HTML());
}

/**
 * @brief Manipulador para requisições não encontradas.
 */
void handle_NotFound() {
  server.send(404, "text/plain", "Não encontrado");
}

/**
 * @brief Manipulador para salvar as credenciais na EEPROM.
 */
void handle_Save() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Método não permitido");
  } else {
    String newSSID = server.arg("ssid");
    String newPass = server.arg("pass");
    Serial.print("Novo SSID: ");
    Serial.println(newSSID);
    Serial.print("Nova Senha: ");
    Serial.println(newPass);
    saveCredentials(newSSID.c_str(), newPass.c_str());
    server.send(200, "text/html", "Salvo com sucesso, reiniciando...");
    ESP.restart();
  }
}

/**
 * @brief Função para configurar o modo de ponto de acesso.
 */
void APConfigMode() {
  WiFi.mode(WIFI_AP);
  delay(2000);

  WiFi.persistent(false);
  WiFi.softAPConfig(ip, gateway, mask);
  WiFi.softAP(ssidAP, password);
  delay(300);

  Serial.println("Modo de Configuração de Ponto de Acesso (AP)");
  Serial.println(WiFi.softAPIP());
  server.on("/", handle_OnConnect);
  server.on("/save", handle_Save);
  server.onNotFound(handle_NotFound);
  server.begin();
  while (true) {
    server.handleClient();
  }
}

/**
 * @brief Função para conectar-se à rede WiFi.
 */
void connectToWiFi() {
  Serial.println("Conectando-se à rede WiFi...");

  // Conecta-se à rede WiFi
  WiFi.begin(ssid, pass);

  // Aguarda a conexão
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando-se à rede WiFi...");
  }

  Serial.println("Conectado à rede WiFi");
}

/**
 * @brief Função para salvar as credenciais na EEPROM.
 * @param newSSID Novo SSID a ser salvo na EEPROM.
 * @param newPass Nova senha a ser salva na EEPROM.
 */
void saveCredentials(const char* newSSID, const char* newPass) {
  // Limpa a EEPROM
  for (int i = 0; i < eepromSize; ++i) {
    EEPROM.write(i, 0);
  }

  // Salva o novo SSID na EEPROM
  for (int i = 0; i < strlen(newSSID); ++i) {
    EEPROM.write(ssidAddress + i, newSSID[i]);
  }

  // Salva a nova senha na EEPROM
  for (int i = 0; i < strlen(newPass); ++i) {
    EEPROM.write(passAddress + i, newPass[i]);
  }
  
  //Salva efetivamente as alterações na EEPROM
  EEPROM.commit();
}

/**
 * @brief Função para carregar as credenciais da EEPROM.
 * @return Verdadeiro se as credenciais foram carregadas com sucesso, falso caso contrário.
 */
bool loadCredentials() {
  // Carrega o SSID da EEPROM
  for (int i = 0; i < 32; ++i) {
    ssid[i] = EEPROM.read(ssidAddress + i);
  }
  ssid[31] = '\0'; // Adiciona terminador nulo ao final da string

  // Carrega a senha da EEPROM
  for (int i = 0; i < 32; ++i) {
    pass[i] = EEPROM.read(passAddress + i);
  }
  pass[31] = '\0'; // Adiciona terminador nulo ao final da string

  Serial.println("Credenciais carregadas:");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Senha: ");
  Serial.println(pass);
  return (ssid[0] != '\0');
}

/**
 * @brief Função para obter a hora do servidor NTP e imprimir no Serial.
 */
void getTimeFromNTP() {
  timeClient.update();
  Serial.print("Data e Hora: ");
  Serial.println(timeClient.getFormattedTime());
}

/**
 * @brief Função para gerar o conteúdo HTML da página de configuração.
 * @return Uma string contendo o código HTML.
 */
String HTML() {
  String ptr = "<!DOCTYPE html><html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Network Config</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: #9dcdf5}\n";
  ptr += "body{margin-top: 50px;} h1 {color: black;margin: 50px auto 30px;} h3 {color: black;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 180px;background-color: #134688;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #134688;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: black;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "<script>";
  ptr += "function passVisible() { var x = document.getElementById('pass'); if (x.type === 'password') { x.type = 'text' } else { x.type = 'password'; }}";
  ptr += "</script>";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>UNESP<br></h1>\n";
  ptr += "<h2>ESP32 Network Configuration</h2>\n";
  ptr += "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/save\">\n";
  ptr += "<p>Rede:</p><input type=\"text\" id=\"ssid\"  name=\"ssid\">\n";
  ptr += "<p>Senha:</p><input type=\"password\" id=\"pass\"  name=\"pass\">\n";
  ptr += "<p><input type='checkbox' onclick='passVisible()'>Mostrar senha\n";
  ptr += "<p><input class=\"button\" type=\"submit\" value=\"Salvar\">\n";
  ptr += "</form>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

/**
 * @brief Função de configuração do Arduino.
 */
void setup() {
  Serial.begin(115200);
  EEPROM.begin(eepromSize);
  
  if (loadCredentials()) {
    connectToWiFi();
    timeClient.begin();
  } else {
    APConfigMode();
  }
}

/**
 * @brief Função principal do Arduino, executada repetidamente em um loop.
 */
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    getTimeFromNTP();
  } else {
    connectToWiFi();
  }
  delay(10000); // Aguarda 10 segundos
}
