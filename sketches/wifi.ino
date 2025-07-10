#include <WiFi.h>
#include <WiFiClientSecure.h>

// Wifi setup
char ssid[] = "your_ssid_here";
char pass[] = "your_password_here";
int status = WL_IDLE_STATUS;

#define SERVER "app.firehydrant.io"
#define PATH "your_hackermode_path_here"

WiFiClientSecure client;

void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);

  // attempt to connect to Wifi network:
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to WiFi");

  client.setInsecure(); // don't use a root cert
  delay(1000);
}

String getData()
{
  String response = "";
  if (!client.connect(SERVER, 443))
  {
    Serial.println("Connection failed.");
  }

  Serial.println("Connected !!");

  // Make HTTP request:
  client.println("GET " PATH " HTTP/1.1");
  client.println("Host: " SERVER);
  client.println("Connection: close");
  if (client.println() == 0)
  {
    Serial.println("Failed to send request");
    delay(100);
    client.stop();
    return response;
  }

  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status + 9, "200 OK") != 0)
  {
    Serial.print("Unexpected response: ");
    Serial.println(status);
    delay(100);
    client.stop();
    return response;
  }

  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders))
  {
    Serial.println("Invalid response");
    delay(100);
    client.stop();
    return response;
  }

  while (client.available())
  {
    response = client.readString();
    Serial.println(response);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
  }

  return response;
}

void loop()
{
  String output = getData();
  if (output == "has_active_alerts")
  {
    digitalWrite(3, HIGH);
  }
  else
  {
    digitalWrite(3, LOW);
  }
  delay(5000);
}