#ifndef TESTMODEL_HPP
#define TESTMODEL_HPP

#include "Model.hpp"

class TestModel: public Model {
private:
    static const std::vector<Vertex> vertex_data;
    static const std::vector<GLushort> element_data;
public:
    TestModel();
    virtual void update() override;
};

#endif