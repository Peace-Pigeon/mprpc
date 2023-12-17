#include <iostream>
#include "friend.pb.h"
#include "mprpcapplication.hpp"
#include "rpcprovider.hpp"
#include <vector>
#include "logger.hpp"

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendsList(uint32_t userid){
        std::cout<<"do GetFriendsList service! usrid: "<<userid<<std::endl;
        std::vector<std::string> vec;
        vec.push_back("zhang san");
        vec.push_back("li si");
        vec.push_back("wang wu");
        return vec;
    }
    void GetFriendsList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendsListRequest* request,
                       ::fixbug::GetFriendsListResponse* response,
                       ::google::protobuf::Closure* done){
        uint32_t userid = request->userid();
        std::vector<std::string> friendsList = GetFriendsList(userid);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");

        for(std::string& name : friendsList){
            std::string *p=response->add_friends();
            *p=name;
        }
        done->Run();
    }
private:
    /* data */
};

int main(int argc,char **argv){

    LOG_INFO("hello world! log message");
    LOG_ERROR("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
    // 调用框架的初始化操作
    MprpcApplication::Init(argc,argv);
    
    //provider是一个rpc网络服务对象。把UserService对象发布到rpc节点上
    RcpProvider provider;
    provider.NotifyService(new FriendService());

    // 启动一个rpc服务发布节点 Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}