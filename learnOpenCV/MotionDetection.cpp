////�J�������͂���3�t���[�������o���A�������v�Z����B
////�ϐ��Fth�̒l����Ƃ���臒l����(threshold)���s���A�ω����������𔒐F�ŕ`�悷��B
//
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//
////臒l(threshold)�̐ݒ�
//static const int threshhold = 3;
//
//int main(void)
//{
//	//�J�������J��
//	cv::VideoCapture    cap(0);
//	//�J�������J�������m�F
//	if (!cap.isOpened()) {
//		cout << "hello" << endl;
//		return -1;
//	}
//
//	cout << "program starts" << endl;
//
//	//�ϐ��̏���
//	cv::Mat im1, im2, im3, frame;
//	cv::Mat d1, d2, diff;
//	cv::Mat im_mask, mask;
//	//�E�B���h�E��2������
//	cv::namedWindow("in");
//	cv::namedWindow("out");
//	//�J��������3�t���[�����o��
//	cap >> frame;
//	cv::cvtColor(frame, im1, CV_RGB2GRAY);
//	cap >> frame;
//	cv::cvtColor(frame, im2, CV_RGB2GRAY);
//	cap >> frame;
//	cv::cvtColor(frame, im3, CV_RGB2GRAY);
//
//	while (1)
//	{
//		//����1�F�t���[��1��2�̍������߂�
//		cv::absdiff(im1, im2, d1);
//		//����2�F�t���[��2��3�̍������߂�
//		cv::absdiff(im2, im3, d2);
//		//����1�ƍ���2�̌��ʂ��r(�_����)���Adiff�ɏo��
//		cv::bitwise_and(d1, d2, diff);
//
//		//����diff�̂����A臒lth�𒴂��Ă��镔����1�A����ȊO��0�Ƃ���mask�ɏo��
//		cv::threshold(diff, mask, threshhold , 1, cv::THRESH_BINARY);
//		//�}�X�Nmask�̂����A1(True)�̕�����(0)�ɁA0(False)�̕�������(255)�ɂ���im_mask�ɏo��
//		cv::threshold(mask, im_mask, 0, 255, cv::THRESH_BINARY);
//		//���f�B�A���t�B���^���g�����������ɂ���ăS�}���m�C�Y�������A�A�p�[�`���T�C�Y5
//		cv::medianBlur(im_mask, im_mask, 5);
//		//�E�B���h�E2���ɂ��ꂼ����͉摜�A�����摜��\��
//		cv::imshow("in", frame);
//		cv::imshow("out", im_mask);
//
//		//�V�����t���[�����J�����������o���A3�̃t���[����S�Ă��炷
//		im2.copyTo(im1, im2);
//		im3.copyTo(im2, im3);
//		cap >> frame;
//		cv::cvtColor(frame, im3, CV_RGB2GRAY);
//
//		//�E�B���h�E���Esc�L�[�������ꂽ��v���O�����I��
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