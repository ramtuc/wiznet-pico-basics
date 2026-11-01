# TCP エコーの確認 (Windows PowerShell 5.1 / PowerShell 7)
# 使い方: .\tcp_echo_client.ps1 192.168.x.x "hello W6300"
# このファイルは UTF-8 (BOM 付き) で保存してある。BOM なしで保存し直すと Windows PowerShell 5.1 が
# 日本語コメントを cp932 として読み、コメントの直後の行が消えて返事が空になる (PowerShell 7 は問題なし)
param(
  [string]$ip = "192.168.x.x",            # シリアルに出た IP (W55RP20 / W6300)
  [string]$text = "hello"                 # 送る文字列 (記事では hello W55RP20 / hello W6300)
)
$c = New-Object System.Net.Sockets.TcpClient($ip, 5000)
$s = $c.GetStream()
$s.ReadTimeout = 3000                     # 3 秒返事がなければエラーで止める (ms)
$msg = [Text.Encoding]::ASCII.GetBytes("$text`n")
$s.Write($msg, 0, $msg.Length)
$buf = New-Object byte[] 64
$got = ""
while ($got -notmatch "`n") {             # 改行が返ってくるまで読み続ける (loop)
  $n = $s.Read($buf, 0, $buf.Length)
  if ($n -le 0) { break }                 # 相手が接続を切ったら終わり (EOF)
  $got += [Text.Encoding]::ASCII.GetString($buf, 0, $n)
}
$got
$c.Close()
