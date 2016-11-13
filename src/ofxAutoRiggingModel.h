#include "ofMain.h"

#include "skeleton.h"
#include "utils.h"
#include "debugging.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include "defmesh.h"
#include "motion.h"

class ofxAutoRiggingModel
{
	public:
	    void load(string _fileMesh, string _fileMotion);
	    void setTypeSkeleton(string type);
	    void setRotation(ofVec4f rot);
	    void setScale(float _s);
	    void update();
	    void drawSkeleton();
	    ofMesh getMesh();

	private:
	    bool stopAtMesh;
	    string filename;
	    string motionname;
	    Quaternion<> meshTransform;
	    double skelScale;
	    bool noFit;
	    Skeleton skeleton;
	    Skeleton given;
	    HumanSkeleton human;
	    string skeletonname;
	    ofMesh lineSkeleton;
	    ofMesh mesh;
	    DefMesh * def;
};
