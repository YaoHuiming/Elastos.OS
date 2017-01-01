//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#include <threadpoolexecutor.h>

namespace Elastos {
namespace RPC {

AutoPtr<ThreadPoolExecutor> ThreadPoolExecutor::sInstance;
android::Mutex ThreadPoolExecutor::sInstanceLock;

AutoPtr<ThreadPoolExecutor> ThreadPoolExecutor::Self()
{
    Prepare();
    return sInstance;
}

void ThreadPoolExecutor::Prepare()
{
    android::AutoMutex _l(sInstanceLock);
    if (sInstance == NULL) {
        sInstance = new ThreadPoolExecutor();
    }
}

ECode ThreadPoolExecutor::RunTask(
    /* [in] */ Runnable* task)
{
    AutoPtr<Worker> w = new Worker(task, this);
    {
        android::AutoMutex _l(w->mLock);

        pthread_attr_t threadAddr;
        pthread_attr_init(&threadAddr);
        pthread_attr_setdetachstate(&threadAddr, PTHREAD_CREATE_DETACHED);

        pthread_t thread;
        int ret = pthread_create(&thread, &threadAddr, ThreadPoolExecutor::ThreadEntry, (void*)w);
        if (ret != 0) {
            return E_FAIL;
        }

        w->mThread = thread;
        {
            android::AutoMutex _l(mMainLock);
            mWorkers.add(w);
        }
    }
    return NOERROR;
}

ECode ThreadPoolExecutor::StopTask(
    /* [in] */ Runnable* task)
{
    android::AutoMutex _l(mMainLock);

    android::Vector< AutoPtr<Worker> >::iterator iter;
    for (iter = mWorkers.begin(); iter != mWorkers.end(); ++iter) {
        AutoPtr<Worker> w = *iter;
        if (w->mTask.Get() == task) {
            if (pthread_kill(w->mThread, SIGUSR2) == ESRCH) {
                ALOGE("ThreadPoolExecutor::pthread_kill failed");
            }
            mWorkers.erase(iter);
            return NOERROR;
        }
    }
    return NOERROR;
}

void* ThreadPoolExecutor::ThreadEntry(void* arg)
{
    AutoPtr<Worker> w = (Worker*)arg;
    {
        android::AutoMutex _l(w->mLock);
        assert(w->mThread != 0);
    }

    ECode ec = w->Run();

    {
        ThreadPoolExecutor* executor = w->mOwner;
        android::AutoMutex _l(executor->mMainLock);
        for (size_t i = 0; i < executor->mWorkers.size(); ++i) {
            if (executor->mWorkers[i] == w) {
                executor->mWorkers.removeAt(i);
                break;
            }
        }
    }

    return (void*)ec;
}

} // namespace RPC
} // namespace Elastos
