#ifndef FUNCTIONMODEL_HPP
#define FUNCTIONMODEL_HPP

#include <functional>

#include "Model.hpp"

class FunctionModel: public Model {
private:
    std::function<float(float, float)> f;
public:
    FunctionModel(std::function<float(float,float)> f, const float x_min, const float x_max, const int x_samples, const float y_min, const float y_max, const int y_samples);
    virtual void update() override;
};

#endif