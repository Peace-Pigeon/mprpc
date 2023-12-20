#include "zookeeperutil.hpp"
#include "mprpcapplication.hpp"
#include <semaphore.h>
#include <iostream>

// 全局的watcher观察者 watcher是一个回调函数，当zookeeper上的znode发生变化时，会通知客户端
void global_watcher(zhandle_t *zh,int type,int state,const char *path,void *watcherCtx){
    if(type==ZOO_SESSION_EVENT){        // 回调的消息类型是和会话相关的消息类型
        if(state==ZOO_CONNECTED_STATE){ // 表示zk客户端和zk服务器已经成功建立会话
            sem_t *sem=(sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient():m_zhandle(nullptr){
}

ZkClient::~ZkClient(){
    if(m_zhandle!=nullptr){
        zookeeper_close(m_zhandle); // 关闭句柄,释放资源
    }

}
// zkclient启动，和zkserver建立连接
void ZkClient::Start(){
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;
    std::cout << "zookeeper connstr:" << connstr << std::endl;

    // 创建一个与zookeeper服务器的连接
    /**
     * zookeeper_mt是多线程版本的zookeeper客户端，zookeeper_st是单线程版本的zookeeper客户端
     * zookeeper的API客户端程序提供了三个线程
     * API调用线程：调用zookeeper的API接口的线程，也就是我们的业务线程
     * 网络I/O线程：负责和zookeeper服务器进行网络通信的线程
     * watcher回调线程：负责watcher回调的线程
    */
    m_zhandle = zookeeper_init(connstr.c_str(),global_watcher,30000,nullptr,nullptr,0);
    if(m_zhandle==nullptr){
        std::cout<<"zookeeper_init error!"<<std::endl;
        exit(EXIT_FAILURE);
    }
    sem_t sem;
    sem_init(&sem,0,0);
    // 表示为句柄添加一些上下文信息，这些上下文信息会在watcher回调函数中使用
    zoo_set_context(m_zhandle,&sem);
    sem_wait(&sem);
    std::cout<<"zookeeper_init success!"<<std::endl;

}
// 在zkserver上根据指定的path创建一个znode节点
void ZkClient::Create(const char* path,const char* data,int datalen,int state){
    char path_buffer[128];
    int bufferlen=sizeof(path_buffer);
    // 先判断path表示的znode节点时候存在,如果存在,就不再重复创建了
    int flag=zoo_exists(m_zhandle,path,0,nullptr);
    if(flag==ZNONODE){ // 表示指定的节点不存在
        // ZOO_EPHEMERAL 表示创建的是一个临时节点
        // ZOO_SEQUENCE 表示创建的是一个序列节点
        flag=zoo_create(m_zhandle,path,data,datalen,&ZOO_OPEN_ACL_UNSAFE,state,path_buffer,bufferlen);
        if(flag==ZOK){
            std::cout<<"create znode success! path= "<<path<<std::endl;
        }else{
            std::cout<<"flag: "<<flag<<std::endl;
            std::cout<<"znode create error... path= "<<path<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
}
// 根据参数指定的znode获取对应的znode节点的值
std::string ZkClient::GetData(const char* path){
    char buf[64];
    int buflen=sizeof(buf);
    int flag=zoo_get(m_zhandle,path,0,buf,&buflen,nullptr);
    if(flag!=ZOK){
        std::cout<<"get znode data error! path: "<<path<<std::endl;
        return "";
    }else return buf;
}