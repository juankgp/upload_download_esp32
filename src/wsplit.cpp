#include "wsplit.h"
#include <algorithm>

using ulong=unsigned long;
vector<std::string> wsp::wsplit(std::string str, char separator)
{
	vector<int> vpos;
	vpos.push_back(0);
	char ch;
	for(ulong i = 0; i < str.size(); i++){
		ch = str.at(i);
		if(ch == separator){
			vpos.push_back(i+1);
		}
	}

	vector<string> vtokens{};

	int ini;
	int ancho;
	for(ulong i = 0; i < vpos.size(); i++){
		ini = vpos[i];// (i==0?vpos[i]:vpos[i]+1);
		if(i < vpos.size()-1){
	    ancho = vpos[i+1]-vpos[i]-1;
	}else{
	    ancho = str.size() - vpos[i]; // ? -1;
	}
		//ancho =  vpos[i+1]-vpos[i]-1;
		vtokens.push_back(str.substr(ini, ancho));
	}

	return vtokens;
}

vector<std::string> wsp::wsplits(std::string str, const std::string &seps)
{
	vector<int> vpos;
	vpos.push_back(0);
	char ch;
	for(ulong i = 0; i < str.size(); i++){
		ch = str.at(i);
		if(string::npos != seps.find(ch)){
			vpos.push_back(i+1);
		}
	}

	vector<string> vtokens{};

	int ini;
	int ancho;
	for(ulong i = 0; i < vpos.size(); i++){
		ini = vpos[i];// (i==0?vpos[i]:vpos[i]+1);
		ancho =  vpos[i+1]-vpos[i]-1;
		vtokens.push_back(str.substr(ini, ancho));
	}

	return vtokens;
}
vector<string> wsp::msplit(string str, string seps)
{
	vector<string> vt;
	do {
		auto pos = str.find_last_of(seps);
		if(pos == string::npos){
			break;
		}
		vt.push_back(str.substr(pos+1));
		str.erase(pos);
	}while(true);
	if(!str.empty()){
		vt.push_back(str);
	}
	std::reverse(vt.begin(), vt.end());
	return vt;
}
