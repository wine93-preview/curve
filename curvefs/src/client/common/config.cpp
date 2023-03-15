/*
 *  Copyright (c) 2021 NetEase Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Project: curve
 * Created Date: Thur May 27 2021
 * Author: xuchaojie
 */

#include "curvefs/src/client/common/config.h"

#include <gflags/gflags.h>

#include <string>
#include <vector>

#include "src/common/gflags_helper.h"
#include "src/common/string_util.h"

namespace brpc {
DECLARE_int32(defer_close_second);
DECLARE_int32(health_check_interval);
}  // namespace brpc

namespace curvefs {
namespace client {
namespace common {
DECLARE_bool(useFakeS3);
}  // namespace common
}  // namespace client
}  // namespace curvefs

namespace curvefs {
namespace client {
namespace common {
DEFINE_bool(enableCto, true, "acheieve cto consistency");
DEFINE_bool(useFakeS3, false,
            "Use fake s3 to inject more metadata for testing metaserver");
DEFINE_bool(supportKVcache, false, "use kvcache to speed up sharing");

void InitMdsOption(Configuration *conf, MdsOption *mdsOpt) {
    conf->GetValueFatalIfFail("mdsOpt.mdsMaxRetryMS", &mdsOpt->mdsMaxRetryMS);
    conf->GetValueFatalIfFail("mdsOpt.rpcRetryOpt.maxRPCTimeoutMS",
                              &mdsOpt->rpcRetryOpt.maxRPCTimeoutMS);
    conf->GetValueFatalIfFail("mdsOpt.rpcRetryOpt.rpcTimeoutMs",
                              &mdsOpt->rpcRetryOpt.rpcTimeoutMs);
    conf->GetValueFatalIfFail("mdsOpt.rpcRetryOpt.rpcRetryIntervalUS",
                              &mdsOpt->rpcRetryOpt.rpcRetryIntervalUS);
    conf->GetValueFatalIfFail(
        "mdsOpt.rpcRetryOpt.maxFailedTimesBeforeChangeAddr",
        &mdsOpt->rpcRetryOpt.maxFailedTimesBeforeChangeAddr);
    conf->GetValueFatalIfFail(
        "mdsOpt.rpcRetryOpt.normalRetryTimesBeforeTriggerWait",
        &mdsOpt->rpcRetryOpt.normalRetryTimesBeforeTriggerWait);
    conf->GetValueFatalIfFail("mdsOpt.rpcRetryOpt.waitSleepMs",
                              &mdsOpt->rpcRetryOpt.waitSleepMs);
    std::string adds;
    conf->GetValueFatalIfFail("mdsOpt.rpcRetryOpt.addrs", &adds);

    std::vector<std::string> mdsAddr;
    curve::common::SplitString(adds, ",", &mdsAddr);
    mdsOpt->rpcRetryOpt.addrs.assign(mdsAddr.begin(), mdsAddr.end());
}

void InitMetaCacheOption(Configuration *conf, MetaCacheOpt *opts) {
    conf->GetValueFatalIfFail("metaCacheOpt.metacacheGetLeaderRetry",
                              &opts->metacacheGetLeaderRetry);
    conf->GetValueFatalIfFail("metaCacheOpt.metacacheRPCRetryIntervalUS",
                              &opts->metacacheRPCRetryIntervalUS);
    conf->GetValueFatalIfFail("metaCacheOpt.metacacheGetLeaderRPCTimeOutMS",
                              &opts->metacacheGetLeaderRPCTimeOutMS);
}

void InitExcutorOption(Configuration *conf, ExcutorOpt *opts, bool internal) {
    if (internal) {
        conf->GetValueFatalIfFail("executorOpt.maxInternalRetry",
                                  &opts->maxRetry);
    } else {
        conf->GetValueFatalIfFail("executorOpt.maxRetry", &opts->maxRetry);
    }

    conf->GetValueFatalIfFail("executorOpt.retryIntervalUS",
                              &opts->retryIntervalUS);
    conf->GetValueFatalIfFail("executorOpt.rpcTimeoutMS", &opts->rpcTimeoutMS);
    conf->GetValueFatalIfFail("executorOpt.rpcStreamIdleTimeoutMS",
                              &opts->rpcStreamIdleTimeoutMS);
    conf->GetValueFatalIfFail("executorOpt.maxRPCTimeoutMS",
                              &opts->maxRPCTimeoutMS);
    conf->GetValueFatalIfFail("executorOpt.maxRetrySleepIntervalUS",
                              &opts->maxRetrySleepIntervalUS);
    conf->GetValueFatalIfFail("executorOpt.minRetryTimesForceTimeoutBackoff",
                              &opts->minRetryTimesForceTimeoutBackoff);
    conf->GetValueFatalIfFail("executorOpt.maxRetryTimesBeforeConsiderSuspend",
                              &opts->maxRetryTimesBeforeConsiderSuspend);
    conf->GetValueFatalIfFail("executorOpt.batchInodeAttrLimit",
                              &opts->batchInodeAttrLimit);
    conf->GetValueFatalIfFail("fuseClient.enableMultiMountPointRename",
                              &opts->enableRenameParallel);
}

void InitBlockDeviceOption(Configuration *conf,
                           BlockDeviceClientOptions *bdevOpt) {
    conf->GetValueFatalIfFail("bdev.confPath", &bdevOpt->configPath);
}

void InitDiskCacheOption(Configuration *conf,
                         DiskCacheOption *diskCacheOption) {
    uint32_t diskCacheType;
    conf->GetValueFatalIfFail("diskCache.diskCacheType",
                              &diskCacheType);
    diskCacheOption->diskCacheType = (DiskCacheType)diskCacheType;
    conf->GetValueFatalIfFail("diskCache.forceFlush",
                              &diskCacheOption->forceFlush);
    conf->GetValueFatalIfFail("diskCache.cacheDir", &diskCacheOption->cacheDir);
    conf->GetValueFatalIfFail("diskCache.trimCheckIntervalSec",
                              &diskCacheOption->trimCheckIntervalSec);
    conf->GetValueFatalIfFail("diskCache.asyncLoadPeriodMs",
                              &diskCacheOption->asyncLoadPeriodMs);
    conf->GetValueFatalIfFail("diskCache.fullRatio",
                              &diskCacheOption->fullRatio);
    conf->GetValueFatalIfFail("diskCache.safeRatio",
                              &diskCacheOption->safeRatio);
    conf->GetValueFatalIfFail("diskCache.maxUsableSpaceBytes",
                              &diskCacheOption->maxUsableSpaceBytes);
    conf->GetValueFatalIfFail("diskCache.maxFileNums",
                              &diskCacheOption->maxFileNums);
    conf->GetValueFatalIfFail("diskCache.cmdTimeoutSec",
                              &diskCacheOption->cmdTimeoutSec);
    conf->GetValueFatalIfFail("diskCache.threads",
                              &diskCacheOption->threads);
    conf->GetValueFatalIfFail("diskCache.avgFlushBytes",
                              &diskCacheOption->avgFlushBytes);
    conf->GetValueFatalIfFail("diskCache.burstFlushBytes",
                              &diskCacheOption->burstFlushBytes);
    conf->GetValueFatalIfFail("diskCache.burstSecs",
                              &diskCacheOption->burstSecs);
    conf->GetValueFatalIfFail("diskCache.avgFlushIops",
                              &diskCacheOption->avgFlushIops);
    conf->GetValueFatalIfFail("diskCache.avgReadFileBytes",
                              &diskCacheOption->avgReadFileBytes);
    conf->GetValueFatalIfFail("diskCache.avgReadFileIops",
                              &diskCacheOption->avgReadFileIops);
}

void InitS3Option(Configuration *conf, S3Option *s3Opt) {
    conf->GetValueFatalIfFail("s3.fakeS3", &FLAGS_useFakeS3);
    conf->GetValueFatalIfFail("s3.fuseMaxSize",
                              &s3Opt->s3ClientAdaptorOpt.fuseMaxSize);
    conf->GetValueFatalIfFail("s3.pageSize",
                              &s3Opt->s3ClientAdaptorOpt.pageSize);
    conf->GetValueFatalIfFail("s3.prefetchBlocks",
                              &s3Opt->s3ClientAdaptorOpt.prefetchBlocks);
    conf->GetValueFatalIfFail("s3.prefetchExecQueueNum",
                              &s3Opt->s3ClientAdaptorOpt.prefetchExecQueueNum);
    conf->GetValueFatalIfFail("s3.threadScheduleInterval",
                              &s3Opt->s3ClientAdaptorOpt.intervalSec);
    conf->GetValueFatalIfFail("s3.cacheFlushIntervalSec",
                              &s3Opt->s3ClientAdaptorOpt.flushIntervalSec);
    conf->GetValueFatalIfFail("s3.chunkFlushThreads",
                              &s3Opt->s3ClientAdaptorOpt.chunkFlushThreads);
    conf->GetValueFatalIfFail("s3.writeCacheMaxByte",
                              &s3Opt->s3ClientAdaptorOpt.writeCacheMaxByte);
    conf->GetValueFatalIfFail("s3.readCacheMaxByte",
                              &s3Opt->s3ClientAdaptorOpt.readCacheMaxByte);
    conf->GetValueFatalIfFail("s3.nearfullRatio",
                              &s3Opt->s3ClientAdaptorOpt.nearfullRatio);
    conf->GetValueFatalIfFail("s3.baseSleepUs",
                              &s3Opt->s3ClientAdaptorOpt.baseSleepUs);
    conf->GetValueFatalIfFail(
        "s3.maxReadRetryIntervalMs",
        &s3Opt->s3ClientAdaptorOpt.maxReadRetryIntervalMs);
    conf->GetValueFatalIfFail("s3.readRetryIntervalMs",
                              &s3Opt->s3ClientAdaptorOpt.readRetryIntervalMs);
    ::curve::common::InitS3AdaptorOptionExceptS3InfoOption(conf,
                                                           &s3Opt->s3AdaptrOpt);
    InitDiskCacheOption(conf, &s3Opt->s3ClientAdaptorOpt.diskCacheOpt);
}

void InitVolumeOption(Configuration *conf, VolumeOption *volumeOpt) {
    conf->GetValueFatalIfFail("volume.bigFileSize", &volumeOpt->bigFileSize);
    conf->GetValueFatalIfFail("volume.volBlockSize", &volumeOpt->volBlockSize);
    conf->GetValueFatalIfFail("volume.fsBlockSize", &volumeOpt->fsBlockSize);
    conf->GetValueFatalIfFail("volume.allocator.type",
                              &volumeOpt->allocatorOption.type);

    conf->GetValueFatalIfFail(
        "volume.blockGroup.allocateOnce",
        &volumeOpt->allocatorOption.blockGroupOption.allocateOnce);

    if (volumeOpt->allocatorOption.type == "bitmap") {
        conf->GetValueFatalIfFail(
            "volume.bitmapAllocator.sizePerBit",
            &volumeOpt->allocatorOption.bitmapAllocatorOption.sizePerBit);
        conf->GetValueFatalIfFail(
            "volume.bitmapAllocator.smallAllocProportion",
            &volumeOpt->allocatorOption.bitmapAllocatorOption
                 .smallAllocProportion);
    } else {
        CHECK(false) << "only support bitmap allocator";
    }
}

void InitExtentManagerOption(Configuration *conf,
                             ExtentManagerOption *extentManagerOpt) {
    conf->GetValueFatalIfFail("extentManager.preAllocSize",
                              &extentManagerOpt->preAllocSize);
}

void InitLeaseOpt(Configuration *conf, LeaseOpt *leaseOpt) {
    conf->GetValueFatalIfFail("mds.leaseTimesUs", &leaseOpt->leaseTimeUs);
    conf->GetValueFatalIfFail("mds.refreshTimesPerLease",
                              &leaseOpt->refreshTimesPerLease);
}

void InitRefreshDataOpt(Configuration *conf,
                        RefreshDataOption *opt) {
    conf->GetValueFatalIfFail("fuseClient.maxDataSize",
                              &opt->maxDataSize);
    conf->GetValueFatalIfFail("fuseClient.refreshDataIntervalSec",
                              &opt->refreshDataIntervalSec);
}

void InitKVClientManagerOpt(Configuration *conf,
                               KVClientManagerOpt *config) {
    conf->GetValueFatalIfFail("fuseClient.supportKVcache",
                              &FLAGS_supportKVcache);
    conf->GetValueFatalIfFail("fuseClient.setThreadPool",
                              &config->setThreadPooln);
    conf->GetValueFatalIfFail("fuseClient.getThreadPool",
                              &config->getThreadPooln);
}

void InitFileSystemOption(Configuration* c, FileSystemOption* option) {
    c->GetValueFatalIfFail("fs.cto", &option->cto);
    c->GetValueFatalIfFail("fs.cto", &FLAGS_enableCto);
    c->GetValueFatalIfFail("fs.maxNameLength", &option->maxNameLength);
    { // kernel cache option
        auto o = option->kernelCacheOption;
        c->GetValueFatalIfFail("fs.kernelCache.attrTimeout", &o.attrTimeout);
        c->GetValueFatalIfFail("fs.kernelCache.dirAttrTimeout", &o.dirAttrTimeout);
        c->GetValueFatalIfFail("fs.kernelCache.entryTimeout", &o.entryTimeout);
        c->GetValueFatalIfFail("fs.kernelCache.dirEntryTimeout", &o.dirEntryTimeout);
    }
    { // dir cache option
        auto o = option->dirCacheOption;
        c->GetValueFatalIfFail("fs.dirCache.lruSize", &o.lruSize);
    }
    { // open file option
        auto o = option->openFileOption;
        c->GetValueFatalIfFail("fs.openFile.lruSize", &o.lruSize);
    }
    { // attr watcher option
        auto o = option->attrWatcherOption;
        c->GetValueFatalIfFail("fs.attrWatcher.lruSize", &o.lruSize);
    }
    { // attr watcher option
        auto o = option->rpcOption;
    }
}

void SetBrpcOpt(Configuration *conf) {
    curve::common::GflagsLoadValueFromConfIfCmdNotSet dummy;
    dummy.Load(conf, "defer_close_second", "rpc.defer.close.second",
               &brpc::FLAGS_defer_close_second);
    dummy.Load(conf, "health_check_interval", "rpc.healthCheckIntervalSec",
               &brpc::FLAGS_health_check_interval);
}

void InitFuseClientOption(Configuration *conf, FuseClientOption *clientOption) {
    InitMdsOption(conf, &clientOption->mdsOpt);
    InitMetaCacheOption(conf, &clientOption->metaCacheOpt);
    InitExcutorOption(conf, &clientOption->excutorOpt, false);
    InitExcutorOption(conf, &clientOption->excutorInternalOpt, true);
    InitBlockDeviceOption(conf, &clientOption->bdevOpt);
    InitS3Option(conf, &clientOption->s3Opt);
    InitExtentManagerOption(conf, &clientOption->extentManagerOpt);
    InitVolumeOption(conf, &clientOption->volumeOpt);
    InitLeaseOpt(conf, &clientOption->leaseOpt);
    InitRefreshDataOpt(conf, &clientOption->refreshDataOption);
    InitKVClientManagerOpt(conf, &clientOption->kvClientManagerOpt);

    conf->GetValueFatalIfFail("fuseClient.listDentryLimit",
                              &clientOption->listDentryLimit);
    conf->GetValueFatalIfFail("fuseClient.listDentryThreads",
                              &clientOption->listDentryThreads);
    conf->GetValueFatalIfFail("client.dummyServer.startPort",
                              &clientOption->dummyServerStartPort);
    conf->GetValueFatalIfFail("fuseClient.enableMultiMountPointRename",
                              &clientOption->enableMultiMountPointRename);
    conf->GetValueFatalIfFail("fuseClient.disableXattr",
                              &clientOption->disableXattr);
    conf->GetValueFatalIfFail("fuseClient.downloadMaxRetryTimes",
                              &clientOption->downloadMaxRetryTimes);
    conf->GetValueFatalIfFail("fuseClient.warmupThreadsNum",
                              &clientOption->warmupThreadsNum);
    LOG_IF(WARNING, conf->GetBoolValue("fuseClient.enableSplice",
                                       &clientOption->enableFuseSplice))
        << "Not found `fuseClient.enableSplice` in conf, use default value `"
        << std::boolalpha << clientOption->enableFuseSplice << '`';

    InitFileSystemOption(conf, &clientOption->fileSystemOption);
    SetBrpcOpt(conf);
}

void SetFuseClientS3Option(FuseClientOption *clientOption,
    const S3InfoOption &fsS3Opt) {
    clientOption->s3Opt.s3ClientAdaptorOpt.blockSize = fsS3Opt.blockSize;
    clientOption->s3Opt.s3ClientAdaptorOpt.chunkSize = fsS3Opt.chunkSize;
    clientOption->s3Opt.s3AdaptrOpt.s3Address = fsS3Opt.s3Address;
    clientOption->s3Opt.s3AdaptrOpt.ak = fsS3Opt.ak;
    clientOption->s3Opt.s3AdaptrOpt.sk = fsS3Opt.sk;
    clientOption->s3Opt.s3AdaptrOpt.bucketName = fsS3Opt.bucketName;
}

void S3Info2FsS3Option(const curvefs::common::S3Info& s3,
                       S3InfoOption* fsS3Opt) {
    fsS3Opt->ak = s3.ak();
    fsS3Opt->sk = s3.sk();
    fsS3Opt->s3Address = s3.endpoint();
    fsS3Opt->bucketName = s3.bucketname();
    fsS3Opt->blockSize = s3.blocksize();
    fsS3Opt->chunkSize = s3.chunksize();
}

}  // namespace common
}  // namespace client
}  // namespace curvefs
