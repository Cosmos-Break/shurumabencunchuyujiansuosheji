// 输入码本存储与检索设计.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include <Windows.h>
#include <algorithm>
#include <set>
#include <unordered_map>
using namespace std;

wstring transfer(string sBuf, UINT CodePage) {
	//获取输入缓存大小
	int sBufSize = sBuf.length();

	//CodePage选择代码页 936:GBK   950:BIG-5
	DWORD dBufSize = MultiByteToWideChar(CodePage, 0, sBuf.c_str(), sBufSize, NULL, 0);
	wchar_t * dBuf = new wchar_t[dBufSize];
	wmemset(dBuf, 0, dBufSize);

	//进行转换
	int nRet = MultiByteToWideChar(CodePage, 0, sBuf.c_str(), sBufSize, dBuf, dBufSize);
	return (wstring)dBuf;
}

bool myfunction(string i, string j) { return (i < j); }
int srm2hz(string srm, string &hz, unordered_map<string,string> &map) 
{
	int ret = 0;
	auto pos = map.find(srm);
	if (pos == map.end()) {
		ret = 0;
	}
	else {
		hz = pos->second;
		ret = hz.length() / 2;
	}
	return ret;
}



int main()
{
	ifstream infile("zici_ANSI.TXT");
	vector<string> pinyin(60000);
	vector<string> zi(60000);
	int pinyinCount = 0;
	int ziCount = 0;
	/*pinyin[0] = "a";
	zi[0] = "啊";*/
	string str;
	while (getline(infile, str)) {
		if (IsDBCSLeadByte(str[2]))
			continue;
		/*zi[ziCount] = str.substr(0, 2);
		pinyin[pinyinCount] = str.substr(2);*/
		istringstream iss(str.substr(2));
		string duoyin;
		string hanzi = str.substr(0, 2);
		while (iss >> duoyin) {
			zi[ziCount] = hanzi;
			pinyin[pinyinCount] = duoyin;
			ziCount++;
			pinyinCount++;
		}
		/*ziCount++;
		pinyinCount++;*/
	}
	ofstream outfile("zi.txt");
	for (int i = 0,j = 0; i < ziCount && j< pinyinCount; i++,j++) {
		outfile << pinyin[j] << zi[i] << endl;
	}
	vector<string> pinyinAndZi(50000);
	for (int i = 0; i < pinyinCount; i++) {
		pinyinAndZi[i] = pinyin[i] + zi[i];
	}
	pinyinAndZi.resize(pinyinCount);
	sort(pinyinAndZi.begin(), pinyinAndZi.end());//按照zi.txt前面的拼音来排序
	set<string> s;//用set来去除重复后重新dump进vector
	unsigned size = pinyinAndZi.size();
	for (unsigned i = 0; i < size; ++i) s.insert(pinyinAndZi[i]);
	pinyinAndZi.assign(s.begin(), s.end());
	ofstream outfile2("zi2.txt");
	for (unsigned int i = 0; i < pinyinAndZi.size(); i++) {
		outfile2 << pinyinAndZi[i] << endl;
	}
	pinyin.clear();
	zi.clear();
	pinyin.resize(40000);
	zi.resize(40000);
	for (unsigned int i = 0; i < pinyinAndZi.size(); i++) {
		size_t t = pinyinAndZi[i].length() - 2;
		string part1 = pinyinAndZi[i].substr(0, t);
		string part2 = pinyinAndZi[i].substr(t, 2);
		pinyin[i] = part1;
		zi[i] = part2;
	}
	vector<string> pinyinUnique(50000);
	vector<string> ziCandidateStr(50000);
	int pinyinUniqueCount = 0;
	int ziCandidateStrCount = 0;
	pinyinUnique[0] = "0";
	ziCandidateStr[0] = "0";
	for (unsigned int i = 0; i < pinyin.size(); i++) {
		if (pinyin[i] != pinyinUnique[pinyinUniqueCount]) {
			pinyinUniqueCount++;
			pinyinUnique[pinyinUniqueCount] = pinyin[i];
			ziCandidateStrCount++;
			ziCandidateStr[ziCandidateStrCount] += zi[i];
		}
		else {
			ziCandidateStr[ziCandidateStrCount] += zi[i];
		}
	}
	ofstream outfilezimb("zimb.txt");
	for (int i = 1; i < pinyinUniqueCount; i++) {
		outfilezimb << pinyinUnique[i] << ziCandidateStr[i] << endl;
	}

	unordered_map<string, string> map;
	for (int i = 1; i < pinyinUniqueCount; i++) {
		map.insert(make_pair(pinyinUnique[i], ziCandidateStr[i]));
	}
	
	/*vector<string> finalPinyin(20000);
	vector<string> finalString(20000);
	int finalPinyinCount = 0;
	int finalStringCount = 0;
	for (int i = 1; i < pinyinUniqueCount; i++) {
		string candidateStr = ziCandidateStr[i];
		for (int j = 2; j < pinyinUniqueCount; j++) {
			if (pinyinUnique[i] == pinyinUnique[j]) {
				candidateStr += ziCandidateStr[j];
		
			}

		}

	}*/
	
	


}
