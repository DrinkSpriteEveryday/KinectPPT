
// TestPPTDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTestPPTDlg �Ի���




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


// CTestPPTDlg ��Ϣ�������

BOOL CTestPPTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(L"Failed to initialize OLE");
		return FALSE;
	}


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestPPTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestPPTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define PI 3.141592
ofstream GestureSamplesTextFile;                               //д��������txt�ļ�
KinectController myKinectController = KinectController();      //����Kinect����������

void RecordSampleData(Joint *JointArray);                      //��¼�������ݣ�������ѧϰ����ʹ��
float CalculateProduct(ColorSpacePoint A, ColorSpacePoint B);  //�������������ڻ�
float CalculateModulo(ColorSpacePoint A);                      //����������ģ
void KinectCalculateResult(Joint *JointArray);                 //���ݵ�ǰ����֡�����һ�����


ColorSpacePoint ColorHandRight;                            //��ȡ�������� of ɫ������ϵ ������A
ColorSpacePoint ColorSpineShoudler;						   //��ȡ�в����� of ɫ������ϵ ������B
ColorSpacePoint ColorSpineBase;                            //��ȡ�������� of ɫ������ϵ ������C
ColorSpacePoint ColorHandLeft;							   //��ȡ�������� of ɫ������ϵ ������D
ColorSpacePoint ColorHead;                                 //��ȡ�Դ����� of ɫ������ϵ 
ColorSpacePoint VecBC;                                     //����BA
ColorSpacePoint VecBA;                                     //����BC
ColorSpacePoint VecBD;                                     //����BD
float           AngleABC;                                  //��ABC �Ĵ�С ���㣩
float           AngleDBC;                                  //��DBC �Ĵ�С ���㣩
unsigned char   FlagState;                                 //����״̬

void CTestPPTDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// Start PowerPoint and get Application object...
	if (!app.CreateDispatch(L"Powerpoint.Application"))
	{
		AfxMessageBox(L"Couldn't start PowerPoint.");
	}
	else
	{
		app.put_Visible(TRUE);
		//��ʱ��Ϊ��ppt��Ӱ�����,������������С
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
	//���濪ʼKinect�ĳ�ʼ��
	UINT DepthBufferSize;									   //���֡��������С
	UINT16 *DepthBuffer;									   //���֡��������ַ
	UINT BodyIndexBufferSize;                                  //����֡��������С
	BYTE *BodyIndexBuffer;									   //����֡��������ַ
	Joint JointArray[JointType_Count];                         //���ڻ�ȡ�ؽ�����



	myKinectController.OpenSensor();                           //��Kinect������
	myKinectController.OpenDepthReader();
	myKinectController.OpenBodyIndexReader();				   
	myKinectController.OpenBodyReader();                       //��������Ҫ��Reader
	myKinectController.GetMapper();							   //������ӳ�书��
	
	while(1)
	{
		myKinectController.GetDepthData(&DepthBufferSize,&DepthBuffer);               //����������
		myKinectController.GetBodyIndexData(&BodyIndexBufferSize,&BodyIndexBuffer);   //���������������
		if(myKinectController.GetBodyData(JointArray))                                //��ȡ���˹ؽ�����
		{
			KinectCalculateResult(JointArray);
			if(AngleABC >= 80 && AngleABC <= 100 && FlagState != 1 && ColorHead.Y < ColorHandRight.Y)     //�Ƕȷ���Ҫ���ҵ�ǰ����������ұ�״̬
			{
				FlagState = 1;                      //��ǰ״̬����Ϊ����ұ�
				presentation = app.get_ActivePresentation();
				slideShowWindow = presentation.get_SlideShowWindow();
				slideShowView = slideShowWindow.get_View();
				slideShowView.Next();				//NEXT��ҳ
				
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
		myKinectController.ReleaseDepthData();      //�ͷ��������
		myKinectController.ReleaseBodyIndexData();  //�ͷ�������������
		myKinectController.ReleaseBodyData();		//�ͷŹؽ�����
	}
}

void CTestPPTDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	app.Quit();
}

void CTestPPTDlg::OnBnClickedButtonFirst()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	presentation = app.get_ActivePresentation();
	slideShowWindow = presentation.get_SlideShowWindow();
	slideShowView = slideShowWindow.get_View();
	slideShowView.First();
}

void CTestPPTDlg::OnBnClickedButtonLast()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	presentation = app.get_ActivePresentation();
	slideShowWindow = presentation.get_SlideShowWindow();
	slideShowView = slideShowWindow.get_View();
	slideShowView.Last();
}

void CTestPPTDlg::OnBnClickedButtonPrevious()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	presentation = app.get_ActivePresentation();
	slideShowWindow = presentation.get_SlideShowWindow();
	slideShowView = slideShowWindow.get_View();
	slideShowView.Previous();
}

void CTestPPTDlg::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	presentation = app.get_ActivePresentation();
	slideShowWindow = presentation.get_SlideShowWindow();
	slideShowView = slideShowWindow.get_View();
	slideShowView.Next();
}

void CTestPPTDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	OnBnClickedButtonClose();
}



//������Kinect����ʱʹ�õĺ���

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