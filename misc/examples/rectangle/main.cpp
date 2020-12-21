#include <murAPI.hpp>

int main() {

    mur.initCamera(0); //! Front camera
    mur.addDetectorToList(Object::RECTANGLE, 0);

    while (true) {
        for (const auto &obj : mur.getDetectedObjectsList(0)) {
            if (obj.type == Object::RECTANGLE) {
                //! DO SOMETHING WITH RECTANGLE. !//
            }
        }
    } 
}