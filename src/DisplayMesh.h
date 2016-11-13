#ifndef DISPLAYMESH_H
#define DISPLAYMESH_H

#include "mesh.h"

class DisplayMesh
{
public:
    virtual ~DisplayMesh() {}

    virtual const Mesh &getMesh() = 0;
    virtual vector<Vector3> getSkel() const { return vector<Vector3>(); }
};

class StaticDisplayMesh : public DisplayMesh
{
public:
    StaticDisplayMesh(const Mesh &inM) : m(inM) {}

    virtual const Mesh &getMesh() { return m; }
private:

    Mesh m;
};

#endif //DISPLAYMESH_H
