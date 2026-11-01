# wiznet-pico-basics

WIZnet の Raspberry Pi Pico 互換ボード（W55RP20-EVB-Pico / W6300-EVB-Pico2）を LAN につなぐ「最初の一歩」のコードです。
electwork.net の記事で使っているスケッチと PC 側のスクリプトを、そのまま置いています。

| ディレクトリ | 内容 | ボード（Arduino IDE のボード名） |
|:--|:--|:--|
| `arduino/w55rp20-evb-pico/dhcp_tcp_echo/` | DHCP で IP を取り、MAC・IP を表示し、TCP 5000 番でエコーする | WIZnet W55RP20-EVB-Pico（arduino-pico 6.3.1） |
| `arduino/w6300-evb-pico2/dhcp_tcp_echo_ipv6/` | 同じことを W6300 で行い、IPv6 リンクローカルアドレスも表示する | WIZnet W6300-EVB-Pico2（arduino-pico 6.3.1・IP/Bluetooth Stack = IPv4 + IPv6） |
| `tools/tcp_echo_client.ps1` | PC（Windows PowerShell）から TCP 5000 番に 1 行送って、返ってくるまで読む。`.\tcp_echo_client.ps1 192.168.x.x "hello W6300"` のように IP と文字列を引数で渡せる | — |

## 使い方（Arduino IDE）

1. ボードマネージャに arduino-pico を入れる（追加の URL: `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`）
2. ボードで上の名前を選び、スケッチを書き込む（USB をつなぐだけで書けます。書き込み後は COM 番号が変わります）
3. シリアルモニタ（115200）に出た IP に、PC から `ping` と `tools/tcp_echo_client.ps1`（IP と文字列を引数で渡す）

arduino-cli なら `rp2040:rp2040:wiznet_55rp20_evb_pico` / `rp2040:rp2040:wiznet_6300_evb_pico2` です。

## メモ

- `loop()` の `delay(50)` は必要です。arduino-pico の有線 Ethernet は lwIP を 20 ms 周期のポーリングで動かしていて、`client.connected()` などを休みなく呼ぶループではその処理が走らず、相手の切断や ping に応えなくなります（W55RP20-EVB-Pico で実測）
- PC 側は 1 回の `Read()` で全部返るとは限らないので、改行が来るまで読んでいます
- `tools/tcp_echo_client.ps1` は UTF-8（BOM 付き）で保存してあります。BOM なしで保存し直すと Windows PowerShell 5.1 が日本語コメントを cp932 として読み、コメントの次の行が消えて返事が空になります（コメントの末尾を ASCII で終えているので、いまの内容なら BOM なしでも動きます）

## 記事

- [W55RP20-EVB-Pico の基本｜RP2040 と W5500 が 1 チップに入った有線 LAN ボードを、DHCP・ping・TCP エコーまで通す](https://electwork.net/posts/w55rp20-evb-pico-basics-rp2040-w5500-ethernet/)
- [W6300-EVB-Pico2 の基本｜RP2350 と W6300 を QSPI でつないだ有線 LAN ボードを、IPv4 と IPv6 の両方で LAN につなぐ](https://electwork.net/posts/w6300-evb-pico2-basics-rp2350-w6300-qspi-ethernet-ipv6/)
- [Raspberry Pi Pico の C/C++ 開発環境を VS Code 拡張で作る｜Pico SDK 2.3.1 で blink・Hello World・既存プロジェクトのビルドまで](https://electwork.net/posts/pico-sdk-vscode-extension-setup-blink-hello-world/)

## License

MIT
