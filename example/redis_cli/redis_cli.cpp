#include <brpc/redis.h>
#include <brpc/channel.h>

void setCommand(std::string &key, int value)
{


}


int main(int argc, char *argv[])
{

    brpc::ChannelOptions options;
    options.protocol = brpc::PROTOCOL_REDIS;
    brpc::Channel redis_channel;
    if (redis_channel.Init("0.0.0.0:6379", &options) != 0)
    { // 6379是redis-server的默认端口
        LOG(ERROR) << "Fail to init channel to redis-server";
        return -1;
    }

    std::string my_key = "my_key_1";
    int my_number = 1;
        // 执行"SET <my_key> <my_number>"

    brpc::RedisRequest set_request;
    brpc::RedisResponse response;
    brpc::Controller cntl;
    set_request.AddCommand("SET %s %d", my_key.c_str(), my_number);
    redis_channel.CallMethod(NULL, &cntl, &set_request, &response, NULL /*done*/);
    if (cntl.Failed())
    {
        LOG(ERROR) << "Fail to access redis-server";
        return -1;
    }
    // 可以通过response.reply(i)访问某个reply
    if (response.reply(0).is_error())
    {
        LOG(ERROR) << "Fail to set";
        return -1;
    }

    // 可用多种方式打印reply
    LOG(INFO) << response.reply(0).c_str() // OK
              << response.reply(0)         // OK
              << response;                 // OK

    // 第二次set
    cntl.Reset();
    set_request.AddCommand("SET %s %d", "my_key_2", 9283);
    redis_channel.CallMethod(NULL, &cntl, &set_request, &response, NULL /*done*/);
    if (cntl.Failed())
    {
        LOG(ERROR) << "Fail to access redis-server";
        return -1;
    }


    //std::string my_all_key = "my_key_2";
    std::string my_all_key = "my_key_2 my_key_1";
    brpc::RedisRequest mget_request;

    cntl.Reset();
    //mget_request.AddCommand("mget %s", my_all_key.c_str());
    mget_request.AddCommand(std::string("mget ") + my_all_key);
    //mget_request.AddCommand("mget %s %s", "my_key_2", "my_key_1");

    redis_channel.CallMethod(NULL, &cntl, &mget_request, &response, NULL /*done*/);

    if (cntl.Failed())
    {
        LOG(ERROR) << "Fail to access redis-server";
        return -1;
    }

    // 可以通过response.reply(i)访问某个reply
    if (response.reply(0).is_error())
    {
        LOG(ERROR) << "Fail to set";
        return -1;
    }

    // 可用多种方式打印reply
    LOG(INFO) << response.reply(0) // OK
              << response.reply(0)         // OK
              << response;                 // OK

    return 0;
}
