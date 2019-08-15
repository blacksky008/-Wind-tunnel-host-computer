
// SerialTestDlg.h: 头文件
//

#pragma once
#include "mscomm1.h"
#include "afxwin.h"
#include "mscomm1.h"


// CSerialTestDlg 对话框
class CSerialTestDlg : public CDialogEx
{
// 构造
public:
	CSerialTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboCom;
	afx_msg void OnCbnSelchangeComboCom();
	CMscomm1 m_ctrlComm;
private:
	bool m_setOk;
public:
	CString m_strTXData;
	CString m_strRXData;
	DECLARE_EVENTSINK_MAP()
	void OnComm();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonCleanup();
	afx_msg void OnEnChangeEditRxdata();
	afx_msg void OnBnClickedButtonInputwrite();
	afx_msg void OnBnClickedButtonSure();
	CString m_edit_sure ;
	afx_msg void OnBnClickedButtonCalibration();
	CString m_edit_calibration;
	CString m_edit_k_x;
	afx_msg void OnBnClickedButtonSaveHj();
	afx_msg void OnBnClickedButtonWrite();
	CString m_edit_read;
	CString m_edit_k_y;
	CString m_edit_hj_x;
	CString m_edit_hj_y;
	afx_msg void OnBnClickedButtonChange();
};
