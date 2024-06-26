#include "button.h"

Button::Button(const int pin)
{
    this->pin = pin;
    this->state = ButtonState::Idle;

    pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed()
{
    return digitalRead(this->pin) == LOW;
}

void Button::setLongPressTime(unsigned int time)
{
    this->longPressTime = time;
}

void Button::onClick(callback cb)
{
    this->onClickCallback = cb;
}

void Button::onLongPress(callback cb)
{
    this->onLongPressCallback = cb;
}

void Button::onLongPressRepeat(callback cb)
{
    this->onLongPressRepeatCallback = cb;
}

void Button::onLongPressRelease(callback cb)
{
    this->onLongPressReleaseCallback = cb;
}

void Button::update()
{
    bool pressed = digitalRead(this->pin) == LOW;

    long now = millis();
    long diff = now - this->lastChange;

    if (diff < this->debounceTime)
        return;

    if (state == ButtonState::Idle && pressed)
    {
        this->state = ButtonState::Pressed;
        this->lastChange = now;
        this->longPressRepeatTime = 100;
    }
    else if (state == ButtonState::Pressed && !pressed)
    {
        this->state = ButtonState::Idle;
        if (diff < this->longPressTime)
        {
            if (this->onClickCallback != nullptr)
                this->onClickCallback();
        }
        else
        {
            longPress = false;
            if (this->onLongPressReleaseCallback != nullptr)
                this->onLongPressReleaseCallback();
        }
        this->lastChange = now;
    }
    else if (state == ButtonState::Pressed && pressed && diff > this->longPressTime)
    {
        if (this->onLongPressRepeatCallback != nullptr)
        {
            if (now - this->lastRepeat > this->longPressRepeatTime)
            {
                this->onLongPressRepeatCallback();
                this->lastRepeat = now;

                if (this->repeatCount >= 5 && this->longPressRepeatTime > 50)
                {
                    this->longPressRepeatTime -= 0.1 * this->longPressRepeatTime;

                    if (this->longPressRepeatTime < 50)
                    {
                        this->longPressRepeatTime = 50;
                    }
                }
            }
        }
        if (this->onLongPressCallback != nullptr)
        {
            if (!longPress)
            {
                longPress = true;
                this->onLongPressCallback();
            }
        }
    }
}