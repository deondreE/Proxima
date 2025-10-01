#include "UI/Slider.hpp"
#include <math.h>
#include <iostream>
#include <sstream>
#include <cassert>
#include <iomanip>

namespace UI {

Slider::Slider(int x, int y, int w, float min, float max, float initial)
    : minValue(min), maxValue(max) {
  View::pos(x, y);
  View::size(w, thumbHeight);

  setValue(initial);
}

float Slider::getThumbX(int absoluteSliderX) const {
  float normalizedValue = (currentValue - minValue) / (maxValue - minValue);
  float trackLenght = (float)width - thumbWidth;
  return absoluteSliderX + (normalizedValue * trackLenght);
}

float Slider::getValueFromMouseX(int mouseX, int abosoluteSliderX,
                                int sliderWidth) const {
  int trackLength = sliderWidth - thumbWidth;
  if (trackLength <= std::numeric_limits<float>::epsilon()) {
    if (mouseX < abosoluteSliderX + (float)sliderWidth / 2.0f) {
      return minValue;
    } else {
      return maxValue;
    }
  }
  
  float clickXInTrack =
      mouseX - abosoluteSliderX - thumbWidth / 2.0f;
  float normalizedClick = std::clamp(clickXInTrack / trackLength, 0.0f, 1.0f);

  float newValue = minValue + normalizedClick * (maxValue - minValue);

  if (stepValue > 0.0f) {
    newValue = std::round(newValue / stepValue) * stepValue;
    newValue = std::clamp(newValue, minValue, maxValue);
  }

  return newValue;
}

Slider& Slider::setRange(float min, float max) {
  minValue = min;
  maxValue = max;
  setValue(currentValue);
  return *this;
}

Slider& Slider::setValue(float value) {
  float clampedValue = std::clamp(value, minValue, maxValue);
  if (stepValue > 0.0f) {
    clampedValue = std::round(clampedValue / stepValue) * stepValue;
    clampedValue = std::clamp(clampedValue, minValue, maxValue);
  }

  if (currentValue != clampedValue) {
    currentValue = clampedValue;

    if (onValueChangedCallback) {
      onValueChangedCallback(currentValue);
    }
  }

  return *this;
}

Slider& Slider::setStep(float step) {
  stepValue = std::max(0.0f, step);
  setValue(currentValue);
  return *this;
}

Slider& Slider::setColors(const Color& track, const Color& thumb) {
  trackColor = track;
  thumbColor = thumb;
  return *this;
}

Slider& Slider::setDimentions(int trackH, int thumbW, int thumbH) {
  trackHeight = std::max(1, trackH);
  thumbWidth = std::max(1, thumbW);
  thumbHeight = std::max(1, thumbH);
  View::size(width, thumbHeight);

  if (this->width <= this->thumbWidth) {
    std::cerr << this->width - 1;
    this->thumbWidth = this->width - 1;
  }

  return *this;
}

Slider& Slider::onValueChanged(OnValueChangedHandler handler) {
  onValueChangedCallback = std::move(handler);
  return *this;
}

void Slider::layout(int offsetX, int offsetY) {
    View::layout(offsetX, offsetY);
}

void Slider::draw(const ViewContext& context) {
  UI::Renderer* renderer = context.renderer;
  
  renderer->setDrawColor(trackColor);
  renderer->drawRect(x, y + (height-trackHeight) / 2.0f, width, trackHeight);

  renderer->setDrawColor(thumbColor);
  float thumbDrawX = getThumbX(x);

  renderer->fillRect(thumbDrawX, y + (height -  thumbHeight) / 2.0f, thumbWidth, thumbHeight);

  View::draw(context);
}

bool Slider::handleProximaEvent(const ProximaEvent& event) {
  int absX = getAbsoluteX();
  int absY = getAbsoluteY();

  std::cout << isDragging << std::endl;

  switch (event.type) {
    case MOUSE_PRESS:
      if (event.x >= absX && event.x < (absX + width) && event.y >= absY &&
          event.y < (absY + height)) {
        assert(!isDragging);
        isDragging = true;
        float newValue = getValueFromMouseX(event.x, absX, width);
        setValue(newValue);
        return true;
      }
      break;
    case MOUSE_RELEASE:
      if (isDragging) {
        isDragging = false;
        return true;
      }
      break;
    case MOUSE_MOTION:
      if (isDragging) {
        float newValue = getValueFromMouseX(event.x, absX, width);
        std::cout << getValueFromMouseX(event.x, absX, width);
        setValue(newValue);
        return true;
      }
      break;
    default:
      break;
  }

  return View::handleProximaEvent(event);
}

}  // Namespace UI