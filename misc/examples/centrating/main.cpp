#include <murAPI.hpp>

int main() {

    mur.initCamera(0); //! Front camera

    mur.addDetectorToList(Object::RECTANGLE, 0);
    Timer timer;
    timer.start();
    const int indent = 40;
    const int hCenter = 320 / 2;
    const int vCenter = 240 / 2;
    while (timer.elapsed() < 50000) {
        for (const auto &obj : mur.getDetectedObjectsList(0)) {
            if (obj.type == Object::RECTANGLE) {

                int powerA = 0;
                int powerB = 0;
                int powerC = 0;
                if (obj.x < (hCenter - indent)) {
                    powerA = 50;
                    powerB = 0;
                }
                if (obj.x > (hCenter + indent)) {
                    powerB = 50;
                    powerA = 0;
                }
                if ((obj.x > (hCenter - indent)) && (obj.x < (hCenter + indent))) {
                    powerA = 0;
                    powerB = 0;
                }
                if (obj.y < (vCenter - indent)) { 
                    powerC = 50;
                }
                if (obj.y > (vCenter + indent)) { 
                    powerC = -50;
                }
                if ((obj.y > (vCenter - indent)) && (obj.y < (vCenter + indent))) {
                    powerC = 0;
                }
                mur.setPortD(powerA);
                mur.setPortC(powerB);
                mur.setPortB(powerC);
            }
        }
    } 
}