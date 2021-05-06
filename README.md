# is 何
MHRise の護石が表示されているキャプチャ動画から, 護石のリストを作成するプログラムです.

# 使い方
## 護石リストの生成
1. input ディレクトリにゲームのキャプチャ動画を準備する
2. make build run (要 docker)
3. 生成された charm-list.txt を確認する

## スキルシミュレータへの入力
1. cat charm-list.txt | grep info | scripts/generate-input-script.pl > input.js
2. 入力画面で, input.js を実行する (devtools 等)

## 準備するキャプチャ動画について
- switch の機能で撮影した 30 秒キャプチャを想定しています.
- 動画の例は sample/ 内のファイルを参考にしてください.
