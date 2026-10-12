#include <WiFi.h>

// ======================================================
// CONFIGURACIÓN WIFI
// ======================================================

const char* SSID = "WIFI4433";
const char* PASSWORD = "jldp1991";


// ======================================================
// VARIABLES DE CONTROL
// ======================================================

unsigned long inicioDesconexion = 0;
unsigned long tiempoUltimaDesconexion = 0;
unsigned long tiempoUltimaReconexion = 0;

unsigned long cantidadReconexiones = 0;

bool estabaConectado = false;
bool desconectado = false;

// Intervalo entre intentos de reconexión
const unsigned long INTERVALO_RECONEXION = 5000;

// Tiempo del último intento
unsigned long ultimoIntento = 0;


// ======================================================
// FUNCIÓN DE CONEXIÓN INICIAL
// ======================================================

void conectarWiFi() {

  Serial.println();
  Serial.println("======================================");
  Serial.println("       CONEXIÓN WIFI - ESP32");
  Serial.println("======================================");

  Serial.print("Conectando a: ");
  Serial.println(SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  unsigned long inicio = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - inicio < 20000) {

    Serial.print(".");
    delay(500);
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("WiFi conectado correctamente");

    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    estabaConectado = true;

  } else {

    Serial.println("No se pudo conectar al WiFi.");

    estabaConectado = false;
  }
}


// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("======================================");
  Serial.println("   F.3 - RECONEXIÓN WIFI ESP32");
  Serial.println("======================================");

  conectarWiFi();
}


// ======================================================
// LOOP
// ======================================================

void loop() {

  unsigned long ahora = millis();

  // ====================================================
  // WIFI CONECTADO
  // ====================================================

  if (WiFi.status() == WL_CONNECTED) {

    // Detectamos que acabamos de recuperar conexión
    if (desconectado) {

      tiempoUltimaReconexion = ahora - inicioDesconexion;

      cantidadReconexiones++;

      Serial.println();
      Serial.println("**************************************");
      Serial.println("        WIFI RECONectado");
      Serial.println("**************************************");

      Serial.print("Cantidad de reconexiones: ");
      Serial.println(cantidadReconexiones);

      Serial.print("Tiempo de pérdida: ");
      Serial.print(tiempoUltimaReconexion);
      Serial.println(" ms");

      Serial.print("Tiempo de pérdida: ");
      Serial.print(tiempoUltimaReconexion / 1000.0);
      Serial.println(" segundos");

      Serial.print("IP actual: ");
      Serial.println(WiFi.localIP());

      Serial.print("RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");

      Serial.println("**************************************");
      Serial.println();

      desconectado = false;
      estabaConectado = true;
    }

    // Mostrar estado cada cierto tiempo
    static unsigned long ultimoReporte = 0;

    if (ahora - ultimoReporte >= 10000) {

      ultimoReporte = ahora;

      Serial.print("[WIFI OK] IP: ");
      Serial.print(WiFi.localIP());

      Serial.print(" | RSSI: ");
      Serial.print(WiFi.RSSI());

      Serial.println(" dBm");
    }
  }


  // ====================================================
  // WIFI DESCONECTADO
  // ====================================================

  else {

    // Primera detección de la pérdida
    if (!desconectado) {

      inicioDesconexion = ahora;

      desconectado = true;
      estabaConectado = false;

      Serial.println();
      Serial.println("######################################");
      Serial.println("       ¡WIFI DESCONECTADO!");
      Serial.println("######################################");

      Serial.println("Se detectó una pérdida de conexión.");
      Serial.println("Iniciando proceso de reconexión...");

      Serial.print("Estado WiFi: ");
      Serial.println(WiFi.status());

      Serial.println("######################################");
      Serial.println();
    }


    // ==================================================
    // INTENTO DE RECONEXIÓN
    // ==================================================

    if (ahora - ultimoIntento >= INTERVALO_RECONEXION) {

      ultimoIntento = ahora;

      Serial.println("--------------------------------------");
      Serial.println("Intentando reconectar...");

      WiFi.disconnect();
      WiFi.begin(SSID, PASSWORD);

      Serial.println("Solicitud de reconexión enviada.");
      Serial.println("--------------------------------------");
    }
  }

  delay(100);
}