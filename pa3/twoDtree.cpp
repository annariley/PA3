
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* node constructor given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, HSLAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),LT(NULL),RB(NULL)
	{}

/* destructor given */
twoDtree::~twoDtree(){
	clear();
}

/* copy constructor given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


/* operator= given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

/* twoDtree constructor */
twoDtree::twoDtree(PNG & imIn){ 
	this->height=imIn.height();
	this->width=imIn.width();
	stats s = stats(imIn);
	this->root = buildTree(s, pair<int,int>(0,0), pair<int,int>(imIn.width()-1,imIn.height()-1), 1);
}

/* buildTree helper for twoDtree constructor */
twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {
	if (ul == lr) return new Node(ul,lr,s.getAvg(ul,lr));
	/*if(ul.first>=lr.first && ul.second >= lr.second){
		return NULL;
	}
	if(ul.first>lr.first || ul.second > lr.second){
		return NULL;
	}*/
	cout << "upper left: ";
	cout << ul.first;
	cout << ", ";
	cout << ul.second << endl;
	cout << "lower right: ";
	cout << lr.first;
	cout << ", ";
	cout << lr.second << endl;
	pair<int,int> newul = ul;
	pair<int,int> newlr = lr;
	double w1, w2, weightedSum;
	double minEntropy = 1000000000;//2*s.entropy(ul,lr);
	if(ul.second==lr.second || vert){ //vertical
		for(int i = ul.first; i < lr.first; i++){
			double w1 = s.rectArea(ul,pair<int,int>(i,lr.second));
			double w2 = s.rectArea(pair<int,int>(i+1,ul.second),lr);
			double weightedSum = ((w1*(s.entropy(ul,pair<int,int>(i,lr.second))))+(w2*(s.entropy(pair<int,int>(i+1,ul.second),lr))))/(w1+w2);
			if(weightedSum<=minEntropy){
				cout << weightedSum << endl;
				minEntropy = weightedSum;
				newlr.first = i;
				newul.first = i+1;
			}
		}
		
	} else{ //horizontal
		for(int i = ul.second; i < lr.second; i++){
			w1 = s.rectArea(ul,pair<int,int>(lr.first,i));
			w2 = s.rectArea(pair<int,int>(ul.first,i+1),lr);
			weightedSum = (w1*(s.entropy(ul,pair<int,int>(lr.first,i)))+w2*(s.entropy(pair<int,int>(ul.first,i+1),lr)))/(w1+w2);
			if(weightedSum<=minEntropy){
				minEntropy = weightedSum;
				newlr.second = i;
				newul.second = i+1;
			}
		}
	}

	Node * node = new Node(ul,lr,s.getAvg(ul,lr));
	node->LT = buildTree(s,ul,newlr,!vert);
	node->RB = buildTree(s,newul,lr,!vert);
	return node;
}

/* render your twoDtree into a png */
PNG twoDtree::render(){
	PNG retPNG = PNG(this->width,this->height);
	for(int y = 0; y < this->height; y++){
		for(int x = 0; x < this->width; x++){
			HSLAPixel * pix = retPNG.getPixel(x,y);
			*pix = getPixFromTree(x,y, this->root);
		}
	}
	return retPNG;
}
HSLAPixel twoDtree::getPixFromTree(int x, int y, Node * node){
	if(node->LT==NULL &&node->RB==NULL){ //&& x>=node->upLeft.first && x<=node->lowRight.first && y>=node->upLeft.second && y<=node->lowRight.second){
		return node->avg;
	}
	if(x>=node->LT->upLeft.first && x<=node->LT->lowRight.first && y>=node->LT->upLeft.second && y<=node->LT->lowRight.second){
		getPixFromTree(x,y,node->LT);
	}else if(x>=node->RB->upLeft.first && x<=node->RB->lowRight.first && y>=node->RB->upLeft.second && y<=node->RB->lowRight.second){
		getPixFromTree(x,y,node->RB);
	}
}

/* prune function modifies tree by cutting off
 * subtrees whose leaves are all within tol of 
 * the average pixel value contained in the root
 * of the subtree
 */
void twoDtree::prune(double tol){
	prune_recursive(tol,this->root->avg, this->root);
}
void twoDtree::prune_recursive(double tol, HSLAPixel rootpix, Node * node){
	if(node == NULL) return;
	if(rootpix.dist(node->avg)>tol){
		clear_recursive(node);
		return;
	}
	prune_recursive(tol,rootpix,node->LT);
	prune_recursive(tol,rootpix,node->RB);
}

/* helper function for destructor and op= */
/* frees dynamic memory associated w the twoDtree */
void twoDtree::clear() {
	clear_recursive(this->root);
}

void twoDtree::clear_recursive(Node * root){
	if(root->LT==NULL && root->RB==NULL){
		root = NULL;
		delete root;
		return;
	}
	clear_recursive(root->LT);
	clear_recursive(root->RB);
	root = NULL;
	delete root;
	return;

}


/* helper function for copy constructor and op= */
void twoDtree::copy(const twoDtree & orig){
	copy_recursive(orig.root,this->root);
}
void twoDtree::copy_recursive(Node * from, Node * to){
	if(from==NULL){
		return;
	}
	to = new Node(from->upLeft,from->lowRight,from->avg);
	copy_recursive(from->LT,to->LT);
	copy_recursive(from->RB,to->RB);
	return;
}



