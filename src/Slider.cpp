#include "UI/Slider.hpp"
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace UI {

Slider::Slider(int x, int y, int w, float min, float max, float initial)
    : minValue(min), maxValue(max) {
  View::pos(x, y);
  View::size(w, thumbHeight);

  valueTextLabel = std::make_unique<Text>("");
  valueTextLabel->setColor({0, 0, 0, 255});
  valueTextLabel->z_index(zIndex + 1);
  add(std::move(valueTextLabel)); 

  setValue(initial);
}

float Slider::getThumbX(int absoluteSliderX) const {
  float normalizedValue = (currentValue - minValue) / (maxValue - minValue);
  float trackLenght = (float)width - thumbWidth;
  return absoluteSliderX + (normalizedValue * trackLenght);
}

float Slider::getValueFromMouseX(int mouseX, int abosoluteSliderX,
                                 int sliderWidth) const {
  float trackLength = (float)sliderWidth - thumbWidth;
  float clickXInTrack =
      (float)mouseX - abosoluteSliderX - (float)thumbWidth / 2.0f;
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

    if (valueTextLabel) {
        for (const auto& child_ptr : children) {
            if (Text* text_ptr = dynamic_cast<Text*>(child_ptr.get())) {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) << currentValue;
                text_ptr->content(ss.str());
                break;
            }
        }
    }

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

Slider& Slider::setColors(SDL_Color track, SDL_Color thumb) {
  trackColor = track;
  thumbColor = thumb;
  return *this;
}

Slider& Slider::setDimentions(int trackH, int thumbW, int thumbH) {
  trackHeight = std::max(1, trackH);
  thumbWidth = std::max(1, thumbW);
  thumbHeight = std::max(1, thumbH);
  View::size(width, thumbHeight);
  return *this;
}

Slider& Slider::onValueChanged(OnValueChangedHandler handler) {
  onValueChangedCallback = std::move(handler);
  return *this;
}

void Slider::layout(int offsetX, int offsetY) {
    if (valueTextLabel) {
        for (const auto& child_ptr : children) {
            if (Text* text_ptr = dynamic_cast<Text*>(child_ptr.get())) {
                text_ptr->pos(getThumbX(offsetX) - text_ptr->width/2 - (offsetX - x), // Relative to Slider's top-left
                              - text_ptr->height - 5);
                text_ptr->layout(offsetX - x, offsetY - y);
                break;
            }
        }
    }

    View::layout(offsetX, offsetY);
}

void Slider::draw(SDL_Renderer* renderer) {
  SDL_Color originalColor;
  SDL_GetRenderDrawColor(renderer, &originalColor.r, &originalColor.g,
                         &originalColor.b, &originalColor.a);

  SDL_SetRenderDrawColor(renderer, trackColor.r, trackColor.g, trackColor.b,
                         trackColor.a);
  SDL_FRect trackRect = {(float)x, (float)y + (height - trackHeight) / 2.0f,
                         (float)width, (float)trackHeight};
  SDL_RenderFillRect(renderer, &trackRect);

  SDL_SetRenderDrawColor(renderer, thumbColor.r, thumbColor.g, thumbColor.b,
                         thumbColor.a);
  float thumbDrawX = getThumbX(x);

  SDL_FRect thumbRect = {thumbDrawX, (float)y + (height - thumbHeight) / 2.0f,
                         (float)thumbWidth, (float)thumbHeight};
  SDL_RenderFillRect(renderer, &thumbRect);

  SDL_SetRenderDrawColor(renderer, originalColor.r, originalColor.g,
                         originalColor.b, originalColor.a);
  View::draw(renderer);
}

bool Slider::handleProximaEvent(const ProximaEvent& event) {
  int absX = x;
  int absY = y;

  switch (event.type) {
    case MOUSE_PRESS:
      if (event.x >= absX && event.x < (absX + width) && event.y >= absY &&
          event.y < (absY + height)) {
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