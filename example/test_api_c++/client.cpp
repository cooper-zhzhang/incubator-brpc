// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

// A client sending requests to server every 1 second.

#include <gflags/gflags.h>
#include <butil/logging.h>
#include <butil/time.h>
#include <brpc/channel.h>
#include <bthread/work_stealing_queue.h>
#include <bthread/timer_thread.h>
#include <butil/resource_pool.h>
#include <butil/resource_pool_inl.h>

DEFINE_int32(count, 1000, "Milliseconds between consecutive requests");

void WorkStealingQueue()
{
    bthread::WorkStealingQueue<int> que;

    que.init(8);

    for (int i = 0; i < FLAGS_count; i++)
    {
        que.push(i);
    }

}

void TimerFunc(void *p)
{
    std::cout << "Timer Func " << std::endl;
}

void TestTimerThread()
{
    bthread::TimerThread *pTimerThread = bthread::get_or_create_global_timer_thread();
    if(!pTimerThread)
    {
        std::cout << "timer thread is null";
        return ;
    }

    std::cout << "start timer thread " << bthread::get_global_timer_thread()->thread_id() << std::endl;

    bthread::get_global_timer_thread()->schedule(
            TimerFunc, NULL,
            butil::microseconds_from_now(1 * 1000000L));

    pthread_t id = bthread::get_global_timer_thread()->thread_id();

    std::cout << id << std::endl;// 不知道为什么取出来的全是0

    int joinRet = pthread_join(bthread::get_global_timer_thread()->thread_id(), NULL);
    std::cout << "join thread ret " << joinRet << std::endl;

    //pTimerThread->stop_and_join();

    //pthread_join(_thread, NULL);
    sleep(10);

    /*
    bthread::TimerThread timerThread;
    int ret = timerThread.start(NULL);
    if(ret != 0)
    {
        LOG_ASSERT(ret);
        return; 
    }

    std::cout << "timer thread id = " << timerThread.thread_id() << std::endl;*/

}

void ResourcePool()
{

    butil::ResourceId<int> id;
    //template <typename T> inline T* 
    int *pValue =  get_resource(&id, 1);
    if(pValue == NULL)
    {
        std::cout << "get_resource is null" << std::endl;
    }

    //std::cout << *pValue << std::endl;

    *pValue = 12;

    pValue = address_resource(id);


    //pValue =  get_resource(&id, 2);


    if (pValue)
    {
        std::cout << " cur id is exists" << std::endl;
    }
    
    //ResourcePool<T>::singleton()->get_resource(id);
}

int main(int argc, char* argv[]) {

    #ifdef TETST
    WorkStealingQueue();

    TestTimerThread();
    #endif

    ResourcePool();



    return 0;
}


