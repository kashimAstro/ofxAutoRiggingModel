#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include "matrix.h"
#include "vector.h"
#include "transform.h"

//class Matrixn;
//class Vectorn;

class MotionFilter
{
public:
    MotionFilter(const vector<Vector3> &inJoints, const vector<int> inPrev)
        : joints(inJoints), prev(inPrev) {}

    void step(const vector<Transform<> > &transforms, vector<Vector3> feet);

    const vector<Transform<> > &getTransforms() const { return curTransforms; }

private:
    Matrixn<double> getJac(const vector<Transform<> > &transforms) const;
    void addTranslation();

    vector<Vector3> joints;
    vector<int> prev;

    Vector3 prevTrans;
    Vectorn<double> prevFeet;
    vector<Transform<> > curTransforms;
};

#endif //FILTER_H_INCLUDED
