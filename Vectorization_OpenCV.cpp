﻿
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat lines;
    vector<Vec3d> line3d;
    vector<Point2f> point;

    // Set of 27 points from 1.txt
    const static float Points[27][2] = {
    { 8.04411f, 78.9279f }, { 8.89507f, 79.151f }, { 9.75144f, 79.5972f }, { 11.0208f, 79.4857f },
    { 11.4095f, 78.0355f }, { 11.8096f, 77.1432f }, { 12.2081f, 76.2508f }, { 12.4008f, 75.5815f },
    { 11.5546f, 75.3584f }, { 10.2843f, 74.9122f }, { 9.43571f, 74.466f }, { 8.1698f, 74.0198f },
    { 7.32486f, 73.5736f }, { 6.48152f, 73.1274f }, { 5.22202f, 72.6812f }, { 4.38229f, 72.235f },
    { 2.71349f, 72.3466f }, { 2.301f, 73.3505f }, { 1.88804f, 74.6891f }, { 1.47164f, 75.693f },
    { 1.68387f, 76.2508f }, { 2.94818f, 76.4739f }, { 3.79413f, 76.9201f }, { 4.64168f, 77.3663f },
    { 5.91321f, 77.8125f }, { 6.76436f, 78.2586f }, { 8.0422f, 78.8164f }
    };

    for (int i = 0; i < 27; i++)
    {
        point.push_back(Point2f(Points[i][0], Points[i][1]));
    }

    // MAIN CALL of Hough
    // rho -> shortest distance from the origin (0,0) to the line
    // theta -> angle between X-axis and a normal to the line
    double rhoMin = 0.0f, rhoMax = 200.0f, rhoStep = 0.1f;
    double thetaMin = 0.0f, thetaMax = CV_PI * 2.0f, thetaStep = CV_PI / 720.0f; // step = pi/720 = (pi/180) / 4 ~~ 0.25 degrees
    HoughLinesPointSet(point, lines, 100, 1,
        rhoMin, rhoMax, rhoStep,
        thetaMin, thetaMax, thetaStep);

    lines.copyTo(line3d);

    // basic console output to rank all the lines by their votes
    for (int i = 0; i < line3d.size(); i++) {
        printf("votes:%d, rho:%.7f, theta:%.7f\n", (int)line3d.at(i).val[0], line3d.at(i).val[1], line3d.at(i).val[2]);
    }
    

    // Plotting the lines
    char hough_window[] = "1.txt through Hough";

    Mat hough_image = Mat::zeros(700, 700, CV_8UC3);  // basic 700x700 image

    for (size_t i = 0; i < line3d.size(); i++)
    {
        float rho = line3d.at(i).val[1], theta = line3d.at(i).val[2];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 300 * (-b) + 300); // сдвигаем точки на 300 пикселей для центрирования наших пучков
        pt1.y = cvRound(y0 + 300 * (a) + 300);
        pt2.x = cvRound(x0 - 300 * (-b) + 300);
        pt2.y = cvRound(y0 - 300 * (a) + 300);
        line(hough_image, pt1, pt2, Scalar(0, 0, 40 * ((int)line3d.at(i).val[0] - 1)), 1, LINE_AA);
        
    }

    printf("Number of lines: %d \n", line3d.size());

    imshow(hough_window, hough_image);
    moveWindow(hough_window, 0, 200);
    waitKey(0);

    return 0;
}