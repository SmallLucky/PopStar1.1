#include "DataSave.h"

DataSava*DataSava::_instance = nullptr;

DataSava::DataSava()
{

}

DataSava::~DataSava()
{
	colorList.clear();
	indexIList.clear();
	indexJList.clear();
}

DataSava* DataSava::getInstance()
{
	if (!_instance)
	{
		_instance = new DataSava();
	}
	return _instance;
}

bool DataSava::purgeInstance()
{
	CCASSERT(_instance, "instance can not emptoy");
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
	return true;
}

rapidjson::Document DataSava::preparePrase()
{
	FileUtils *fu = FileUtils::getInstance();
	path = fu->getWritablePath();
	string filename = "DataSave.json";
	path += filename;
	this->copyFileToPath(filename);

	rapidjson::Document doc;
	std::string data = fu->getStringFromFile(path);
	std::string clearData(data);
	size_t pos = clearData.rfind("}");
	clearData = clearData.substr(0, pos + 1);

	doc.Parse<4>(clearData.c_str());

	if (doc.HasParseError()) {
		log("GetParaseErro %s\n", doc.GetParseError());
		return NULL;
	}

	return doc;
}

void DataSava::praseFinish(rapidjson::Document &document)
{
	////修改json内容之后重新写入
	rapidjson::StringBuffer buffer;			//初始化缓冲区
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);//初始化写入器
	document.Accept(writer);					//将doc里面的内容放到writer里面的buffer

	FILE* file = fopen(path.c_str(), "wb");//二进制写入
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
		cout << "解析完成" << endl;
	}
}

void DataSava::copyFileToPath(std::string path)
{
	//获取文件复制的原路径和目录路径
	FileUtils *fu = FileUtils::getInstance();
	std::string wrpath = fu->getWritablePath();
	wrpath += path;
	log("%s", wrpath.c_str());
	if (!fu->isFileExist(wrpath)) {
		//获取app里面的路径
		std::string datapath = fu->fullPathForFilename(path.c_str());
		log("datapath=%s", datapath.c_str());
		//根据这个路径来获取到文件
		std::string pFileContent = fu->getStringFromFile(datapath);
		CCLOG("%s", pFileContent.c_str());
		//将文件内容读出，写入到沙盒中
		FILE *file = fopen(wrpath.c_str(), "w");
		if (file) {
			fputs(pFileContent.c_str(), file);
			fclose(file);
		}
	}
}

void DataSava::praseJsonData()
{
	colorList.clear();
	indexIList.clear();
	indexJList.clear();

	auto doc = preparePrase();
	
	rapidjson::Value &chapter = doc["chapter"];
	//三个数组
	rapidjson::Value &starColor = chapter["starColor"];
	rapidjson::Value &starIndexI = chapter["starIndexI"];
	rapidjson::Value &StarIndexJ = chapter["StarIndexJ"];
	
	//starColor.PushBack("1");	//写入数据
	//starColor.AddMember();

	//starColor.Clear();		//清除数组
	//starColor.SetArray();		//清除数组
	
	for (int i = 0; i < starColor.Size();i++)
	{
		rapidjson::Value &value = starColor[i];
		colorList.push_back(value.GetInt());
	}
	for (int i = 0; i < starIndexI.Size(); i++)
	{
		rapidjson::Value &value = starIndexI[i];
		indexIList.push_back(value.GetInt());
	}
	for (int i = 0; i < StarIndexJ.Size(); i++)
	{
		rapidjson::Value &value = StarIndexJ[i];
		indexJList.push_back(value.GetInt());
	}
	cout << "---------------------------" << endl;
	printf("color==%d\n", colorList.size());
	printf("indexI==%d\n", indexIList.size());
	printf("indexJ==%d\n", indexJList.size());

	cout << "---------------------------" << endl;
	
	praseFinish(doc);
}

std::vector< int > DataSava::getStarColorList()
{
	if (!colorList.empty())
	{
		return colorList;
	}
	return std::vector<int >();
}

std::vector< int > DataSava::getStarIndexIList()
{
	if (!indexIList.empty())
	{
		return indexIList;
	}
	return std::vector<int >();
}

std::vector< int > DataSava::getStarIndexJList()
{
	if (!indexJList.empty())
	{
		return indexJList;
	}
	return std::vector<int >();
}

void DataSava::clearJsonData()
{
	colorList.clear();
	indexIList.clear();
	indexJList.clear();

	auto doc = preparePrase();

	rapidjson::Value &chapter = doc["chapter"];
	//三个数组
	rapidjson::Value &starColor = chapter["starColor"];
	rapidjson::Value &starIndexI = chapter["starIndexI"];
	rapidjson::Value &StarIndexJ = chapter["StarIndexJ"];

	starColor.Clear();		//清除数组
	starIndexI.Clear();
	StarIndexJ.Clear();

	praseFinish(doc);
}

void  DataSava::setStarColor(int _value)
{
	auto doc = preparePrase();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value &chapter = doc["chapter"];
	rapidjson::Value &starColor = chapter["starColor"];

	starColor.PushBack(_value, allocator);		//添加数据

	praseFinish(doc);
}

void DataSava::setStarIndexI(int _value)
{
	auto doc = preparePrase();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value &chapter = doc["chapter"];
	rapidjson::Value &starIndexI = chapter["starIndexI"];

	starIndexI.PushBack(_value, allocator);		//添加数据

	praseFinish(doc);
}

void DataSava::setStarIndexJ(int _value)
{
	auto doc = preparePrase();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value &chapter = doc["chapter"];
	rapidjson::Value &StarIndexJ = chapter["StarIndexJ"];

	StarIndexJ.PushBack(_value, allocator);		//添加数据
	praseFinish(doc);
}