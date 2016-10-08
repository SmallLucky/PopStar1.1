#pragma  once

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class DataSava
{
public:
	static DataSava* getInstance();
	static bool purgeInstance();

	rapidjson::Document preparePrase();
	void praseFinish(rapidjson::Document &document);

	void copyFileToPath(std::string path);
	void praseJsonData();

private:
	DataSava();
	~DataSava();

public:

	std::vector< int >  getStarColorList();		//从json文件获取数据
	void  setStarColor(int _value);				//添加数据到json文件

	std::vector< int >  getStarIndexIList();
	void setStarIndexI(int _value);

	std::vector< int >  getStarIndexJList();
	void setStarIndexJ(int _value);

	void clearJsonData();

private:
	std::vector< int > colorList;
	std::vector< int > indexIList;
	std::vector< int > indexJList;

private:
	string path;
	static DataSava*_instance;
};
