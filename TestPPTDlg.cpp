
// TestPPTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestPPT.h"
#include "TestPPTDlg.h"
#include "KinectController.h"
#include <iostream>
#include <fstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestPPTDlg 对话框




CTestPPTDlg::CTestPPTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestPPTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestPPTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestPPTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CTestPPTDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CTestPPTDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CTestPPTDlg::OnBnClickedButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_LAST, &CTestPPTDlg::OnBnClickedButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, &CTestPPTDlg::OnBnClickedButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CTestPPTDlg::OnBnClickedButtonNext)
	ON_WM_DESTROY()
	END_MESSAGE_MAP()


// CTestPPTDlg 消息处理程序

BOOL CTestPPTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(L"Failed to initialize OLE");
		return FALSE;
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestPPTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestPPTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestPPTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define PI 3.141592
ofstream GestureSamplesTextFile;                               //写入样本的txt文件
KinectController myKinectController = KinectController();      //申明Kinect控制器变量

void RecordSampleData(Joint *JointArray);                      //记录样本数据，供机器学习方法使用
float CalculateProduct(ColorSpacePoint A, ColorSpacePoint B);  //计算两个向量内积
float CalculateModulo(ColorSpacePoint A);                      //计算向量的模
void KinectCalculateResult(Joint *JointArray);                 //根据当前身体帧计算进一步结果


ColorSpacePoint ColorHandRight;                            //获取右手坐标 of 色彩坐标系 记作点A
ColorSpacePoint ColorSpineShoudler;						   //获取中部坐标 of 色彩坐标系 记作点B
ColorSpacePoint ColorSpineBase;                            //获取腰部坐标 of 色彩坐标系 记作点C
ColorSpacePoint ColorHandLeft;							   //获取右手坐标 of 色彩坐标系 记作点D
ColorSpacePoint ColorHead;                                 //获取脑袋坐标 of 色彩坐标系 
ColorSpacePoint VecBC;                                     //向量BA
ColorSpacePoint VecBA;                                     //向量BC
ColorSpacePoint VecBD;                                     //向量BD
float           AngleABC;                                  //∠ABC 的大小 （°）
float           AngleDBC;                                  //∠DBC 的大小 （°）
unsigned char   FlagState;                                 //表明状态

void CTestPPTDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	// Start PowerPoint and get Application object...
	if (!app.CreateDispatch(L"Powerpoint.Application"))
	{
		AfxMessageBox(L"Couldn't start PowerPoint.");
	}
	else
	{
		app.put_Visible(TRUE);
		//有时候为了ppt不影响程序,会设置启动大小
		//app.put_Width(200);
		//app.put_Height(200);
	}
	CString strFilter = L"PowerPoint Files (*.ppt;*.pptx)|*.ppt;*.pptx|All Files(*.*)|*.*||";
	CFileDialog FileDlg(TRUE,L"PPT",NULL,OFN_FILEMUSTEXIST|OFN_NONETWORKBUTTON
		|OFN_PATHMUSTEXIST,strFilter);
	FileDlg.DoModal();

	// To get the selected file's path and name
	CString strFileName;
	strFileName = FileDlg.GetPathName();

	if(!strFileName.IsEmpty())
	{
		presentations = app.get_Presentations();
		presentation = presentations.Open(strFileName,0,0,1);
	}

	presentations = app.get_ActivePresentation();
	slides = presentation.get_Slides(); 
	// Show the first slide of the presentation 
	slide = slides.Item(COleVariant((long)1));

	//Run the show 
	slideShowSettings = presentation.get_SlideShowSettings(); 
	slideShowSettings.Run();

	//The Following Code is for Kinect_Controling
	//下面开始Kinect的初始化
	UINT DepthBufferSize;									   //深度帧缓冲区大小
	UINT16 *DepthBuffer;									   //深度帧缓冲区地址
	UINT BodyIndexBufferSize;                                  //身体帧缓冲区大小
	BYTE *BodyIndexBuffer;									   //身体帧缓冲区地址
	Joint JointArray[JointType_Count];                         //用于获取关节数据



	myKinectController.OpenSensor();                           //打开Kinect传感器
	myKinectController.OpenDepthReader();
	myKinectController.OpenBodyIndexReader();				   
	myKinectController.OpenBodyReader();                       //打开所有需要的Reader
	myKinectController.GetMapper();							   //打开坐标映射功能
	
	while(1)
	{
		myKinectController.GetDepthData(&DepthBufferSize,&DepthBuffer);               //获得深度数据
		myKinectController.GetBodyIndexData(&BodyIndexBufferSize,&BodyIndexBuffer);   //获得人物索引数据
		if(myKinectController.GetBodyData(JointArray))                                //读取到了关节数据
		{
			KinectCalculateResult(JointArray);
			if(AngleABC >= 80 && AngleABC <= 100 && FlagState != 1 && ColorHead.Y < ColorHandRight.Y)     //角度符合要求且当前不处于伸出右臂状态
			{
				FlagState = 1;                      //当前状态设置为伸出右臂
				presentation = app.get_ActivePresentation();
				slideShowWindow = presentation.get_SlideShowWindow();
				slideShowView = slideShowWindow.get_View();
				slideShowView.Next();				//NEXT翻页
				
			}
			else if(AngleDBC >= 80 && AngleDBC <= 100 && FlagState != 2 && ColorHead.Y < ColorHandLeft.Y)
			{
				FlagState = 2;
				presentation = app.get_ActivePresentation();
				slideShowWindow = presentation.get_SlideShowWindow();
				slideShowView = slideShowWindow.get_View();
				slideShowView.Previous();
				
			}
			else if(ColorHead.Y > ColorHandRight.Y && FlagState != 3)
			{
				FlagState = 3;
				presentation = app.get_ActivePresentation();
				slideShowWindow = presentation.get_SlideShowWindow();
				slideShowView = slideShowWindow.get_View();
				slideShowView.Last();
				_sleep(500);
			}
			else if(ColorHead.Y > ColorHandLeft.Y && FlagState != 4)
			{
				FlagState = 4;
				presentation = app.get_ActivePresentation();
				slideShowWindow = presentation.get_SlideShowWindow();
				slideShowView = slideShowWindow.get_View();
				slideShowView.First();
				_sleep(500);
			}
			else if((AngleABC < 80 || AngleABC >100) && (AngleDBC < 80 || AngleDBC >100)
				&& ColorHead.Y <= ColorHandRight.Y && ColorHead.Y <= ColorHandLeft.Y)
			{
				FlagState = 0;
			}
		}
		myKinectController.ReleaseDepthData();      //释放深度数据
		myKinectController.ReleaseBodyIndexData();  //释放人体索引数据
		myKinectController.ReleaseBodyData();		//释放关节数据
	}
}

void CTestPPTDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	app.Quit();
}

void CTestPPTDlg::OnBnClickedButtonFirst()
{
	// TODO: 在此添加控件通知处理程序代码
	presentation = app.get_ActivePresentation();
	slideShowWindow = presentation.get_SlideShowWindow();
	slideShowView = slideShowWindow.get_View();
	slideShowView.First();
}

void CTestPPTDlg::OnBnClickedButtonLast()
{
	// TODO: 在此添加控件通知处理程序代码
	presentation = app.get_ActivePresentation();
	slideShowWindow = presentation.get_SlideShowWindow();
	slideShowView = slideShowWindow.get_View();
	slideShowView.Last();
}

void CTestPPTDlg::OnBnClickedButtonPrevious()
{
	// TODO: 在此添加控件通知处理程序代码
	presentation = app.get_ActivePresentation();
	slideShowWindow = presentation.get_SlideShowWindow();
	slideShowView = slideShowWindow.get_View();
	slideShowView.Previous();
}

void CTestPPTDlg::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	presentation = app.get_ActivePresentation();
	slideShowWindow = presentation.get_SlideShowWindow();
	slideShowView = slideShowWindow.get_View();
	slideShowView.Next();
}

void CTestPPTDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	OnBnClickedButtonClose();
}



//以下是Kinect处理时使用的函数

float CalculateProduct(ColorSpacePoint A, ColorSpacePoint B)
{
	return A.X * B.X + A.Y * B.Y;
}

float CalculateModulo(ColorSpacePoint A)
{
	return sqrt( A.X * A.X + A.Y * A.Y);
}

void KinectCalculateResult(Joint *JointArray)
{
		myKinectController.MapCameratoColor(JointArray[JointType_HandRight].Position, &ColorHandRight);
		myKinectController.MapCameratoColor(JointArray[JointType_SpineShoulder].Position, &ColorSpineShoudler);
		myKinectController.MapCameratoColor(JointArray[JointType_SpineBase].Position, &ColorSpineBase);
		myKinectController.MapCameratoColor(JointArray[JointType_HandLeft].Position, &ColorHandLeft);
		myKinectController.MapCameratoColor(JointArray[JointType_Head].Position, &ColorHead);
		VecBC.X = ColorSpineBase.X - ColorSpineShoudler.X;
		VecBC.Y = ColorSpineBase.Y - ColorSpineShoudler.Y;
		VecBA.X = ColorHandRight.X - ColorSpineShoudler.X;
		VecBA.Y = ColorHandRight.Y - ColorSpineShoudler.Y;
		VecBD.X = ColorHandLeft.X - ColorSpineShoudler.X;
		VecBD.Y = ColorHandLeft.Y - ColorSpineShoudler.Y;
		AngleABC = acos(CalculateProduct(VecBA, VecBC)/(CalculateModulo(VecBA) * CalculateModulo(VecBC)));
		AngleABC = AngleABC * 180 / PI;
		AngleDBC = acos(CalculateProduct(VecBD, VecBC)/(CalculateModulo(VecBD) * CalculateModulo(VecBC)));
		AngleDBC = AngleDBC * 180 / PI;
}