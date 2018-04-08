#include "stdafx.h"
#include "KinectController.h"
#include <iostream>

using namespace std;

KinectController::KinectController()
{
	Sensor = nullptr;

	DepthFrame = nullptr;
	DepthReader = nullptr;
	DepthFrame = nullptr;

	BodyIndexSource = nullptr;
	BodyIndexReader = nullptr;
	BodyIndexFrame = nullptr;

	Mapper = nullptr;
}


void KinectController::OpenSensor()
{
	GetDefaultKinectSensor(&Sensor);
	Sensor->Open();
}

void KinectController::OpenDepthReader()
{
	Sensor->get_DepthFrameSource(&DepthSource);
	DepthSource->OpenReader(&DepthReader);
}

void KinectController::OpenBodyIndexReader()
{
	Sensor->get_BodyIndexFrameSource(&BodyIndexSource);
	BodyIndexSource->OpenReader(&BodyIndexReader);
}

void KinectController::OpenBodyReader()
{
	Sensor->get_BodyFrameSource(&BodySource);
	BodySource->OpenReader(&BodyReader);
}

void KinectController::GetMapper()
{
	Sensor->get_CoordinateMapper(&Mapper);
}

void KinectController::GetDepthData(UINT *DepthBufferSize,UINT16 **DepthBuffer)
{
	while(DepthReader->AcquireLatestFrame(&DepthFrame) != S_OK);
	DepthFrame->AccessUnderlyingBuffer(DepthBufferSize, DepthBuffer);
}

void KinectController::GetBodyIndexData(UINT *BodyIndexBufferSize,BYTE **BodyIndexBuffer)
{
	while(BodyIndexReader->AcquireLatestFrame(&BodyIndexFrame) != S_OK);
	BodyIndexFrame->AccessUnderlyingBuffer(BodyIndexBufferSize, BodyIndexBuffer);
}

//此函数仅适用于探测单个人物的情景，仅返回一组关节坐标
bool KinectController::GetBodyData(Joint *JointArray)
{
	int BodyBufferSize = 0;
	while(BodyReader->AcquireLatestFrame(&BodyFrame) != S_OK);
	BodySource->get_BodyCount(&BodyBufferSize);
	IBody **BodyArray = new IBody *[BodyBufferSize];
	for (int i = 0;i < BodyBufferSize; i++)
		BodyArray[i] = nullptr;
	BodyFrame->GetAndRefreshBodyData(BodyBufferSize, BodyArray);
	for (int i = 0; i < BodyBufferSize; i++)
	{
		BOOLEAN TrackedResult = false;
		if (BodyArray[i]->get_IsTracked(&TrackedResult) == S_OK && TrackedResult)
		{
			//cout<<"Human tracked! Return joint data..."<<endl;
			BodyArray[i]->GetJoints(JointType_Count,JointArray);
			delete[] BodyArray;
			return TRACKED;
		}
	}
	delete[] BodyArray;
	return NOTRACKED;
}

void KinectController::MapCameratoDepth(CameraSpacePoint cameraPoint, DepthSpacePoint *depthpoint)
{
	Mapper->MapCameraPointToDepthSpace(cameraPoint ,depthpoint);
}

void KinectController::MapDepthtoCamera(DepthSpacePoint depthpoint, UINT16 depth, CameraSpacePoint *camerapoint)
{
	Mapper->MapDepthPointToCameraSpace(depthpoint, depth, camerapoint);
}

void KinectController::MapCameratoColor(CameraSpacePoint cameraPoint, ColorSpacePoint *colorpoint)
{
	Mapper->MapCameraPointToColorSpace(cameraPoint, colorpoint);
}

void KinectController::ReleaseDepthData()
{
	DepthFrame->Release();
}

void KinectController::ReleaseBodyIndexData()
{
	BodyIndexFrame->Release();
}

void KinectController::ReleaseBodyData()
{
	BodyFrame->Release();
}