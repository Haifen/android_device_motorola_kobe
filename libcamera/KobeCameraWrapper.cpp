/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "KobeCameraWrapper"

#include <dlfcn.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <camera/Camera.h>
#include "KobeCameraWrapper.h"

namespace android {

typedef sp<CameraHardwareInterface> (*OpenCamFunc)(int);

static void * g_motoLibHandle = NULL;
static OpenCamFunc g_motoOpenCameraHardware = NULL;

static void ensureMotoLibOpened()
{
    if (g_motoLibHandle == NULL) {
        g_motoLibHandle = ::dlopen("libkobecamera.so", RTLD_NOW);
        if (g_motoLibHandle == NULL) {
            assert(0);
            LOGE("dlopen() error: %s\n", dlerror());
        } else {
            g_motoOpenCameraHardware = (OpenCamFunc) ::dlsym(g_motoLibHandle, "openCameraHardware");
            assert(g_motoOpenCameraHardware != NULL);
        }
    }
}

extern "C" int HAL_getNumberOfCameras()
{
    return 1;
}

extern "C" void HAL_getCameraInfo(int cameraId, struct CameraInfo* cameraInfo)
{
    cameraInfo->facing = CAMERA_FACING_BACK;
    cameraInfo->orientation = 90;
}

extern "C" sp<CameraHardwareInterface> HAL_openCameraHardware(int cameraId)
{
    LOGV("openCameraHardware: call createInstance");
    ensureMotoLibOpened();
    return KobeCameraWrapper::createInstance(cameraId);
}

wp<CameraHardwareInterface> KobeCameraWrapper::singleton;

sp<CameraHardwareInterface> KobeCameraWrapper::createInstance(int cameraId)
{
    LOGV("%s :", __func__);
    if (singleton != NULL) {
        sp<CameraHardwareInterface> hardware = singleton.promote();
        if (hardware != NULL) {
            return hardware;
        }
    }

    ensureMotoLibOpened();

    sp<CameraHardwareInterface> hardware(new KobeCameraWrapper(cameraId));
    singleton = hardware;
    return hardware;
}

static bool
deviceCardMatches(const char *device, const char *matchCard)
{
    struct v4l2_capability caps;
    int fd = ::open(device, O_RDWR);
    bool ret;

    if (fd < 0) {
        return false;
    }

    if (::ioctl(fd, VIDIOC_QUERYCAP, &caps) < 0) {
        ret = false;
    } else {
        const char *card = (const char *) caps.card;

        LOGD("device %s card is %s\n", device, card);
        ret = strstr(card, matchCard) != NULL;
    }

    ::close(fd);

    return ret;
}

KobeCameraWrapper::KobeCameraWrapper(int cameraId) :
    mMotoInterface(g_motoOpenCameraHardware(cameraId)),
    mCameraType(CAM_UNKNOWN)
{
    struct v4l2_capability caps;

    if (deviceCardMatches("/dev/video3", "camise")) {
        LOGI("Detected SOC device\n");
        mCameraType = CAM_SOC;
    } else if (deviceCardMatches("/dev/video0", "mt9p012")) {
        LOGI("Detected BAYER device\n");
        mCameraType = CAM_BAYER;
    }
}

KobeCameraWrapper::~KobeCameraWrapper()
{
}

sp<IMemoryHeap>
KobeCameraWrapper::getPreviewHeap() const
{
    return mMotoInterface->getPreviewHeap();
}

sp<IMemoryHeap>
KobeCameraWrapper::getRawHeap() const
{
    return mMotoInterface->getRawHeap();
}

void
KobeCameraWrapper::setCallbacks(notify_callback notify_cb,
                                  data_callback data_cb,
                                  data_callback_timestamp data_cb_timestamp,
                                  void* user)
{
    mMotoInterface->setCallbacks(notify_cb, data_cb, data_cb_timestamp, user);
}

void
KobeCameraWrapper::enableMsgType(int32_t msgType)
{
    mMotoInterface->enableMsgType(msgType);
}

void
KobeCameraWrapper::disableMsgType(int32_t msgType)
{
    mMotoInterface->disableMsgType(msgType);
}

bool
KobeCameraWrapper::msgTypeEnabled(int32_t msgType)
{
    return mMotoInterface->msgTypeEnabled(msgType);
}

status_t
KobeCameraWrapper::startPreview()
{
    return mMotoInterface->startPreview();
}

bool
KobeCameraWrapper::useOverlay()
{
    return mMotoInterface->useOverlay();
}

status_t
KobeCameraWrapper::setOverlay(const sp<Overlay> &overlay)
{
    return mMotoInterface->setOverlay(overlay);
}

void
KobeCameraWrapper::stopPreview()
{
    mMotoInterface->stopPreview();
}

bool
KobeCameraWrapper::previewEnabled()
{
    return mMotoInterface->previewEnabled();
}

status_t
KobeCameraWrapper::startRecording()
{
    return mMotoInterface->startRecording();
}

void
KobeCameraWrapper::stopRecording()
{
    mMotoInterface->stopRecording();
}

bool
KobeCameraWrapper::recordingEnabled()
{
    return mMotoInterface->recordingEnabled();
}

void
KobeCameraWrapper::releaseRecordingFrame(const sp<IMemory>& mem)
{
    return mMotoInterface->releaseRecordingFrame(mem);
}

status_t
KobeCameraWrapper::autoFocus()
{
    return mMotoInterface->autoFocus();
}

status_t
KobeCameraWrapper::cancelAutoFocus()
{
    return mMotoInterface->cancelAutoFocus();
}

status_t
KobeCameraWrapper::takePicture()
{
    return mMotoInterface->takePicture();
}

status_t
KobeCameraWrapper::cancelPicture()
{
    return mMotoInterface->cancelPicture();
}

status_t
KobeCameraWrapper::setParameters(const CameraParameters& params)
{
    return mMotoInterface->setParameters(params);
}

CameraParameters
KobeCameraWrapper::getParameters() const
{
    CameraParameters ret = mMotoInterface->getParameters();

    return ret;
}

status_t
KobeCameraWrapper::sendCommand(int32_t cmd, int32_t arg1, int32_t arg2)
{
    return mMotoInterface->sendCommand(cmd, arg1, arg2);
}

void
KobeCameraWrapper::release()
{
    mMotoInterface->release();
}

status_t
KobeCameraWrapper::dump(int fd, const Vector<String16>& args) const
{
    return mMotoInterface->dump(fd, args);
}

}; //namespace android
