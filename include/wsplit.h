#ifndef __WSPLIT__
#define __WSPLIT__
#pragma once
#include<vector>
#include<string>
//#include <experimental/string_view>

using std::vector;
using std::string;
using VS=vector<string>;


//vector<string> wsplit(string str, char separator = '\t' );
//vector<string> wsplits(string str, const string & seps );

namespace wsp {

template<typename ts>
vector<string> wSplit(string str, ts separator = '\t' )
{
    vector<int> vpos;
    vpos.push_back(0);
    char ch;
    for(uint i = 0; i < str.size(); i++){
	ch = str.at(i);
	if(ch == separator){
	    vpos.push_back(i+1);
	}
    }

//    string::size_type start = str.find_first_not_of( separator );
//    string::size_type end = 0;

    vector<string> vtokens{};

    int ini;
    int ancho;
    for(uint i = 0; i < vpos.size(); i++){
	ini = vpos[i];// (i==0?vpos[i]:vpos[i]+1);
	ancho =  vpos[i+1]-vpos[i]-1;
	vtokens.push_back(str.substr(ini, ancho));
    }
//    while ((end = str.find_first_of( separator, start)) != string::npos) {
//	vtokens.push_back(str.substr(start,end-start));
//	start = str.find_first_not_of( separator, end );
//    }
//    if( start != string::npos ){
//	vtokens.push_back(str.substr(start,str.length()));
//    }
    return vtokens;
}
///
///248906 elementos, 0.0322072s

// template<typename ts>
// vector<string> wSplit2(string str, ts separator, int ncampos )
// {
// 	string::size_type start = str.find_first_not_of( separator );
// 	string::size_type end = 0;
//
// 	vector<string> vtokens;
// 	while ((end = str.find_first_of( separator, start)) != string::npos) {
// 		vtokens.push_back(str.substr(start,end-start));
// 		start = str.find_first_not_of( separator, end );
// 	}
// 	if( start != string::npos ){
// 		vtokens.push_back(str.substr(start,str.length()));
// 	}
// 	if( vtokens.size () < static_cast<unsigned long>(ncampos) ){
// 		throw string("wSplit2: Error en extraccion de campos. "+
// 			     std::to_string(vtokens.size())+ " encontrados. Solocitados: "+
// 			     std::to_string(ncampos)+"\r\n"+str);
// 	}
// 	return vtokens;
// }

//template <typename T>
//inline std::vector<std::string_view> svsplit(const std::string& s, T delimiter)
//{
//    std::vector<std::string_view> result;

//    std::size_t current = 0;
//    std::size_t p = s.find_first_of(delimiter, 0);

//    std::string_view sv{s.c_str(), s.size()};
//    while (p != std::string::npos) {
//        //.substr(current, p - current);
//        std::string_view sv2{s.c_str()+current, p-current};
//        result.push_back(sv2);
//        current = p + 1;
//        p = s.find_first_of(delimiter, current);
//    }

//    result.push_back(sv.substr(current, p-current));//, current);

//    return result;
//}

///
///248906 elementos, 0.119489s
///
template <typename T>
inline std::vector<std::string> split(const std::string& s, T delimiter)
{
    std::vector<std::string> result;

    std::size_t current = 0;
    std::size_t p = s.find_first_of(delimiter, 0);

    while (p != std::string::npos){
	result.push_back(s.substr(current, p - current));
	current = p + 1;
	p = s.find_first_of(delimiter, current);
    }

    result.push_back(s.substr(current, p-current));

    return result;
}
vector<string> wsplit(string str, char separator );

vector<string> wsplits(string str, const std::string &seps );

vector<string> msplit(string str, string seps);
} // end namespace

#endif
