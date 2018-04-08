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
	static const int DepthHeight = 424;     //������ȿռ�߶�
	static const int DepthWidth  = 512;		//������ȿռ���
	KinectController();					//���캯��
	void OpenSensor();                  //����Kinect������
	void OpenDepthReader();             //����ȿռ�Reader
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