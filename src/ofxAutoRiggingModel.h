#include "ofMain.h"

#include "skeleton.h"
#include "utils.h"
#include "debugging.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include "defmesh.h"
#include "motion.h"
#include "bvh.h"
#include "bvh-parser.h"

enum AUTO_TYPE_RIGGING
{
	HUMAN,
	HORSE,
	TQUAD,
	CENTAUR
};

class ofxAutoRiggingModel
{
	public:
	    void load(string _fileMesh, string _fileMotion, AUTO_TYPE_RIGGING type=HUMAN, float _scale=2.,bool exports=false);
	    void setTypeSkeleton(AUTO_TYPE_RIGGING type);
	    void setRotation(ofVec4f rot);
	    void setScale(float _s);
	    void update();
	    void drawSkeleton();
	    ofVboMesh getMesh();

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
	    ofVboMesh lineSkeleton;
	    ofVboMesh mesh;
	    DefMesh * def;
};
