#pragma once

#include "mprpcconfig.hpp"

// mprpc框架的基础类，负责框架的一些初始化操作
class MprpcApplication
{
public:
    static void Init(int argc,char **argv); // 单例模式
    static MprpcApplication &GetInstance();
    static MprpcConfig& GetConfig();
private:
    static MprpcConfig m_config;

    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&) = delete;// 与拷贝相关的函数全部取消
    MprpcApplication(MprpcApplication&&) = delete;
};

