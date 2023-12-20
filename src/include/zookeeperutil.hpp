#pragma once
#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

// 用来为rpc服务提供者提供服务发布的
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    // zkclient启动，和zkserver建立连接
    void Start();
    // 在zkserver上根据指定的path创建一个znode节点
    void Create(const char* path,const char* data,int datalen,int state);
    // 根据参数指定的znode获取对应的znode节点的值
    std::string GetData(const char* path);
private:
    // zookeeper客户端的句柄
    zhandle_t *m_zhandle;
};

