#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>

const char *ssid = "LAPTOP-RI79EFTJ 8859";
const char *pass = ">h16S263";
const char *host = "dweet.io";
const int port = 80; // Puerto por defecto del servicio web

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE); //Construyo un objeto de la clase DHT que representa el sensor

void setup() {
  Serial.begin(115200);
   WiFi.begin(ssid, pass); // Inicializar modulo WIFI y que se vincule a la red indicada en el ssid
  Serial.print("Intentando Conexion");
  Serial.println("Prueba del DHT22!");
   while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Conectado");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());
  dht.begin();
}





void loop() {
  WiFiClient cliente; // Objeto que realiza peticiones a un socket servidor
    if (!cliente.connect(host, port))
  {
    Serial.println("Conexion Fallida al servidor");
    delay(1000);
    return;
  }
 
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();


  String dataToSend = "TEMPERATURA=" + String(temperatura) + "&" +
                      "HUMEDAD=" + String(humedad); 

  String dweetURL = "/dweet/for/cristhian?" + dataToSend;
  String request = String("GET ") + dweetURL + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n";

   cliente.print(request);
  unsigned long inicio = millis();

   while (cliente.available() == 0)
  {
    if (millis() - inicio > 5000)
    {
      Serial.println("Tiempo de espera del servidor agotado");
      cliente.stop();
      return;
    }
  }

    // Si se llega a este punto es porque se recibieron datos del servidor (cliente.available() !=0)
  while (cliente.available())
  {
    String linea = cliente.readStringUntil('\r'); // Lea un string hasta que encuentre el caracter
    Serial.println(linea);
  }
  

  if(isnan(humedad) || isnan(temperatura)){
    Serial.println("Error al leer el sensor DHT22");
    return;
  }
 delay(2000);
  Serial.print("Humedad: ");
  Serial.println(humedad);
  Serial.print("Temperatura: ");
  Serial.println(temperatura);

}


