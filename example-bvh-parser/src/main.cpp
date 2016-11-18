#include "ofMain.h"
#include "ofxAutoRiggingModel.h"
#include "ofxFakeShadowMap.h"

class ofApp : public ofBaseApp{
	public:
		ofEasyCam cam;
		ofxBVHParser bvh;
                ofxFakeShadowMap shadow;

		void setup(){
			shadow.setup();
                        shadow.setDirLight(ofVec3f(0.1,-0.2,0.2));

			bvh.load("1.bvh");
		}

		void update(){
			ofSetWindowTitle(ofToString(ofGetFrameRate()));
			bvh.update(mouseX);
		}

		void draw(){
			ofEnableDepthTest();
			cam.begin();
				 ofScale(5,5,5);
				 ofSetLineWidth(4);
				 bvh.getMesh().draw();
		                 shadow.begin(cam);
					 bvh.getMesh().draw();
                        	 shadow.end();
			cam.end();
			ofDisableDepthTest();
		}
};

int main()
{
	ofGLWindowSettings settings;
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
