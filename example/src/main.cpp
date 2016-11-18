#include "ofMain.h"
#include "ofxAutoRiggingModel.h"
#include "ofxFakeShadowMap.h"

class ofApp : public ofBaseApp{
        public:
		ofEasyCam cam;
		ofLight light;
		ofMaterial mat;
		ofxAutoRiggingModel rig;
                ofxFakeShadowMap shadow;
		bool showRig;

		void setup(){
			ofSetSmoothLighting(true);
			shadow.setup();
		        shadow.setColors(ofColor(0,0,0));
		        shadow.setAlpha(1.f);
			shadow.setDirLight(ofVec3f(0.1,-0.2,0.2));
		        light.setPosition(
				ofMap(0.1,0,10,0,1000),
		                ofMap(-0.2,0,10,0,1000),
		                ofMap(0.2,0,10,0,1000)
		        );

			rig.load("test.obj","jumpAround.txt",HUMAN);
			cam.setNearClip(.01);
			cam.setFarClip(ofGetWidth()*2);
			cam.setDistance(5);
		}

		void update(){
			ofSetWindowTitle(ofToString(ofGetFrameRate()));
			rig.update();
		}

		void render(){
			ofPushMatrix();
			ofSetColor(ofColor::red);
			if(showRig)
				rig.drawSkeleton();
			else
				rig.getMesh().drawWireframe();
			ofPopMatrix();
		}

		void draw(){
			ofEnableDepthTest();
			cam.begin();

		        ofEnableLighting();
			light.enable();
		        mat.begin();
				render();
				//plane
				ofPushMatrix();
				ofRotateX(90);
				ofSetColor(ofColor::blue);
			        ofDrawPlane(0,0,0,5,5);
				ofPopMatrix();
			mat.end();
		        light.disable();
		        ofDisableLighting();

		        shadow.begin(cam);
				render();
			shadow.end();

			cam.end();
			ofDisableDepthTest();
		}

		void keyPressed(int key){
			if(key == ' ')
				showRig=!showRig;
			if(key == '1')
				rig.load("test.obj","runAround.txt");
			if(key == '2')
				rig.load("cheb.obj","wakeUpSequence2.txt");
			if(key == '3')
				rig.load("FinalBaseMesh.obj","jumpAround.txt");
			if(key == '4')
				rig.load("test.obj","crossWalk.txt");
			if(key == '5')
				rig.load("test.obj","walkAndSkip.txt");
			if(key == '6')
				rig.load("test.obj","walk.txt");
		}
};

int main()
{
	ofGLWindowSettings settings;
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
