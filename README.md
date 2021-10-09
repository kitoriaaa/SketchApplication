# Sketch Application
C++, OpenSiv3Dによるスケッチアプリケーション。ここで描画した画像を、[SketchModelingServer](https://github.com/kitoriaaa/SketchModelingServer)にて3Dに変換することを想定して作成した。
最終的に作成したモデルは[SketchViewHoloLens](https://github.com/kitoriaaa/SketchViewHoloLens)でMicrosoft HoloLensで見ることができる。

## 機能
- ペン入力
- 消しゴム
- 直線モード
	- 2点を指定して ```DrawLine``` ボタンを押すと2点間の直線を引く
- キャンパス削除
- 画像として保存・読み込み
