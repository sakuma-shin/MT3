#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <string.h>
#include <assert.h>
#include "easing.h"
#include"Calculation.h"
#include"Hit.h"

const int kColmunWidth = 60;
const int kColmunHeight = 50;
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

void VectorScreenPrintf(int x, int y, const Vector3& v, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", v.x);
	Novice::ScreenPrintf(x + kColmunWidth, y, "%.02f", v.y);
	Novice::ScreenPrintf(x + kColmunWidth * 2, y, "%.02f", v.z);
	Novice::ScreenPrintf(x + kColmunWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* a) {
	Novice::ScreenPrintf(x, y, "%s", a);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + (column + 1) * kColmunWidth, y + (row + 1) * kColmunHeight, "%0.02f", matrix.m[row][column]);
		}
	}
}

const char kWindowTitle[] = "LE2D_07_サクマ_シン_MT3-1-01";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//クロス積の確認
	Vector3 v1 { 1.2f,-3.9f,2.5f };
	Vector3 v2 { 2.8f,0.4f,-1.3f };

	Vector3 cross = Cross(v1, v2);

	Vector3 rotate{};
	Vector3 translate{};

	Vector3 kLocalVertices[3];
	kLocalVertices[0] = { 0.0f,0.0f,0.0f };
	kLocalVertices[1] = {-0.5f,-0.5f,0.0f};
	kLocalVertices[2] = { 0.5f,-0.5f,0.0f };

	Vector3 cameraPosition = { 0.0f,0.0f,-5.0f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		/// 
		/// 
		///
		rotate.y += 0.05f;
		
		if (keys[DIK_W]) {
			translate.z += 0.3f;
		}

		if (keys[DIK_S]) {
			translate.z -= 0.3f;
		}

		if (keys[DIK_A]) {
			translate.x -= 0.3f;
			
		}

		if (keys[DIK_D]) {
			translate.x += 0.3f;
		}

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix=MakeAffineMatrix({ 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		//WVPMatrixを作る
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		//viewportMatrixを作る
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		//screen空間へと頂点を変換する
		Vector3 screenVertices[3];

		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = TransForm(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = TransForm(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		/// 

		VectorScreenPrintf(0, 0, cross, "cross");

		Novice::ScreenPrintf(30, 30, "screenVertices[0].x:%f", screenVertices[0].x);
		Novice::ScreenPrintf(30, 45, "screenVertices[0].y:%f", screenVertices[0].y);
		Novice::ScreenPrintf(30, 60, "screenVertices[1].x:%f", screenVertices[1].x);
		Novice::ScreenPrintf(30, 75, "screenVertices[1].y:%f", screenVertices[1].y);
		Novice::ScreenPrintf(30, 90, "screenVertices[2].x:%f", screenVertices[2].x);
		Novice::ScreenPrintf(30, 105, "screenVertices[2].y:%f", screenVertices[2].y);

		Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid
		);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
