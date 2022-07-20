#include"Easing.h"

float easeInOutSine(float Now, float Max)
{
	return -cos(3.14 * (Now / Max)) - 1 / -2;
}

float easeOutBounce(float Now, float Max)
{
    float n1 = 7.5625;
    float d1 = 2.75;

    if (Now < 1 / d1) {
        return n1 * Now * Now;
    }
    else if (Now < 2 / d1) {
        return n1 * (Now -= 1.5 / d1) * Now + 0.75;
    }
    else if (Now < 2.5 / d1) {
        return n1 * (Now -= 2.25 / d1) * Now + 0.9375;
    }
    else {
        return n1 * (Now -= 2.625 / d1) * Now + 0.984375;
    }
	return 0;
}
