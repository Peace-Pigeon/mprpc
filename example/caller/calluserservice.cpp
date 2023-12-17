#include <iostream>
#include "mprpcapplication.hpp"
#include "user.pb.h"
#include "mprpcchannel.hpp"

int main(int argc,char** argv){
    // 整个程序启动后，想使用mprpc框架来享受rpc服务调用，一定要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc,argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    // rpc 方法的响应
    fixbug::LoginResponse response;
    // 发起rpc方法的调用，同步rpc调用过程 MprpcChannel::callmethod
    stub.Login(nullptr,&request,&response,nullptr);       // RpcChannel->RpcChannel::callMethod 集中来做所有的rpc方法调用的参数序列化和网络发送
    
    // 一次rpc调用完成，读取调用的结果
    if(0==response.result().errcode()){
        std::cout<<"rpc login response success:"<<response.success()<<std::endl;
    }else{
        std::cout<<"rpc login response fail:"<<response.result().errmsg()<<std::endl;
    }

    // 演示调用远程发布的rpc方法Register
    fixbug::RegisterRequest request2;
    request2.set_id(1000);
    request2.set_name("mprpc");
    request2.set_pwd("666666");
    // rpc 方法的响应
    fixbug::RegisterResponse response2;
    // 发起rpc方法的调用，同步rpc调用过程 MprpcChannel::callmethod
    stub.Register(nullptr,&request2,&response2,nullptr);       // RpcChannel->RpcChannel::callMethod 集中来做所有的rpc方法调用的参数序列化和网络发送

    // 一次rpc调用完成，读取调用的结果
    if(0==response2.result().errcode()){
        std::cout<<"rpc Register response success:"<<response2.success()<<std::endl;
    }else{
        std::cout<<"rpc Register response fail:"<<response2.result().errmsg()<<std::endl;
    }

    return 0;
}