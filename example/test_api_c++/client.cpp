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

void TestTimerThread()
{
    bthread::TimerThread timerThread;
    timerThread.start(NULL);


}

int main(int argc, char* argv[]) {

    WorkStealingQueue();

    TestTimerThread();


    return 0;
}


