#pragma once
#include <algorithm>
#include <functional>
#include <string>
#include "View.hpp"
#include "Color.hpp"
#include "Platform.hpp"

namespace UI {

class PEXPORT Slider : public View {
 public:
  using OnValueChangedHandler = std::function<void(float)>;

 private:
  float minValue = 0.0f;
  float maxValue = 1.0f;
  float currentValue = 0.5f;
  float stepValue = 0.0f;

  Color trackColor = {150, 150, 150, 255};
  Color thumbColor = {50, 50, 200, 255};
  int trackHeight = 4;
  int thumbWidth = 16;
  int thumbHeight = 24;

  bool isDragging = false;

  OnValueChangedHandler onValueChangedCallback;

  float getThumbX(int absoluteSliderX) const;
  float getValueFromMouseX(int mouseX, int absoluteSliderX,
                     int sliderWidth) const;

 public:
  Slider(int x = 0, int y = 0, int w = 200, float min = 0.0f, float max = 1.0f,
       float initial = 0.5f);
  ~Slider() override = default;

  Slider& setRange(float min, float max);
  Slider& setValue(float value);
  Slider& setStep(float step); 
  Slider& setColors(const Color& track, const Color& thumb); 
  Slider& setDimentions(int trackH, int thumbW, int thumbH);

  Slider& onValueChanged(OnValueChangedHandler handler);

  float getValue() const { return currentValue; }

  void layout(int offsetX, int offsetY) override;
  void draw(const ViewContext& context) override;
  bool handleProximaEvent(const ProximaEvent& event) override;

  Slider(const Slider&) = delete;
  Slider& operator=(const Slider&) = delete;
  Slider(Slider&&) = delete;
  Slider& operator=(Slider&&) =delete;
};

}  // Namespace UI