#include <murAPI.hpp>

int main() {

    mur.initCamera(0);
    mur.addDetectorToList(Object::RECTANGLE, 0);

    while (true) {
        for (const auto &obj : mur.getDetectedObjectsList(0)) {
            cv::Mat image = mur.getCameraOneFrame();
            if (obj.type == Object::RECTANGLE) {
                if (obj.r > 10) {
                    cv::circle(image, cv::Point2i(obj.x, obj.y), obj.r, cv::Scalar(255, 0, 100), 8);
                }
            }
            cv::imshow("Image", image);
            cv::waitKey(10);
        }
    } 
}