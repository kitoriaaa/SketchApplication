# include <Siv3D.hpp>

void Main()
{
    // キャンバスのサイズ
    constexpr Size canvasSize(1024, 1024);

    // ペンの太さ
    int32 thickness = 2;

    // ペンの色
    Color penColor = Palette::Black;

    // Indicate Eraser switch on/off
    bool canErase = false;

    // 書き込み用の画像データを用意
    Image image(canvasSize, Palette::White);

    // 表示用のテクスチャ（内容を更新するので DynamicTexture）
    DynamicTexture texture(image);

    // Window size
    Window::SetStyle(WindowStyle::Sizable);
    Scene::SetScaleMode(ScaleMode::ResizeFill);

    while (System::Update())
    {
        if (MouseL.pressed())
        {
            // 書き込む線の始点は直前のフレームのマウスカーソル座標
            // （初回はタッチ操作時の座標のジャンプを防ぐため、現在のマウスカーソル座標にする）
            const Point from = MouseL.down() ? Cursor::Pos() : Cursor::PreviousPos();

            // 書き込む線の終点は現在のマウスカーソル座標
            const Point to = Cursor::Pos();

            // image に線を書き込む
            Line(from, to).overwrite(image, thickness, penColor);

            // 書き込み終わった image でテクスチャを更新
            texture.fill(image);
        }

        // 描いたものを消去するボタンが押されたら
        if (SimpleGUI::Button(U"Clear", Vec2(1050, 40), 120))
        {
            // 画像を白で塗りつぶす
            image.fill(Palette::White);

            // 塗りつぶし終わった image でテクスチャを更新
            texture.fill(image);
        }

        // Save button
        if (SimpleGUI::Button(U"Save", Vec2(1050, 90), 120))
        {
            const auto save = Dialog::SaveImage();
            if (save)
            {
                image.save(save.value());
            }
        }

        // Eraser switch
        if (SimpleGUI::CheckBox(canErase, U"Eraser", Vec2(1050, 140)))
        {
            if (Palette::Black == penColor)
            {
                penColor = Palette::White;
                thickness = 5;
            }
            else
            {
                penColor = Palette::Black;
                thickness = 2;
            }
        }

        // テクスチャを表示
        texture.draw();
    }
}

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D ユーザコミュニティ Slack への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
