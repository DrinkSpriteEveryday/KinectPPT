#ifndef _KinectController
#define _KinectController

#include <Kinect.h>
#pragma comment (lib, "User32.lib")

#define TRACKED (TRUE)
#define NOTRACKED (FALSE)

class KinectController
{
private:
	IKinectSensor *Sensor;

	IDepthFrameSource *DepthSource;
	IDepthFrameReader *DepthReader;
	IDepthFrame	*DepthFrame;

	IBodyIndexFrameSource *BodyIndexSource;
	IBodyIndexFrameReader *BodyIndexReader;
	IBodyIndexFrame	*BodyIndexFrame;

	IBodyFrameSource *BodySource;
	IBodyFrameReader *BodyReader;
	IBodyFrame *BodyFrame;

	ICoordinateMapper *Mapper;
public:
	static const int DepthHeight = 424;     //定义深度空间高度
	static const int DepthWidth  = 512;		//定义深度空间宽度
	KinectController();					//构造函数
	void OpenSensor();                  //开启Kinect传感器
	void OpenDepthReader();             //打开深度空间Reader
	void OpenBodyIndexReader();         
	void OpenBodyReader();
	void GetMapper();
	void GetDepthData(UINT *DepthBufferSize,UINT16 **DepthBuffer);             
	void GetBodyIndexData(UINT *BodyIndexBufferSize,BYTE **BodyIndexBuffer);			
	bool GetBodyData(Joint *JointArray);

	void MapCameratoDepth(CameraSpacePoint cameraPoint, DepthSpacePoint *depthpoint);
	void MapDepthtoCamera(DepthSpacePoint depthpoint, UINT16 depth, CameraSpacePoint *camerapoint);
	void MapCameratoColor(CameraSpacePoint cameraPoint, ColorSpacePoint *colorpoint);     

	void ReleaseDepthData();
	void ReleaseBodyIndexData();
	void ReleaseBodyData();
};

#endif