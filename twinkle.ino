#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LEDS 28
#define DIPPER_LENGTH 7
#define RND_RANGE 30

#define RED_MASK 0xFF0000
#define GREEN_MASK 0x00FF00
#define BLUE_MASK 0x0000FF

// This defines the Big Dipper gradient:
// #196464 -> #641919 -> #196419
const long colorGrad[] = {0x196464, 0x1E5E5E, 0x245858, 0x2A5252, 0x304C4C, 0x354747, 0x3B4141, 0x413B3B, 0x473535, 0x4C3030, 0x522A2A, 0x582424, 0x5E1E1E, 0x641919, 0x5E1E19, 0x582419, 0x522A19, 0x4C3019, 0x473519, 0x413B19, 0x3B4119, 0x354719, 0x304C19, 0x2A5219, 0x245819, 0x1E5E19, 0x196419, 0x19641E, 0x196424, 0x19642A, 0x196430, 0x196435, 0x19643B, 0x196441, 0x196447, 0x19644C, 0x196452, 0x196458, 0x19645E};
#define NUM_GRADS 39

int greenVals[NUM_LEDS];
int redVals[NUM_LEDS];
int blueVals[NUM_LEDS];
int previousDir[NUM_LEDS];

void loop() {} // We will make our own

void setup()
{
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

    randomSeed(1234);

    for (int i = 0; i < NUM_LEDS - 7; i++) // Set other stars to magenta
    {
        redVals[i] = 50;
        greenVals[i] = 0;
        blueVals[i] = 50;
    }

    for (int i = NUM_LEDS - 7; i < NUM_LEDS; i++) // Set big dipper first color
    {
        redVals[i] = 25;
        greenVals[i] = 100;
        blueVals[i] = 100;
    }
    writeColors(strip, redVals, greenVals, blueVals);

    int gradPos = 0;

    while (1)
    {
        for (int i = 0; i < NUM_LEDS - DIPPER_LENGTH; i++)
        {
            int currentVal = redVals[i];

            switch (random(RND_RANGE))
            {
                case 0:
                    if (currentVal > 2) currentVal -= 1;
                    previousDir[i] = 0;
                    break;
                case (RND_RANGE - 1):
                    if (currentVal < 50) currentVal += 1;
                    previousDir[i] = 1;
                    break;
                default: // continue in the same direction
                    if (previousDir[i])
                    {
                        if (currentVal > 2) currentVal -= 1;
                    }
                    else
                    {
                        if (currentVal < 50) currentVal += 1;
                    }
                    break;
            }

            redVals[i] = currentVal;
            blueVals[i] = currentVal;
        }

        for (int i = NUM_LEDS - DIPPER_LENGTH; i < NUM_LEDS; i++)
        {
            int dipperIndex = i - (NUM_LEDS - DIPPER_LENGTH);

            long color = colorGrad[getArrayIndex(gradPos - dipperIndex)];

            redVals[i] = (color & RED_MASK) >> 16;
            greenVals[i] = (color & GREEN_MASK) >> 8;
            blueVals[i] = (color & BLUE_MASK);
        }

        gradPos++;
        if (gradPos == NUM_GRADS) gradPos = 0;

        writeColors(strip, redVals, greenVals, blueVals);

        delay(40); // Sleep 40 ms (25 Hz)
    }

}

void writeColors(Adafruit_NeoPixel &strip, int *redVals, int *greenVals, int *blueVals)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        strip.setPixelColor(i, redVals[i], greenVals[i], blueVals[i]);    
    }

    strip.show();
}

int getArrayIndex(int index)
{
    if (index < 0)
    {
        return NUM_GRADS + index;
    }
    else
    {
        return index;
    }
}

// vim: set syntax=c autoindent:
