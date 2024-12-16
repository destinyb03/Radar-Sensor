#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "ESP8266_Radar";
const char* password = "12345678";

// Start server on port 80
WiFiServer server(80);

void setup() {
  Serial.begin(9600); // Communication for debugging
  WiFi.softAP(ssid, password); // Set up Access Point
  Serial.println("Wi-Fi Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP()); // Print AP IP address

  server.begin(); // Start the server
}

void loop() {
  WiFiClient client = server.available(); // Check for incoming clients
  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r'); // Read client request
    Serial.println(request);
    client.flush(); // Clear input buffer

    // Generate radar data dynamically
    String radarData = "";
    for (int angle = 15; angle <= 165; angle += 15) { // Show data at 15° intervals
      int distance = random(10, 400); // Simulated distance
      radarData += "<tr><td>" + String(angle) + "°</td><td>" + String(distance) + " cm</td></tr>";
    }

    // Serve the HTML page with radar data
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<title>ESP8266 Radar</title>");
    client.println("<meta http-equiv='refresh' content='2'>"); // Auto-refresh every 2 seconds
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; background-color: #f0f0f0; }");
    client.println("h1 { color: #333; margin: 20px 0; }");
    client.println("table { margin: 0 auto; border-collapse: collapse; width: 60%; }");
    client.println("th, td { padding: 10px; border: 1px solid #ddd; text-align: center; }");
    client.println("th { background-color: #4CAF50; color: white; }");
    client.println("td { background-color: #f9f9f9; }");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1>ESP8266 Radar Data</h1>");
    client.println("<table>");
    client.println("<tr><th>Angle</th><th>Distance</th></tr>");
    client.println(radarData);
    client.println("</table>");
    client.println("<p>Data updates every 2 seconds.</p>");
    client.println("</body>");
    client.println("</html>");
    client.stop(); // Disconnect client
    Serial.println("Client disconnected");
  }
}
