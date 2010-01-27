#if !defined(AFX_WIZARDAW_H__97D19CEB_2C98_4727_9304_0996099E7144__INCLUDED_)
#define AFX_WIZARDAW_H__97D19CEB_2C98_4727_9304_0996099E7144__INCLUDED_

// Wizardaw.h : header file
//

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see Wizard.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CWizardAppWiz : public CCustomAppWiz
{
public:
	virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
		
	virtual void InitCustomAppWiz();
	virtual void ExitCustomAppWiz();
	virtual void CustomizeProject(IBuildProject* pProject);
};

// This declares the one instance of the CWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global Wizardaw.  (Its definition is in Wizardaw.cpp.)
extern CWizardAppWiz Wizardaw;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDAW_H__97D19CEB_2C98_4727_9304_0996099E7144__INCLUDED_)
