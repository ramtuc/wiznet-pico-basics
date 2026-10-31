$ip = "192.168.x.x"   # ← シリアルに出た IP に書き換える
$c = New-Object System.Net.Sockets.TcpClient($ip, 5000)
$s = $c.GetStream()
$s.ReadTimeout = 3000                     # 3 秒返事がなければエラーで止める
$msg = [Text.Encoding]::ASCII.GetBytes("hello W55RP20`n")
$s.Write($msg, 0, $msg.Length)
$buf = New-Object byte[] 64
$got = ""
while ($got -notmatch "`n") {             # 改行が返ってくるまで読み続ける
  $n = $s.Read($buf, 0, $buf.Length)
  if ($n -le 0) { break }                 # 相手が接続を切った
  $got += [Text.Encoding]::ASCII.GetString($buf, 0, $n)
}
$got
$c.Close()
