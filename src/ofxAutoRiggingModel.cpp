#include "ofMain.h"
#include "ofxAutoRiggingModel.h"

void ofxAutoRiggingModel::load(string _fileMesh, string _fileMotion, AUTO_TYPE_RIGGING type, float _scale, bool exports)
{
	noFit = false;
	setScale(_scale); 
	setTypeSkeleton(type);
	lineSkeleton.setMode(OF_PRIMITIVE_LINES);
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);

	Mesh m(ofToDataPath(_fileMesh,true));
	motionname = ofToDataPath(_fileMotion,true);
	if(m.vertices.size() == 0) 
	{
		ofLog()<<"Error Mesh size!";
	}
	for(int i = 0; i < (int)m.vertices.size(); ++i)
		m.vertices[i].pos = meshTransform * m.vertices[i].pos;
	m.normalizeBoundingBox();
	m.computeVertexNormals();
	given = skeleton;
	given.scale(skelScale * 0.7);

	vector<MeshVertex> v = m.vertices;
	for(int i = 0; i < v.size(); i++ )
	{
		mesh.addVertex(ofVec3f(v[i].pos[0],v[i].pos[1],v[i].pos[2]));
		mesh.addNormal(ofVec3f(v[i].normal[0],v[i].normal[1],v[i].normal[2]));
		mesh.addTexCoord(ofVec2f(v[i].texture[0],v[i].texture[1]));
	}

	PinocchioOutput o;
	if(!noFit)
	{
		ofLog()<<"Auto rigging!";
		o = autorig(given, m);
	}
	else{
	    TreeType *distanceField = constructDistanceField(m);
	    VisTester<TreeType> *tester = new VisTester<TreeType>(distanceField);
		o.embedding = skeleton.fGraph().verts;
	    for(int i = 0; i < (int)o.embedding.size(); ++i)
		o.embedding[i] = m.toAdd + o.embedding[i] * m.scale;
	    o.attachment = new Attachment(m, skeleton, o.embedding, tester);
	    delete tester;
	    delete distanceField;
	}
	if(o.embedding.size() == 0) {
	    ofLog()<< "Error embedding";
	}
	if(motionname.size() > 0) {
	    def = new DefMesh(m, given, o.embedding, *(o.attachment), new Motion(motionname));
	    ofLog()<<"DefMesh !?";
	}
	else {
	    vector<MeshVertex> v = m.vertices;
	    for(int i = 0; i < v.size(); i++ )
	    {
		    mesh.addVertex(ofVec3f(v[i].pos[0],v[i].pos[1],v[i].pos[2]));
		    mesh.addNormal(ofVec3f(v[i].normal[0],v[i].normal[1],v[i].normal[2]));
		    mesh.addTexCoord(ofVec2f(v[i].texture[0],v[i].texture[1]));
	    }
	    for(int i = 1; i < (int)o.embedding.size(); ++i)
	    {
		    lineSkeleton.addVertex(ofVec3f(o.embedding[i][0],
						   o.embedding[i][1],
						   o.embedding[i][2]));
		    lineSkeleton.addVertex(ofVec3f(o.embedding[given.fPrev()[i]][0], 
						   o.embedding[given.fPrev()[i]][1],
						   o.embedding[given.fPrev()[i]][2]));
	    }
	}
	if(exports)
	{
		//output skeleton embedding
		for(int i = 0; i < (int)o.embedding.size(); ++i)
			o.embedding[i] = (o.embedding[i] - m.toAdd) / m.scale;
		ofstream os(ofToDataPath("skeleton.out",true));
		for(int i = 0; i < (int)o.embedding.size(); ++i) {
			os << i << " " << o.embedding[i][0] << " " << o.embedding[i][1] <<
				   " " << o.embedding[i][2] << " " << skeleton.fPrev()[i] << endl;
		}
		//output attachment
		std::ofstream astrm(ofToDataPath("/attachment.out",true));
		for(int i = 0; i < (int)m.vertices.size(); ++i) {
			Vector<double, -1> v = o.attachment->getWeights(i);
			for(int j = 0; j < v.size(); ++j) {
			    double d = floor(0.5 + v[j] * 10000.) / 10000.;
			    astrm << d << " ";
			}
			astrm << endl;
		}
	}
	delete o.attachment;
}

void ofxAutoRiggingModel::setTypeSkeleton(AUTO_TYPE_RIGGING type)
{
	if(type == HUMAN)
		skeleton = HumanSkeleton();
	else if(type == HORSE)
		skeleton = HorseSkeleton();
	else if(type == TQUAD)
		skeleton = QuadSkeleton();
	else if(type == CENTAUR)
	       skeleton = CentaurSkeleton();
}

void ofxAutoRiggingModel::setRotation(ofVec4f rot)
{
	meshTransform = Quaternion<>(Vector3(rot.x, rot.y, rot.z), rot.w * M_PI / 180.) * meshTransform;
}

void ofxAutoRiggingModel::setScale(float _s) 
{
	skelScale = _s;
}

void ofxAutoRiggingModel::update()
{
	def->updateIfHasMotion();
}

void ofxAutoRiggingModel::drawSkeleton()
{
	lineSkeleton.clear();
	vector<Vector3> v = def->getSkel();
	if(v.size() > 0)
	{
	    const vector<int> &prev = human.fPrev();
	    for(int j = 1; j < (int)prev.size(); ++j) {
		int k = prev[j];
		lineSkeleton.addVertex(ofVec3f( v[j][0], v[j][1], v[j][2] ));
		lineSkeleton.addVertex(ofVec3f( v[k][0], v[k][1], v[k][2] ));
	    }
	}
	ofPushStyle();
	ofPushMatrix();
	ofSetLineWidth(4.5);
	lineSkeleton.draw();
	ofPopMatrix();
	ofPopStyle();
}

ofVboMesh ofxAutoRiggingModel::getMesh()
{
	mesh.clear();
	Mesh m = def->getMesh();
	vector<MeshEdge> v =  m.edges;
	for(int i = 0; i < v.size(); i++ )
	{
	       int vp = m.edges[i].vertex;
	       const Vector3 &p = m.vertices[vp].pos;
	       const Vector3 &n = m.vertices[vp].normal;
	       const Vector3 &t = m.vertices[vp].texture;
	       mesh.addVertex(ofVec3f(p[0],p[1],p[2]));
	       mesh.addNormal(ofVec3f(n[0],n[1],n[2]));
	       mesh.addTexCoord(ofVec2f(t[0],t[1]));
	}
	return mesh;
}
