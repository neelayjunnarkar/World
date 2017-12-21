#ifndef SPHERICALFUNCTIONMODEL_HPP
#define SPHERICALFUNCTIONMODEL_HPP

#include "FunctionModel.hpp"

class SphericalFunctionModel: public Model {
public:
    //f is a function of theta, phi
    SphericalFunctionModel(std::function<float(float,float)> f, 
        const float theta_min, const float theta_max, const int theta_samples, 
        const float phi_min, const float phi_max, const int phi_samples,
        const bool stitch = false);
    virtual void update() override;
};

#endif