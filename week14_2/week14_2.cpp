// week14.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int createMaskByKmeans(cv::Mat src, cv::Mat& mask)
{
	if ((mask.type() != CV_8UC1)
		|| (src.size() != mask.size())
		) {
		return 0;
	}

	int width = src.cols;
	int height = src.rows;

	int pixNum = width * height;
	int clusterCount = 2;
	Mat labels;
	Mat centers;

	//制作kmeans用的数据
	Mat sampleData = src.reshape(3, pixNum);
	Mat km_data;
	sampleData.convertTo(km_data, CV_32F);

	//执行kmeans
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1);
	kmeans(km_data, clusterCount, labels, criteria, clusterCount, KMEANS_PP_CENTERS, centers);

	//制作mask
	uchar fg[2] = { 0,255 };
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			mask.at<uchar>(row, col) = fg[labels.at<int>(row * width + col)];

		}
	}

	//初始随机生成的质心，导致mask黑白闪
	if (mask.at<uchar>(0, 0) == 255)
	{//反转整个图像黑白
		mask = 255 - mask;
	}

	return 0;
}

void segColor()
{

	Mat src = imread("C:\\Users\\27318\\Desktop\\大二下网络课程\\数字图像\\week14\\green.jpg");

	Mat mask = Mat::zeros(src.size(), CV_8UC1);
	createMaskByKmeans(src, mask);

	imshow("src", src);
	imshow("mask", mask);

	waitKey(0);

}

int main()
{
	VideoCapture capture1("C:\\Users\\27318\\Desktop\\大二下网络课程\\数字图像\\week14\\pikachu.mp4");
	VideoCapture capture2("C:\\Users\\27318\\Desktop\\大二下网络课程\\数字图像\\week14\\lizhu.mp4");
	Mat frame1, frame2;
	while (1)
	{
		capture1 >> frame1;
		capture2 >> frame2;

		Mat src1 = frame1;
		resize(frame2, frame2, frame1.size());//前景和背景大小设置相同

		Mat src2 = frame2;

		Mat mask = Mat::zeros(src1.size(), CV_8UC1);
		createMaskByKmeans(src1, mask);
		src1.copyTo(src2, mask);
		imshow("src2", src2);
		//imshow("src1", src1);
		//imshow("mask", mask);

		waitKey(10);

	}
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
