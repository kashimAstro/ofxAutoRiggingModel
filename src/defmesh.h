#ifndef DEFMESH_H_INCLUDED
#define DEFMESH_H_INCLUDED

#include "attachment.h"
#include "filter.h"
#include "DisplayMesh.h"

class Motion;

class DefMesh : public DisplayMesh
{
public:
    DefMesh(const Mesh inMesh, const Skeleton &inOrigSkel, const vector<Vector3> &inMatch,
            const Attachment &inAttachment, Motion *inMotion = NULL) 
      : origSkel(inOrigSkel), match(inMatch), attachment(inAttachment),
        origMesh(inMesh), motion(inMotion), filter(match, origSkel.fPrev())
    {
        transforms.resize(origSkel.fPrev().size() - 1);
    }

    void setMotion(Motion *inMotion) { motion = inMotion; }
    Motion *getMotion() const { return motion; }
    void updateIfHasMotion() const { if(motion) updateMesh(); }

    vector<Vector3> getSkel() const;
    const Skeleton &getOrigSkel() const { return origSkel; }

    const Attachment &getAttachment() const { return attachment; }

    const Mesh &getMesh() { updateMesh(); return curMesh; }

private:
    double getLegRatio() const;
    vector<Transform<> > computeTransforms() const;
    void updateMesh() const;

    Skeleton origSkel;
    vector<Vector3> match;
    Attachment attachment;
    Mesh origMesh;
    mutable Mesh curMesh;
    vector<Quaternion<> > transforms;
    Motion *motion;

    vector<double> footOffsets;
    mutable MotionFilter filter;
};

#endif //DEFMESH_H_INCLUDED
