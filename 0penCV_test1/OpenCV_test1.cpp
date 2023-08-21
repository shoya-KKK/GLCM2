#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat computeGLCM(const cv::Mat& img, int dx, int dy) {
    int max_val = 256;
    cv::Mat glcm(max_val, max_val, CV_32F, cv::Scalar(0));

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            int px = img.at<uchar>(y, x);
            int py = img.at<uchar>(y + dy, x + dx);
            glcm.at<float>(px, py)++;
        }
    }

    glcm /= (img.rows * img.cols);
    return glcm;
}

void computeTextureFeatures(const cv::Mat& glcm) {
    float contrast = 0;
    float correlation = 0;
    float energy = 0;
    float homogeneity = 0;

    cv::Scalar mean, stddev;
    cv::meanStdDev(glcm, mean, stddev);

    for (int i = 0; i < glcm.rows; i++) {
        for (int j = 0; j < glcm.cols; j++) {
            contrast += glcm.at<float>(i, j) * (i - j) * (i - j);
            correlation += ((i - mean[0]) * (j - mean[0]) * glcm.at<float>(i, j)) / (stddev[0] * stddev[1]);
            energy += std::pow(glcm.at<float>(i, j), 2);
            homogeneity += glcm.at<float>(i, j) / (1.0f + std::abs(i - j));
        }
    }

    std::cout << "Contrast: " << contrast << std::endl;
    std::cout << "Correlation: " << correlation << std::endl;
    std::cout << "Energy: " << energy << std::endl;
    std::cout << "Homogeneity: " << homogeneity << std::endl;
}

int main() {
    cv::Mat img = cv::imread("test5.png", cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    int dx = 1;//•ûŒü‚Æ‹——£‚ð‚±‚Ìx,y‚Ì”’l‚ÅŒˆ’è‚µ‚Ä‚¢‚é
    int dy = 0;//
    cv::Mat glcm = computeGLCM(img, dx, dy);
    computeTextureFeatures(glcm);

    return 0;
}
