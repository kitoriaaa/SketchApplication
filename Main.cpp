# include <Siv3D.hpp>
# include <WinSock2.h>
# include <WS2tcpip.h>
# include <windows.h>


#pragma comment(lib, "ws2_32.lib")

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
    bool lineMode = false;

    // 書き込み用の画像データを用意
    Image image(canvasSize, Palette::White);

    // 表示用のテクスチャ（内容を更新するので DynamicTexture）
    DynamicTexture texture(image);

    // Window size
    Window::SetStyle(WindowStyle::Sizable);
    //Scene::SetScaleMode(ScaleMode::ResizeFill);
    Scene::Resize(1500, 1024);

    constexpr int buttonSize = 140;
    constexpr int buttonPosX = 1050;

    Point* startPoint = nullptr;
    Point* endPoint = nullptr;
    bool isTouchedStart = false;


    while (System::Update())
    {
        if (MouseL.pressed() && !lineMode)
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
        if (SimpleGUI::Button(U"Clear", Vec2(buttonPosX+2*(buttonSize+10), 100), buttonSize))
        {
            // 画像を白で塗りつぶす
            image.fill(Palette::White);

            // 塗りつぶし終わった image でテクスチャを更新
            texture.fill(image);
        }

        // Save button
        if (SimpleGUI::Button(U"Save", Vec2(buttonPosX+buttonSize+10, 100), buttonSize))
        {
            const auto save = Dialog::SaveImage();
            if (save)
            {
                image.save(save.value());
            }
        }

        // Eraser switch
        if (SimpleGUI::CheckBox(canErase, U"Eraser", Vec2(buttonPosX, 100), buttonSize))
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
            lineMode = false;
        }

        // Load sketch (Currently, it only supports 1024x1024)
        if (SimpleGUI::Button(U"Load Image", Vec2(buttonPosX+buttonSize+10, 150), buttonSize))
        {
            auto load = Dialog::OpenImage();
            if (load)
            {
                image = std::move(load);
                texture.fill(image);
            }
        }

        // Create 3D object. Sketch images send Flask CNN server
        /*if (SimpleGUI::Button(U"Create Object", Vec2(1050, 240), buttonSize))
        {
            WSADATA wsaData;
            SOCKET s;

            int err = WSAStartup(MAKEWORD(2, 0), &wsaData);
            if (err != 0)
            {
                switch (err) {
                case WSASYSNOTREADY:
                    std::cout << "WSASYSNOTREADY" << std::endl;
                    break;
                case WSAVERNOTSUPPORTED:
                    std::cout << "WSAVERNOTSUPPORTED" << std::endl;
                    break;
                case WSAEINPROGRESS:
                    std::cout << "WSAEINPROGRESS" << std::endl;
                    break;
                case WSAEPROCLIM:
                    std::cout << "WSAEPROCLIM" << std::endl;
                    break;
                case WSAEFAULT:
                    std::cout << "WSAEFAULT" << std::endl;
                    break;
                }
            }

            struct sockaddr_in addr;

            WSACleanup();
        }*/

        // Change drawing mode
        if (SimpleGUI::CheckBox(lineMode, U"LineMode", Vec2(buttonPosX, 150), buttonSize)) {
            startPoint = endPoint = nullptr;
            isTouchedStart = false;
        }

        // 2点間の直線を結ぶ
        if (SimpleGUI::Button(U"Draw line", Vec2(buttonPosX+10, 200), buttonSize-20)&& startPoint != nullptr && endPoint != nullptr) {
            Line(*startPoint, *endPoint).overwrite(image, thickness, penColor);
            startPoint = endPoint = nullptr;
        }


        // 直線を結ぶ2点を取得する
        if (lineMode)
        {
            // 1つ目の点
            if (MouseL.down() && isTouchedStart == false) {
                Point from = Cursor::Pos();
                // 押された点がキャンバス内かチェック
                if ((0 <= from.x && from.x <= canvasSize.x) && (0 <= from.y && from.y <= canvasSize.y)) {
                    startPoint = &from;
                    Line(*startPoint, *startPoint).overwrite(image, thickness, penColor);
                    isTouchedStart = true;
                    //Print << U"start pint";
                    //Print << *startPoint;
                }
                // 同一フレーム内でendPointを取得しないようにcontinue
                texture.fill(image);
                texture.draw();
                continue;
            }

            // 2つ目の点
            if (MouseL.down() && isTouchedStart == true) {
                Point to = Cursor::Pos();
                if ((0 <= to.x && to.x <= canvasSize.x) && (0 <= to.y && to.y <= canvasSize.y)) {
                    endPoint = &to;
                    isTouchedStart = false;
                    Line(*endPoint, *endPoint).overwrite(image, thickness, penColor);
                    //Print << U"end point";
                    //Print << *endPoint;
                }
            }
            texture.fill(image);
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
