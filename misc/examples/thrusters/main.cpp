#include <murAPI.hpp>

int main() {
    mur.setPortA(-50);
    mur.setPortB(50);
    mur.setPortC(-50);
    mur.setPortD(50);
    sleepFor(3000);
    
    mur.setPorts(50, -50, 50, -50);
    sleepFor(5000);
    
    return 0;
}
