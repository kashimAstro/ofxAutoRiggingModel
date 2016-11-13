#include "ofMain.h"

#define STR(A) #A

class ofxFakeShadowMap{
	public:
		ofColor colors;
		ofVec3f lightDir;
		ofShader shader;
		float alpha;

		void setup()
		{
			alpha  = 0.2;
			colors = ofColor(15);
			lightDir = ofVec3f(1,2,2);
			string vertex = STR(
				void main() {  
				    gl_Position = ftransform();
				}
			);
			string fragment = STR(
				uniform float alpha;
				uniform vec3 colors;
				void main() {
				    gl_FragColor = vec4(colors,alpha);
				}
			);
			shader.setupShaderFromSource(GL_VERTEX_SHADER,  vertex);
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER,fragment);
			if(ofIsGLProgrammableRenderer())
			{
				shader.bindDefaults();
			}
			shader.linkProgram();
		}

		void setDirLight(ofVec3f _ld)
		{
			lightDir = _ld;
		}

		void setColors(ofColor _c)
		{
			colors = _c;
		}

		void setAlpha(float _a)
                {
			alpha = _a;
		}

		void begin(ofCamera cam)
		{
                        ofVec3f lightRay = lightDir;
			if(lightRay.y == 0) lightRay.y = 1e-5;
			lightRay = -lightRay / lightRay.y;

			ofPushStyle();
                        ofPushMatrix();
			ofGetCurrentRenderer()->multMatrix(ofMatrix4x4(1,0,0,0, (float)lightRay.x,0,(float)lightRay.z,0, 0,0,1,0, 0,0.01f,0,1));

			shader.begin();
			shader.setUniform1f("alpha",alpha);
			shader.setUniform3f("colors",
					     ofMap(colors.r,0,255,0.,1.),
					     ofMap(colors.g,0,255,0.,1.),
					     ofMap(colors.b,0,255,0.,1.));
                }

                void end(){
			shader.end();

                        ofPopMatrix();
			ofPopStyle();
                }
};
