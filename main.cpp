#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
//TODO 定义运算符到数的映射

int cnt,idx[26],chmp[128];
std::string str;
std::ofstream ostrm("oput.csv");
// !:2 &:3 |:4 >:5 =:6
// 不合法时输入框变红
// 如果有优先级 则从高到低加括号
// 对双目运算符内嵌真值表打表出答案

bool stats[][2][2]={
	{0,0},{0,1}, // &
	{0,1},{1,1}, // |
	{1,1},{0,1}, // >
	{1,0},{0,1} // =
};

bool isalpha(char c){
	return c>='A'&&c<='Z';
}
bool isop(char c){
	return c=='!'||c=='&'||c=='|'||c=='>'||c=='=';
}
bool isst(char c){ // 合法开头
	return isalpha(c)||c=='('||c=='!';
}
bool valid(char c){
	return isop(c)||isalpha(c)||c=='('||c==')';
}
template <class T> void vec_clr(std::vector<T> v){
	for(auto &a:v) a=0;
}

void get_pre(int &d){
	if(isalpha(str[d])) return --d,void();
	assert(str[d--]==')');
	while(d>=0&&d!='(') get_pre(--d);
	assert(d>=0);
}

bool get_nx(int &d,int stat){
	assert(d<(int)str.size()&&isst(str[d]));
	if(isalpha(str[d])) return stat>>(idx[d]-1)&1;
	bool res;
	if(str[d]=='!') res=!get_nx(++d,stat);
	else res=get_nx(++d,stat);
	while(d<(int)str.size()&&d!=')'){//WIP
		char c=str[d++];
		res=stats[res][get_nx(d,stat)];
	}
	assert(d<(int)str.size());
	return res;
}

bool init(int &d){
	if(d>=(int)str.size()||!isst(str[d])) return 0;
	if(isalpha(str[d])){
		ostrm<<str[d]<<',';
		return d++;
	}
	int org_d=d;
	if(!init(++d)) return 0;
	while(d<(int)str.size()&&d!=')'){
		char c=str[d++];
		if(!isop(c)||!init(d)) return 0;
	}
	ostrm<<str[d];
	if(org_d==0) ostrm<<std::endl;
	else ostrm<<',';
}

void add_br(std::string &str,const std::vector<int> pl,const std::vector<int> &pr){
	std::string tmp="";
	for(int i=0; i<(int)str.size(); ++i){
		tmp+=i;
		for(int j=pr[i]; j--; ) tmp+=')';
		for(int j=pl[i]; j--; ) tmp+='(';
	}
	str=tmp;
}

void preproc(std::string &str){
	std::vector<int> pl,pr;
	str="("+str+")";
	for(char &c:str){
		if(c>='a'&&c<='z') c-='a'-'A';
		if(!idx[c-'A']) idx[c-'A']=++cnt;
	}
	pl.resize(str.length());
	pr.resize(str.length());
	for(int i=3; i<=6; ++i){
		pl.resize(str.length());
		pr.resize(str.length());
		pl.clear(),pr.clear();
	}
}

int main(){
	// 原子命题为大写或小写字母，大小写字母等价
	// !,&,|,>,=
	chmp['&']=0,chmp['|']=1;
	chmp['>']=2,chmp['=']=3;
	while(std::getline(std::cin,str)){
		std::remove(str.begin(),str.end(),' ');
		int d=0;
		if(!init(d)){
			puts("INVALID INPUT.");
			continue;
		}
		preproc(str); // 去除空格 根据优先级加上括号
	}
	return 0;
}
