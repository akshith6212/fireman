// function to segregate the rules into categories // permit/deny tcp/udp etc..

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
		objd.push_back(obj);
	}
}

int main(){
	string s;getline(cin,s);
	//s = "permit tcp 10.0.1.12 10.0.1.1";
	sep_rule(s);
	//cout<<objp[0].type<<" "<<objp[0].fromip<<" "<<objp[0].toip<<endl;
	//cout<<objd[0].type<<" "<<objd[0].fromip<<" "<<objd[0].toip<<endl;
	if(objp.size() != 0){
		cout<<"The rules says to permit the packet and..."<<endl;
		cout<<"The packet type of the rule is: "<<objp[0].type<<endl;
		cout<<"The fromip of the rule is: "<<objp[0].fromip<<endl;
		cout<<"The toip of rule is: "<<objp[0].toip<<endl;
	}else{
		cout<<"The rules says to deny the packet and..."<<endl;
		cout<<"The packet type of the rule is: "<<objd[0].type<<endl;
		cout<<"The fromip of the rule is: "<<objd[0].fromip<<endl;
		cout<<"The toip of rule is: "<<objd[0].toip<<endl;
	}
		
}











