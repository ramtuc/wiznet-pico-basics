// W6300-EVB-Pico2: DHCP で IPv4 を取り、IPv6 のアドレスも表示して、TCP 5000 番でエコーする（arduino-pico 6.3.1）
// ツール → IP/Bluetooth Stack を「IPv4 + IPv6」にすると IPv6 が有効になる
#include <W6300lwIP.h>
#include <lwip/netif.h>

Wiznet6300lwIP eth(16);     // CS = GPIO16（基板の内部配線）
WiFiServer server(5000);    // TCP エコー用のポート

void printIPv6() {
#if LWIP_IPV6
  netif *n = eth.getNetIf();
  for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
    u8_t st = netif_ip6_addr_state(n, i);
    if (ip6_addr_isinvalid(st)) continue;  // 空きスロットは飛ばす
    Serial.printf("IPv6: %s (%s)\n", ip6addr_ntoa(netif_ip6_addr(n, i)),
                  ip6_addr_ispreferred(st) ? "preferred" : "tentative");
  }
#else
  Serial.println("IPv6: off (Tools > IP/Bluetooth Stack > IPv4 + IPv6)");
#endif
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {}  // USB シリアルを開くまで最大 5 秒待つ

  if (!eth.begin()) {                    // MAC 省略 = チップ固有 ID から生成
    Serial.println("W6300 not found");
    while (true) delay(1000);
  }
  Serial.print("DHCP");
  while (!eth.connected()) {             // IPv4 が付くまで待つ
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

  delay(3000);                           // IPv6 の重複確認（DAD）が終わるのを待つ
  printIPv6();

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
