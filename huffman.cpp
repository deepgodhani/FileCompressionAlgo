#include<bits/stdc++.h>

using namespace std;

class Node{
public:
char ch;
int freq;
Node* left;
Node* right;
	
	Node(int ch,int freq){
		this->ch=ch;
		this->freq=freq;
		left=NULL;
		right=NULL;
	}

};

struct cmp {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq; 
    }
};

class Huffman{
	public:
	unordered_map<char,int> freqDict;
	unordered_map<char,string> bits;
	string path,text;
	priority_queue<Node*,vector<Node*>,cmp> pq;
	

	Huffman(string path){
	this->path=path;
	}
	
	void freqFromText(string text){
		for(char ch:text){
		freqDict[ch]++;	
		}
		
	}
	void buildHeap(){
		for(auto i:freqDict){
			auto key=i.first;
			auto freq=i.second;
			Node* temp=new Node(key,freq);
			pq.push(temp);
		}
	}

	void buildBinaryTree(){
		int tempSum=0;
		while(pq.size()>1){
			Node* node1=pq.top();
			pq.pop();
			Node* node2=pq.top();
			pq.pop();
			tempSum=node1->freq + node2->freq;
			Node* tmp=new Node(NULL,tempSum);
			tmp->left=node1;
			tmp->right=node2;
			pq.push(tmp);
		}
	}

	void buildBits(Node* root,string& temp){
		if(!root)return ;

		if(root->ch!=NULL){
			bits[root->ch]=temp;
			return ;
		}

		temp.push_back('0');
		buildBits(root->left,temp);
		temp.pop_back();
		temp.push_back('1');
		buildBits(root->right,temp);
		temp.pop_back();
	}

	void bitsFromTree(){
		Node* root=pq.top();
		pq.pop();
		string temp="";
		buildBits(root,temp);
	}



	string encodeTxt(string txt){
		string encoded_str="";
		
		for(auto ch:text){
				encoded_str+=bits[ch];
		}
		int paddingLength = 8 - (encoded_str.length() % 8);
        if (paddingLength != 8) { // If not already a multiple of 8
            encoded_str.append(paddingLength, '0'); // Pad with zeros
        }
		string paddingInfo= bitset<8>(paddingLength).to_string();
		string finalStr= paddingInfo+encoded_str;
		return finalStr;
	}
	
	vector<uint8_t> buildByteArray(string& encodeTxt){
		vector<uint8_t> temp;
		for(int i=0;i<encodeTxt.length();i+=8){
			string s=encodeTxt.substr(i,8);
			temp.push_back(static_cast<uint8_t>(std::bitset<8>(s).to_ulong()));
		}
		return temp;
	}

	void compression(){
		freqFromText(text);
		buildHeap();
		buildBinaryTree();
		bitsFromTree();

		string encoded_txt=encodeTxt(text);
		vector<uint8_t> byteArray=buildByteArray(encoded_txt);
	}


};



int main(){
	
	
	return 0;


}
