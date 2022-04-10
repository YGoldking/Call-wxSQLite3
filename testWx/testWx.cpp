// testWx.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <Windows.h>
#include <iostream>
#include <functional>
#include <vector>
#include <ctime>

using namespace std;

#define SQLITE_HAS_CODEC // SQLITE_HAS_CODEC
#include "sqlite3/sqlite3.h"

//#pragma comment(lib,"sqlite3/sqlite3.lib")
#ifdef _M_X64
//#include "sqlite3/sqlite3.h"
#pragma comment(lib,"sqlite3/sqlite3_x64.lib")
#else
//#include "sqlite3/sqlite3.h"
#pragma comment(lib,"sqlite3/sqlite3.lib")
#endif

#define DB_PATH1 "C:\\Users\\q123\\Desktop\\testWx\\db.db"

void query(sqlite3* db, const char* sql, function<bool(sqlite3_stmt*)> func) {
    sqlite3_stmt* stmt = NULL;
    const char* zTail = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, &zTail) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (func(stmt)) break;
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("query %s\n",sqlite3_errmsg(db));
    }
}

string UtfToString(string strValue)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, strValue.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1];//加上末尾'\0'
    ZeroMemory(pwBuf, nwLen * 2 + 2);
    ::MultiByteToWideChar(CP_ACP, 0, strValue.c_str(), strValue.length(), pwBuf, nwLen);
    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);
    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
    std::string retStr(pBuf);
    delete[]pwBuf;
    delete[]pBuf;
    pwBuf = NULL;
    pBuf = NULL;
    return retStr;
}
string StringToUtf(string strValue)
{

    int nwLen = MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1];//加上末尾'\0'
    memset(pwBuf, 0, nwLen * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), strValue.length(), pwBuf, nwLen);
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);
    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
    std::string retStr = pBuf;
    delete[]pBuf;
    delete[]pwBuf;
    return retStr;
}

int main()
{
    sqlite3* db = NULL;
    const char* db_Key = "123456";
    if (sqlite3_open(DB_PATH1, &db)) {
        printf("打开数据库失败！\n");
        db = NULL;
    }
    if (sqlite3_key(db, db_Key, strlen(db_Key)))
    {
        printf("[magic] key fail！\n");
    }
    clock_t start, end;
    start = clock();//计时开始
    vector<string> list;
    query(db, "SELECT * FROM keyword WHERE rowid >= (abs(random()) % (SELECT max(rowid) FROM keyword)) LIMIT 5000;", [&](sqlite3_stmt* stmt) -> bool {
        string content = string((const char*)sqlite3_column_text(stmt, 1));
        list.push_back(content);
        //printf("%s\n", StringToUtf(content).c_str());
        return false;
        });
    end = clock();//计时结束
    printf("list size: %d  耗时:%.2f s 1=%s\n",list.size(), (double)(end - start) / CLOCKS_PER_SEC, StringToUtf(list[1]).c_str());
    
    sqlite3_close(db);
    system("pause");
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
