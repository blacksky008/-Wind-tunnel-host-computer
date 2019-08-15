// SerialTestDlg.cpp: 实现文件
#include "pch.h"
#include "framework.h"
#include "SerialTest.h"
#include "SerialTestDlg.h"
#include "afxdialogex.h"
#include "mscomm1.h"
#include <iostream>
#include <regex>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_LINE 1024

#pragma warning(disable:4996)

using namespace std;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:

	// 实现
protected:

public:

};


// CSerialTestDlg 对话框



CSerialTestDlg::CSerialTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALTEST_DIALOG, pParent)
	, m_strTXData(_T(""))
	, m_strRXData(_T(""))
	, m_edit_sure(_T("S"))
	, m_edit_calibration(_T("Q"))
	, m_edit_k_x(_T(""))
	, m_edit_read(_T("R"))
	, m_edit_k_y(_T(""))
	, m_edit_hj_x(_T(""))
	, m_edit_hj_y(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_comboCom);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);
	DDX_Text(pDX, IDC_EDIT_TXDATA, m_strTXData);
	DDX_Text(pDX, IDC_EDIT_RXDATA, m_strRXData);
	DDX_Text(pDX, IDC_EDIT_SURE, m_edit_sure);
	DDX_Text(pDX, IDC_EDIT_CALIBRATION, m_edit_calibration);
	DDX_Text(pDX, IDC_EDIT_READ, m_edit_read);
	DDX_Text(pDX, IDC_EDIT_K_X, m_edit_k_x);
	DDX_Text(pDX, IDC_EDIT_K_Y, m_edit_k_y);
	DDX_Text(pDX, IDC_EDIT_HJ_X, m_edit_hj_x);
	DDX_Text(pDX, IDC_EDIT_HJ_Y, m_edit_hj_y);
}

BEGIN_MESSAGE_MAP(CSerialTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_COM, &CSerialTestDlg::OnCbnSelchangeComboCom)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSerialTestDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEANUP, &CSerialTestDlg::OnBnClickedButtonCleanup)
	ON_EN_CHANGE(IDC_EDIT_RXDATA, &CSerialTestDlg::OnEnChangeEditRxdata)
	ON_BN_CLICKED(IDC_BUTTON_INPUTWRITE, &CSerialTestDlg::OnBnClickedButtonInputwrite)
	ON_BN_CLICKED(IDC_BUTTON_SURE, &CSerialTestDlg::OnBnClickedButtonSure)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, &CSerialTestDlg::OnBnClickedButtonCalibration)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_HJ, &CSerialTestDlg::OnBnClickedButtonSaveHj)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CSerialTestDlg::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CSerialTestDlg::OnBnClickedButtonChange)
END_MESSAGE_MAP()


// CSerialTestDlg 消息处理程序

BOOL CSerialTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_comboCom.AddString(_T("COM1")); // 为组合框控件的列表框添加列表项
	m_comboCom.AddString(_T("COM2"));
	m_comboCom.AddString(_T("COM3"));
	m_comboCom.AddString(_T("COM4"));
	m_comboCom.AddString(_T("COM5"));
	m_comboCom.AddString(_T("COM6"));
	m_comboCom.AddString(_T("COM7"));
	m_comboCom.AddString(_T("COM8"));
	m_comboCom.AddString(_T("COM9"));
	m_comboCom.AddString(_T("COM10"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSerialTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSerialTestDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSerialTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerialTestDlg::OnCbnSelchangeComboCom()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel;
	nSel = m_comboCom.GetCurSel();//获取组合框控件的列表框中选中项的索引

	m_ctrlComm.put_CommPort(nSel + 1);//选择串口号(这里因为列表框的索引号是从0开始，所以(nSel+1)对应的才是我们所选的串口号)
	m_ctrlComm.put_PortOpen(TRUE);//打开串口
	m_ctrlComm.put_RThreshold(2);//收到两个字节引发OnComm事件 
	m_ctrlComm.put_InputMode(1);//输入模式选为二进制 
	m_ctrlComm.put_Settings(_T("9600,n,8,1"));//设置串口参数，波特率，无奇偶校验，位停止位，位数据位
	m_ctrlComm.put_InputMode(1);  // 以二进制方式检取数据 
	m_ctrlComm.put_RThreshold(1); //参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件 
	m_ctrlComm.put_InputLen(0); //设置当前接收区数据长度为0 
	m_ctrlComm.get_Input();//先预读缓冲区以清除残留数据  

	m_setOk = true;     //标记串口设置OK
}
BEGIN_EVENTSINK_MAP(CSerialTestDlg, CDialogEx)
	ON_EVENT(CSerialTestDlg, IDC_MSCOMM1, 1, CSerialTestDlg::OnComm, VTS_NONE)
END_EVENTSINK_MAP()

void CSerialTestDlg::OnComm()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;   //Variant 是一种特殊的数据类型，除了定长String数据及用户定义类型外，可以包含任何种类的数据。
	COleSafeArray safearray_inp;
	LONG len, k;
	BYTE rxdata[2048]; //设置BYTE数组 An 8-bit integer that is not signed.     
	CString strtemp;

	if (m_ctrlComm.get_CommEvent() == 2) //事件值为2表示接收缓冲区内有字符     
	{
		////////以下你可以根据自己的通信协议加入处理代码 
		variant_inp = m_ctrlComm.get_Input();     //读缓冲区  
		safearray_inp = variant_inp;              //VARIANT型变量转换为ColeSafeArray型变量
		len = safearray_inp.GetOneDimSize();      //得到有效数据长度        
		for (k = 0; k < len; k++)
			safearray_inp.GetElement(&k, rxdata + k);//转换为BYTE型数组         
		for (k = 0; k < len; k++)                    //将数组转换为Cstring型变量    
		{
			BYTE bt = *(char*)(rxdata + k);//字符型      
			strtemp.Format(_T("%c"), bt); //将字符送入临时变量strtemp存放   
			m_strRXData += strtemp; //加入接收编辑框对应字符串 
		}

	}
	GetDlgItem(IDC_EDIT_RXDATA)->SetWindowText(m_strRXData);

	//UpdateData(FALSE); //更新编辑框内容 
	this->SendDlgItemMessage(IDC_EDIT_RXDATA, WM_VSCROLL, SB_BOTTOM, 0); //滚动条始终在底部

}

void CSerialTestDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_setOk == true)    //判断是否打开并初始化串口
	{
		UpdateData(TRUE);  //读取编辑框内容
		m_ctrlComm.put_Output(COleVariant(m_strTXData)); //发送数据
		m_strTXData.Empty();
		UpdateData(false);
	}
	else
	{
		MessageBox(_T("请先选择COM口"));
	}

}

void CSerialTestDlg::OnBnClickedButtonCleanup()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strRXData = "";
	UpdateData(FALSE);//更新编辑框内容 

}


void CSerialTestDlg::OnEnChangeEditRxdata()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CSerialTestDlg::OnBnClickedButtonInputwrite()
{


	// TODO: 在此添加控件通知处理程序代码
	remove("1.txt");
	//static WORD unicode = 0xFEFF;
	CString str;
	GetDlgItem(IDC_EDIT_RXDATA)->GetWindowText(str);
	string strStr;
	strStr = (LPCSTR)(CStringA)(str);

	//CFile  file;

	//bool flag = file.Open(_T("1.txt",), CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	//if (flag == false)
	//{
	//	MessageBox(_T("写文件失败！"), NULL, MB_ICONERROR); //显示错误框
	//}
	//file.SeekToEnd();

	ofstream outfile;
	outfile.open("1.txt");
	outfile << strStr << endl;
	//file.WriteString(strStr);
	MessageBox(L"已写入");
	outfile.close();


	//file.Write(&unicode, 2);
	/*file.Write(str, wcslen(str) * sizeof(wchar_t));
	file.Close();*/



}

void CSerialTestDlg::OnBnClickedButtonSure()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_setOk == true)    //判断是否打开并初始化串口
	{
		UpdateData(TRUE);  //读取编辑框内容
		m_ctrlComm.put_Output(COleVariant(m_edit_sure)); //发送数据
		m_strTXData.Empty();
		UpdateData(false);


	}
	else
	{
		MessageBox(_T("请先选择COM口"));
	}
}

void CSerialTestDlg::OnBnClickedButtonCalibration()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_setOk == true)    //判断是否打开并初始化串口
	{
		UpdateData(TRUE);  //读取编辑框内容
		m_ctrlComm.put_Output(COleVariant(m_edit_calibration)); //发送数据
		UpdateData(false);
	}

}

void CSerialTestDlg::OnBnClickedButtonWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	char buf[MAX_LINE];
	FILE* fp;
	if ((fp = fopen("1.txt", "r")) == NULL)
	{
		perror("fail to read");
		exit(1);
	}
	while (fgets(buf, MAX_LINE, fp) != NULL)
	{

		string text = buf;

		regex  pattern("K_X=(.*)K_Y=(.*)ENV_X(.*)ENV_Y(.*)\r\n");
		//	regex  pattern_2("K_X=(.*)K_Y=(.*)\n");
		smatch results;
		if (regex_match(text, results, pattern)) {

			ofstream outfile_x;
			ofstream outfile_y;
			ofstream outfile_env_x;
			ofstream outfile_env_y;

			outfile_x.open("k_x.txt");
			//for (auto it = results.begin(); it != results.end(); ++it)
			auto k_x = results.begin() + 1;
			outfile_x << *k_x  << endl;
			outfile_x.close();

			outfile_y.open("k_y.txt");
			auto k_y = results.begin() + 2;
			outfile_y << *k_y  << endl;
			outfile_y.close();

			outfile_env_x.open("env_x.txt");
			auto env_x = results.begin() + 3;
			outfile_env_x << *env_x << endl;
			outfile_env_x.close();

			outfile_env_y.open("env_y.txt");
			auto env_y = results.begin() + 4;
			outfile_env_y << *env_y << endl;
			outfile_env_y.close();

			//cout << *k_x <<"\n"<< *k_y << "\n " << endl;
			//outfile_x << *k_x << "\n" << *k_y << "\n " << endl;
		}
		else {
			//cout << "match failed: " << text << endl;
		}

	}
	MessageBox(L"环境变量已存储");
}


void CSerialTestDlg::OnBnClickedButtonSaveHj()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_setOk == true)    //判断是否打开并初始化串口
	{
		UpdateData(TRUE);  //读取编辑框内容
		m_ctrlComm.put_Output(COleVariant(m_edit_read)); //发送数据
		UpdateData(false);
		//k_x(CString a);

		char k_x[MAX_LINE];
		FILE* fp_x;
		CString x;
		if ((fp_x = fopen("k_x.txt", "r")) == NULL)
		{
			perror("fail to read");
			MessageBox(L"fail_x");
			exit(1);
		}
		while (fgets(k_x, MAX_LINE, fp_x) != NULL)
		{
			CString x(k_x);

			UpdateData(TRUE);  //读取编辑框内容
			m_edit_k_x = x;
			UpdateData(false);
			//	MessageBox(m_edit_k_x);
				//m_ctrlComm.put_Output(COleVariant(m_edit_k_x)); //发送数据
		}

		char k_y[MAX_LINE];
		FILE* fp_y;
		CString y;
		if ((fp_y = fopen("k_y.txt", "r")) == NULL)
		{
			perror("fail to read");
			MessageBox(L"fail_y");
			exit(1);
		}
		while (fgets(k_y, MAX_LINE, fp_y) != NULL)
		{
			CString y(k_y);

			UpdateData(TRUE);  //读取编辑框内容
			m_edit_k_y = y;
			UpdateData(false);
			//MessageBox(m_edit_k_y);
			//m_ctrlComm.put_Output(COleVariant(m_edit_k_y)); //发送数据
		}

		char k_env_x[MAX_LINE];
		FILE* fp_env_x;
		CString env_x;
		if ((fp_env_x = fopen("env_x.txt", "r")) == NULL)
		{
			perror("fail to read");
			MessageBox(L"fail_env_x");
			exit(1);
		}
		while (fgets(k_env_x, MAX_LINE, fp_env_x) != NULL)
		{
			CString env_x(k_env_x);

			UpdateData(TRUE);  //读取编辑框内容
			m_edit_hj_x = env_x;
			UpdateData(false);
			//MessageBox(m_edit_hj_x);
		}

		char k_env_y[MAX_LINE];
		FILE* fp_env_y;
		CString env_y;
		if ((fp_env_y = fopen("env_y.txt", "r")) == NULL)
		{
			perror("fail to read");
			MessageBox(L"fail_env_y");
			exit(1);
		}
		while (fgets(k_env_y, MAX_LINE, fp_env_y) != NULL)
		{
			CString env_y(k_env_y);

			UpdateData(TRUE);  //读取编辑框内容
			m_edit_hj_y = env_y;
			UpdateData(false);
			//MessageBox(m_edit_hj_y);
		}

	}

}



void CSerialTestDlg::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
	static int i;

	++i;
	if (i == 4)
		i = 1;
	switch (i)
	{
	case 1:
		m_ctrlComm.put_Output(COleVariant(m_edit_k_x)); //发送数据
		break;
	case 2:
		m_ctrlComm.put_Output(COleVariant(m_edit_k_y)); //发送数据
		break;
	case 3:
		m_ctrlComm.put_Output(COleVariant(m_edit_hj_x)); //发送数据
		break;
	case 4:
		m_ctrlComm.put_Output(COleVariant(m_edit_hj_y)); //发送数据
		break;
	}

}
