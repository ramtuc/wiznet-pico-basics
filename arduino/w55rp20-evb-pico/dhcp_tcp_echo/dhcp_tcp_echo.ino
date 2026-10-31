// W55RP20-EVB-Pico: DHCP で IP を取り、TCP 5000 番でエコーする（arduino-pico 6.3.1）
#include <W55RP20lwIP.h>

Wiznet55rp20lwIP eth(20);   // CS = GPIO20（基板の内部配線）
WiFiServer server(5000);    // TCP エコー用のポート

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {}  // USB シリアルを開くまで最大 5 秒待つ

  if (!eth.begin()) {                    // MAC 省略 = チップ固有 ID から生成
    Serial.println("W5500 not found");
    while (true) delay(1000);
  }
  Serial.print("DHCP");
  while (!eth.connected()) {             // IP が付くまで待つ
    Serial.print('.');
    delay(500);
  }
  Serial.println();

  uint8_t mac[6];
  eth.macAddress(mac);
  Serial.printf("MAC : %02X:%02X:%02X:%02X:%02X:%02X\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("IP  : "); Serial.println(eth.localIP());
  Serial.print("MASK: "); Serial.println(eth.subnetMask());
  Serial.print("GW  : "); Serial.println(eth.gatewayIP());

  server.begin();
  Serial.println("TCP echo server on port 5000");
}

void loop() {
  WiFiClient client = server.accept();
  if (!client) return;
  Serial.print("client: "); Serial.println(client.remoteIP());
  while (client.connected()) {
    int n = client.available();
    if (n > 0) {
      uint8_t buf[128];
      n = client.read(buf, min(n, (int)sizeof(buf)));
      client.write(buf, n);              // 受け取った分をまとめて返す
    }
    delay(50);                           // lwIP のポーリング（既定 20 ms）が走る間を空ける
  }
  client.stop();
  Serial.println("closed");
}
