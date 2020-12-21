#include <murAPI.hpp>

int main() {
    Timer t;
    t.start();

    const int toSet = 30;
    const int kP = 7;

    while (t.elapsed() < 20000) {
        int depth = mur.getInputB();
        int power = (depth - toSet) * kP * (-1);
        mur.setPortA(power);
    }
}