#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Streamer.h"

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "Plug a camera in" << std::endl;
        exit(-1);
    }
    // Set the ip to what ever the ip of your driverstation is (make sure that it's static)
    // If I recall correctly FRC gives you ports 5800-5810
    Streamer streamer("targetIP", 5803);
    cv::Mat frame;
    // The for loop will cry forever
    for (;;) {
        cap >> frame;
        // I also recommend downsizing the image, then upscaling on the viewer end so as to not exceed data limits
        // Change this as you like
        cv::resize(frame, frame, cv::Size(320, 240));
        streamer.sendFrame(frame);
        // This is A. so my ide doesn't yell at me for an infinite loop
        // B if you hit the ESC key after uncommenting the next line you will be able to exit the program
        //cv::imshow("FRAME", frame);
        if (cv::waitKey(1) == 27)
            break;
    }
    streamer.close();
    return 1;
}