#include <iostream>
#include "mprpcapplication.hpp"
#include "friend.pb.h"

int main(int argc,char** argv){
    // 整个程序启动后，想使用mprpc框架来享受rpc服务调用，一定要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc,argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());
    fixbug::GetFriendsListRequest request;
    request.set_userid(1000);
    // rpc 方法的响应
    fixbug::GetFriendsListResponse response;
    // 发起rpc方法的调用，同步rpc调用过程 MprpcChannel::callmethod
    MprpcController controller;
    stub.GetFriendsList(&controller,&request,&response,nullptr);       // RpcChannel->RpcChannel::callMethod 集中来做所有的rpc方法调用的参数序列化和网络发送
    
    // 一次rpc调用完成，读取调用的结果
    if(controller.Failed()){
        std::cout<<controller.ErrorText()<<std::endl;
    }else{
        if(0==response.result().errcode()){
            std::cout<<"rpc GetFriendsList response success:"<<std::endl;
            for(int i=0;i<response.friends_size();++i){
                std::cout<<"index: "<<i<<" name: "<<response.friends(i)<<std::endl;
            }
        }else{
            std::cout<<"rpc GetFriendsList response fail:"<<response.result().errmsg()<<std::endl;
        }
    }
    
    return 0;
}