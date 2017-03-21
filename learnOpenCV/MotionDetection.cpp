////カメラ入力から3フレームを取り出し、差分を計算する。
////変数：thの値を基準として閾値処理(threshold)を行い、変化した部分を白色で描画する。
//
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//
////閾値(threshold)の設定
//static const int threshhold = 3;
//
//int main(void)
//{
//	//カメラを開く
//	cv::VideoCapture    cap(0);
//	//カメラが開けたか確認
//	if (!cap.isOpened()) {
//		cout << "hello" << endl;
//		return -1;
//	}
//
//	cout << "program starts" << endl;
//
//	//変数の準備
//	cv::Mat im1, im2, im3, frame;
//	cv::Mat d1, d2, diff;
//	cv::Mat im_mask, mask;
//	//ウィンドウを2枚生成
//	cv::namedWindow("in");
//	cv::namedWindow("out");
//	//カメラから3フレーム取り出す
//	cap >> frame;
//	cv::cvtColor(frame, im1, CV_RGB2GRAY);
//	cap >> frame;
//	cv::cvtColor(frame, im2, CV_RGB2GRAY);
//	cap >> frame;
//	cv::cvtColor(frame, im3, CV_RGB2GRAY);
//
//	while (1)
//	{
//		//差分1：フレーム1と2の差を求める
//		cv::absdiff(im1, im2, d1);
//		//差分2：フレーム2と3の差を求める
//		cv::absdiff(im2, im3, d2);
//		//差分1と差分2の結果を比較(論理積)し、diffに出力
//		cv::bitwise_and(d1, d2, diff);
//
//		//差分diffのうち、閾値thを超えている部分を1、それ以外を0としてmaskに出力
//		cv::threshold(diff, mask, threshhold , 1, cv::THRESH_BINARY);
//		//マスクmaskのうち、1(True)の部分を白(0)に、0(False)の部分を黒(255)にしてim_maskに出力
//		cv::threshold(mask, im_mask, 0, 255, cv::THRESH_BINARY);
//		//メディアンフィルタを使った平滑化によってゴマ塩ノイズを除去、アパーチャサイズ5
//		cv::medianBlur(im_mask, im_mask, 5);
//		//ウィンドウ2枚にそれぞれ入力画像、差分画像を表示
//		cv::imshow("in", frame);
//		cv::imshow("out", im_mask);
//
//		//新しいフレームをカメラから一つ取り出し、3つのフレームを全てずらす
//		im2.copyTo(im1, im2);
//		im3.copyTo(im2, im3);
//		cap >> frame;
//		cv::cvtColor(frame, im3, CV_RGB2GRAY);
//
//		//ウィンドウ上でEscキーが押されたらプログラム終了
//		if (cv::waitKey(27) >= 0)
//		{
//			cv::destroyAllWindows();
//			break;
//		}
//	}
//
//	cap.release();
//	return EXIT_SUCCESS;
//}