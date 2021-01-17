#include<iostream>
#include<vector>
using namespace std;

class permit{
public:
	string type;
	string fromip,toip;
};

class deny{
public:
	string type;
	string fromip,toip;
};

vector<permit> objp;
vector<deny> objd;
vector<permit> sp;
vector<deny> sd;

bool shadowp(permit obj){	//returns true if shadow rule is present
	for(auto it:objd){
		if(it.type == obj.type){
			if(it.fromip == "any" || obj.fromip == "any" || it.fromip == obj.fromip){
				if(it.toip == "any" || obj.toip == "any" || it.toip == obj.toip){
					return true;
				}
			}
		}
	}
	return false;
}

bool shadowd(deny obj){	//returns true if shadow rule is present
	for(auto it:objp){
		if(it.type == obj.type){
			if(it.fromip == "any" || obj.fromip == "any" || it.fromip == obj.fromip){
				if(it.toip == "any" || obj.toip == "any" || it.toip == obj.toip){
					return true;
				}
			}
		}
	}
	return false;
}

void sep_rule(string s){
	int n = s.size(),flag = 0;
	if(s[0] == 'p'){
		//permit tcp ip1 ip2
		//so, s[7] tells if it's udp or tcp
		permit obj;	//create new object
		if(s[7] == 't')	//=>tcp
			obj.type = "tcp";
		else	//=>udp
			obj.type = "udp";
		
		for(int i=11;i<n;i++){
			if(s[i] == ' '){
				flag = 1;
				continue;
			}if(flag == 0){
				obj.fromip += s[i];
			}else{
				obj.toip += s[i];
			}	
		}
		if(shadowp(obj))
			sp.push_back(obj);
		else
			objp.push_back(obj);
	}else{
		//deny tcp ip1 ip2
		//so, s[5] tells if it's udp or tcp
		deny obj;	//create new object
		if(s[5] == 't')	//=>tcp
			obj.type = "tcp";
		else	//=>udp
			obj.type = "udp";
		
		for(int i=9;i<n;i++){
			if(s[i] == ' '){
				flag = 1;
				continue;
			}if(flag == 0){
				obj.fromip += s[i];
			}else{
				obj.toip += s[i];
			}	
		}
		if(shadowd(obj))
			sd.push_back(obj);
		else
			objd.push_back(obj);
	}
}

int main(){
	vector<string> s(7);
	for(int  i=0;i<7;i++){
		getline(cin,s[i]);
		sep_rule(s[i]);
	}
	//s = "permit tcp 10.0.1.12 10.0.1.1";
	//s = "deny tcp 10.0.1.12 10.0.1.1";
	cout<<"\nrules in permit class:\n";
	for(auto it:objp){
		cout<<"permit "<<it.type<<" "<<it.fromip<<" "<<it.toip<<endl;
	}cout<<endl;
	cout<<"rules in deny class:\n";
	for(auto it:objd){
		cout<<"deny "<<it.type<<" "<<it.fromip<<" "<<it.toip<<endl;
	}cout<<endl;

	cout<<"rules in shadow permit class:\n";
	for(auto it:sp){
		cout<<"permit "<<it.type<<" "<<it.fromip<<" "<<it.toip<<endl;
	}cout<<endl;

	cout<<"rules in shadow deny class:\n";
	for(auto it:sd){
		cout<<"deny "<<it.type<<" "<<it.fromip<<" "<<it.toip<<endl;
	}cout<<endl;

	

	// for(auto it:)
}










