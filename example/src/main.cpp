#include "ofMain.h"
#include "ofxAutoRiggingModel.h"

class ofApp : public ofBaseApp{
        public:
		ofEasyCam cam;
		ofLight light;
		ofMaterial mat;
		ofxAutoRiggingModel rig;
		bool showRig;

		void setup(){
			ofSetSmoothLighting(true);
		        light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
		        light.setSpecularColor( ofColor(255.f, 255.f, 255.f));
		        mat.setShininess( 64 );

			rig.load("FinalBaseMesh.obj","jumpAround.txt");
			cam.setNearClip(.01);
			cam.setFarClip(ofGetWidth()*2);
			cam.setDistance(5);
		}

		void update(){
			ofSetWindowTitle(ofToString(ofGetFrameRate()));
			rig.update();
		}

		void draw(){
			ofEnableDepthTest();
			cam.begin();
		        ofEnableLighting();
			light.enable();
		        mat.begin();
			ofSetColor(ofColor::red);
			if(showRig)
				rig.drawSkeleton();
			else
				rig.getMesh().draw();
			cam.end();
		        mat.end();
		        light.disable();
		        ofDisableLighting();

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
		}
};

int main()
{
	ofGLWindowSettings settings;
	settings.setGLVersion(3,2);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
