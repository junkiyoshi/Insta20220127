#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->line.clear();

	float R = 250;
	float r = R * 0.25;
	float v_step = 0;
	float u_span = 20;
	ofColor color;
	int hue = 0;

	for (float u = 0; u < 360; u += u_span) {

		auto noise_seed = ofRandom(1000);
		for (float v = 0; v < 360; v += 3) {

			auto noise_value = ofNoise(glm::vec4(noise_seed + ofGetFrameNum() * 0.05, this->make_point(R, r, u, v) * 0.005));

			if (noise_value < 0.45) {

				continue;
			}

			auto prev_noise_value = ofNoise(glm::vec4(noise_seed + ofGetFrameNum() * 0.05, this->make_point(R, r, u, v - 1.5) * 0.005));
			auto next_noise_value = ofNoise(glm::vec4(noise_seed + ofGetFrameNum() * 0.05, this->make_point(R, r, u, v + 1.5) * 0.005));

			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(this->make_point(R, r, u - u_span * 0.5 + 1, v - 1.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, u + u_span * 0.5 - 1, v - 1.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, u + u_span * 0.5 - 1, v + 1.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, u - u_span * 0.5 + 1, v + 1.5)));
			

			this->face.addVertices(vertices);
			this->line.addVertices(vertices);

			for (int i = 0; i < 4; i++) {
			
				this->face.addColor(ofColor(0));
				this->line.addColor(ofColor(255));
			}
			
			this->face.addIndex(this->face.getNumVertices() - 1); this->face.addIndex(this->face.getNumVertices() - 2); this->face.addIndex(this->face.getNumVertices() - 4);
			this->face.addIndex(this->face.getNumVertices() - 3); this->face.addIndex(this->face.getNumVertices() - 2); this->face.addIndex(this->face.getNumVertices() - 4);

			this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 4);
			this->line.addIndex(this->line.getNumVertices() - 2); this->line.addIndex(this->line.getNumVertices() - 3);

			if (next_noise_value < 0.45) {

				this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
			}

			if (prev_noise_value < 0.45) {

				this->line.addIndex(this->line.getNumVertices() - 3); this->line.addIndex(this->line.getNumVertices() - 4);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofRotateY(ofGetFrameNum() * 0.6666666666666666);

	this->line.drawWireframe();
	this->face.draw();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}