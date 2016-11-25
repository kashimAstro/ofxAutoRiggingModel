#include "ofMain.h"
#include "bvh.h"

namespace BVHParsing 

{
	class ofxBVHParser
	{
		public:
			Bvh *bvh;
			ofVboMesh vbo;
			short bvh_elements = 0;
			int frame=1;
			int frameChange=0;

			void process(JOINT* joint, vector<glm::vec4>& vertices, vector<GLshort>& indices, GLshort parentIndex = 0)
			{
			    glm::vec4 translatedVertex = joint->matrix[3];
			    vertices.push_back(translatedVertex);
			    GLshort myindex = vertices.size() - 1;
			    if( parentIndex != myindex )
			    {
				indices.push_back(parentIndex);
				indices.push_back(myindex);
			    }
			    for(auto& child : joint->children)
			    {
				process(child, vertices, indices, myindex);
			    }
			}
			
			vector<ofVec3f> glm4ToVec3(vector<glm::vec4> vert){
				vector<ofVec3f> f;
				for(int i = 0; i < vert.size(); i++){
					f.push_back(ofVec3f(vert[i].x,vert[i].y,vert[i].z));
					//ofLog()<<ofVec3f(vert[i].x,vert[i].y,vert[i].z);
				}
				return f;
			}

			void load(string _fi){
				bvh = new Bvh();
				glm::vec3 bvh_scale = glm::vec3(0.25, 0.25, 0.25);
				string path = ofToDataPath(_fi,true);
				//ofLog()<<path;
				bvh->load(path);
				bvh->testOutput();
				bvh->printJoint(bvh->getRootJoint());
				bvh->moveTo(frame++);
				vector<glm::vec4> vertices;
				vector<GLshort> bvhindices;
				process((JOINT*)bvh->getRootJoint(), vertices, bvhindices);
				bvh_elements = bvhindices.size();
				vector<ofVec3f> v = glm4ToVec3(vertices);
				vbo.setMode(OF_PRIMITIVE_LINES);
				for(int i = 0; i < v.size(); i++)
				{
					vbo.addVertex(ofVec3f(v[i].x,v[i].y,v[i].z));
				}
			}

			void update(float _m){
				bvh->moveTo(_m);
				vector<glm::vec4> vertices;
				vector<GLshort> bvhindices;
				process((JOINT*)bvh->getRootJoint(), vertices, bvhindices);
				bvh_elements = bvhindices.size();
				vector<ofVec3f> v = glm4ToVec3(vertices);
				vbo.clear();
				for(int i = 0; i < v.size(); i++)
				{
					vbo.addVertex(ofVec3f(v[i].x,v[i].y,v[i].z));
				}
			}

			ofMesh getMesh()
			{
				return vbo;
			}


	};
};
