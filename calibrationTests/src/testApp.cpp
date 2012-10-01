#include "testApp.h"
#include "ofSoundUtils.h"


inline void gaussian_elimination(float *input, int n){
	// ported to c from pseudocode in
	// http://en.wikipedia.org/wiki/Gaussian_elimination

	float * A = input;
	int i = 0;
	int j = 0;
	int m = n-1;
	while (i < m && j < n){
	  // Find pivot in column j, starting in row i:
	  int maxi = i;
	  for(int k = i+1; k<m; k++){
	    if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
	      maxi = k;
	    }
	  }
	  if (A[maxi*n+j] != 0){
	    //swap rows i and maxi, but do not change the value of i
		if(i!=maxi)
		for(int k=0;k<n;k++){
			float aux = A[i*n+k];
			A[i*n+k]=A[maxi*n+k];
			A[maxi*n+k]=aux;
		}
	    //Now A[i,j] will contain the old value of A[maxi,j].
	    //divide each entry in row i by A[i,j]
		float A_ij=A[i*n+j];
		for(int k=0;k<n;k++){
			A[i*n+k]/=A_ij;
		}
	    //Now A[i,j] will have the value 1.
	    for(int u = i+1; u< m; u++){
    		//subtract A[u,j] * row i from row u
	    	float A_uj = A[u*n+j];
	    	for(int k=0;k<n;k++){
	    		A[u*n+k]-=A_uj*A[i*n+k];
	    	}
	      //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
	    }

	    i++;
	  }
	  j++;
	}

	//back substitution
	for(int i=m-2;i>=0;i--){
		for(int j=i+1;j<n-1;j++){
			A[i*n+m]-=A[i*n+j]*A[j*n+m];
			A[i*n+j]=0;
		}
	}
}


inline vector<float> ofSolveEquationSystem(float *coeffs, int numEquations){
	// ported to c from pseudocode in
	// http://en.wikipedia.org/wiki/Gaussian_elimination

	float * A = new float[numEquations*(numEquations+1)];
	memcpy(A,coeffs,sizeof(float)*numEquations*(numEquations+1));
	int i = 0;
	int j = 0;
	int m = numEquations;
	int n = numEquations+1;
	while (i < m && j < n){
	  // Find pivot in column j, starting in row i:
	  int maxi = i;
	  for(int k = i+1; k<m; k++){
	    if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
	      maxi = k;
	    }
	  }
	  if (A[maxi*n+j] != 0){
	    //swap rows i and maxi, but do not change the value of i
		if(i!=maxi)
		for(int k=0;k<n;k++){
			swap(A[maxi*n+k],A[i*n+k]);
		}
	    //Now A[i,j] will contain the old value of A[maxi,j].
	    //divide each entry in row i by A[i,j]
		float A_ij=A[i*n+j];
		for(int k=0;k<n;k++){
			A[i*n+k]/=A_ij;
		}
	    //Now A[i,j] will have the value 1.
	    for(int u = i+1; u< m; u++){
    		//subtract A[u,j] * row i from row u
	    	float A_uj = A[u*n+j];
	    	for(int k=0;k<n;k++){
	    		A[u*n+k]-=A_uj*A[i*n+k];
	    	}
	      //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
	    }

	    i++;
	  }
	  j++;
	}

	//back substitution
	for(int i=m-2;i>=0;i--){
		for(int j=i+1;j<n-1;j++){
			A[i*n+m]-=A[i*n+j]*A[j*n+m];
			A[i*n+j]=0;
		}
	}

	vector<float> result(numEquations);
	for(int i=0;i<(int)result.size();i++){
		result[i] = A[i*(numEquations+1)+numEquations];
	}
	delete A;
	return result;
}

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	ofFill();
	ofLine(50,50,1050,50);
	for(int i=0;i<5;i++){
		ofCircle(1000*.25*i+50,50,3);
	}
	ofSetColor(255,0,0);
	for(int i=0;i<realPosition.size();i++){
		ofCircle(1000*realPosition[i]+50,50,3);
	}
	float x = ofGetMouseX();
	float realX = 0;
	if(realPosition.size()==5 && x>=50+realPosition[0]*1000 && x<=50+realPosition[4]*1000){
		float pct = double(x-50)/1000.;
		for(int i=0;i<realPosition.size()-1;i++){
			if(realPosition[i+1]>=pct){
				realX = ofMap(pct,realPosition[i],realPosition[i+1],0.25*i,0.25*(i+1));
				realX *= 1000;
				realX += 50;
				break;
			}
		}
		ofSetColor(0,255,0);
		ofCircle(realX,50,3);
	}

	ofSetColor(255);
	for(int i=0;i<4;i++){
		ofLine(50+1000*.25*i,50+400-200*.25*i,50+1000*.25*(i+1),50+400-200*.25*(i+1));
	}
	if(realPosition.size()==5){
		/*ofSetColor(0,255,0);
		for(int i=0;i<4;i++){
			ofLine(50+1000*.25*i,50+400-200*realPosition[i],50+1000*.25*(i+1),50+400-200*realPosition[i+1]);
		}*/


		//y = a*x4+b*x3+c*x2+dx+e;

		float equations[5][6] = {
				{0,0,0,0,1,realPosition[0]},
				{0.25*0.25*0.25*.25,0.25*0.25*0.25,0.25*0.25,0.25,1,realPosition[1]},
				{.5*.5*.5*.5,.5*.5*.5,.5*.5,.5,1,realPosition[2]},
				{.75*.75*.75*.75,.75*.75*.75,.75*.75,.75,1,realPosition[3]},
				{1,1,1,1,1,realPosition[4]},
		};
		vector<float> result = ofSolveEquationSystem(&equations[0][0],5);

		/*gaussian_elimination(&equations[0][0],6);

		float a = equations[0][5];
		float b = equations[1][5];
		float c = equations[2][5];
		float d = equations[3][5];
		float e = equations[4][5];*/
		float a = result[0];
		float b = result[1];
		float c = result[2];
		float d = result[3];
		float e = result[4];

		ofNoFill();
		ofSetColor(0,0,255);
		ofBeginShape();
		ofVertex(50,50+400-200*e);
		for(int j=0;j<101;j++){
			float x = float(j)/100.;
			float x2 = x*x;
			float x4 = x2*x2;
			float x3 = x2*x;
			float y = a*x4 + b*x3 + c*x2 + d*x + e;
			ofVertex(50+1000*x,50+400-200*y);
		}
		ofEndShape();

		//float x = ofGetMouseX();
		//float realX = 0;
		if(realPosition.size()==5 && x>=50+realPosition[0]*1000 && x<=50+realPosition[4]*1000){
			ofFill();
			float equations[5][6] = {
					{pow(realPosition[0],4),pow(realPosition[0],3),pow(realPosition[0],2),realPosition[0],1,0},
					{pow(realPosition[1],4),pow(realPosition[1],3),pow(realPosition[1],2),realPosition[1],1,0.25},
					{pow(realPosition[2],4),pow(realPosition[2],3),pow(realPosition[2],2),realPosition[2],1,0.5},
					{pow(realPosition[3],4),pow(realPosition[3],3),pow(realPosition[3],2),realPosition[3],1,0.75},
					{pow(realPosition[4],4),pow(realPosition[4],3),pow(realPosition[4],2),realPosition[4],1,1},
			};
			gaussian_elimination(&equations[0][0],6);

			float a = equations[0][5];
			float b = equations[1][5];
			float c = equations[2][5];
			float d = equations[3][5];
			float e = equations[4][5];

			float x = double(ofGetMouseX()-50)/1000.;
			float x2 = x*x;
			float x3 = x2*x;
			float x4 = x2*x2;
			float realX = a*x4 + b*x3 + c*x2 + d*x + e;
			ofSetColor(0,0,255);
			realX *= 1000;
			realX += 50;
			ofCircle(realX,50,3);
		}

	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(button==0 && realPosition.size()<5)realPosition.push_back(double(x-50)/1000.);
	if(button==2) realPosition.clear();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
