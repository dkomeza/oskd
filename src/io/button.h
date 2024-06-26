#pragma once

#include <Arduino.h>

typedef void (*callback)(void);

enum class ButtonState
{
    Initial,
    Idle,
    Pressed,
};

class Button
{
public:
    Button(const int pin);

    void setLongPressTime(unsigned int time);

    void onClick(callback cb);
    void onLongPress(callback cb);
    void onLongPressRepeat(callback cb);
    void onLongPressRelease(callback cb);

    void update();

    bool isPressed();

private:
    int pin;
    ButtonState state;

    callback onClickCallback;
    callback onLongPressCallback;
    callback onLongPressRepeatCallback;
    callback onLongPressReleaseCallback;

    unsigned int debounceTime = 50;
    unsigned int longPressTime = 1000;
    bool longPress = false;
    unsigned int longPressRepeatTime = 1000;

    unsigned long lastChange;
    unsigned long lastRepeat;
    unsigned long repeatCount;
};