// Wizardaw.cpp : implementation file
//

#include "stdafx.h"
#include "Wizard.h"
#include "Wizardaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#import "C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\IDE\devbld.pkg"

// This is called immediately after the custom AppWizard is loaded.  Initialize
//  the state of the custom AppWizard here.
void CWizardAppWiz::InitCustomAppWiz()
{
	// There are no steps in this custom AppWizard.
	SetNumberOfSteps(0);

	// Add build step to .hpj if there is one
	m_Dictionary[_T("HELP")] = _T("1");

	// TODO: Add any other custom AppWizard-wide initialization here.
}

// This is called just before the custom AppWizard is unloaded.
void CWizardAppWiz::ExitCustomAppWiz()
{
	// TODO: Add code here to deallocate resources used by the custom AppWizard
}

// This is called when the user clicks "Create..." on the New Project dialog
CAppWizStepDlg* CWizardAppWiz::Next(CAppWizStepDlg* pDlg)
{
	ASSERT(pDlg == NULL);	// By default, this custom AppWizard has no steps

	// Set template macros based on the project name entered by the user.

	// Get value of $$root$$ (already set by AppWizard)
	CString strRoot;
	m_Dictionary.Lookup(_T("root"), strRoot);
	
	// Set value of $$Doc$$, $$DOC$$
	CString strDoc = strRoot.Left(6);
	m_Dictionary[_T("Doc")] = strDoc;
	strDoc.MakeUpper();
	m_Dictionary[_T("DOC")] = strDoc;

	// Set value of $$MAC_TYPE$$
	strRoot = strRoot.Left(4);
	int nLen = strRoot.GetLength();
	if (strRoot.GetLength() < 4)
	{
		CString strPad(_T(' '), 4 - nLen);
		strRoot += strPad;
	}
	strRoot.MakeUpper();
	m_Dictionary[_T("MAC_TYPE")] = strRoot;

	// Return NULL to indicate there are no more steps.  (In this case, there are
	//  no steps at all.)
	return NULL;
}

void CWizardAppWiz::CustomizeProject(IBuildProject* pProject)
{
	// TODO: Add code here to customize the project.  If you don't wish
	//  to customize project, you may remove this virtual override.
	
	// This is called immediately after the default Debug and Release
	//  configurations have been created for each platform.  You may customize
	//  existing configurations on this project by using the methods
	//  of IBuildProject and IConfiguration such as AddToolSettings,
	//  RemoveToolSettings, and AddCustomBuildStep. These are documented in
	//  the Developer Studio object model documentation.

	// WARNING!!  IBuildProject and all interfaces you can get from it are OLE
	//  COM interfaces.  You must be careful to release all new interfaces
	//  you acquire.  In accordance with the standard rules of COM, you must
	//  NOT release pProject, unless you explicitly AddRef it, since pProject
	//  is passed as an "in" parameter to this function.  See the documentation
	//  on CCustomAppWiz::CustomizeProject for more information.

	  using namespace DSProjectSystem;

      long lNumConfigs;
      IConfigurationsPtr pConfigs;
      IBuildProjectPtr pProj;

      // Needed to convert IBuildProject to the DSProjectSystem namespace
      pProj.Attach((DSProjectSystem::IBuildProject*)pProject, true);

      pProj->get_Configurations(&pConfigs);
      pConfigs->get_Count(&lNumConfigs);
      //Get each individual configuration
      for (long j = 1 ; j < lNumConfigs+1 ; j++)
      {
         _bstr_t varTool;
         _bstr_t varSwitch;
         IConfigurationPtr pConfig;
         _variant_t varj = j;

         pConfig = pConfigs->Item(varj);

         // Remove Preprocessor def for MFC DLL specifier, _AFXDLL
         varTool   = "cl.exe";
         varSwitch = "/D \"_AFXDLL\"";
         pConfig->RemoveToolSettings(varTool, varSwitch, varj);

         varTool   = "rc.exe";
         varSwitch = "/d \"_AFXDLL\"";
         pConfig->RemoveToolSettings(varTool, varSwitch, varj);

 		 // MIKE, this is the magic line!
         pConfig->AddToolSettings("mfc", "0", varj);
 		 
         // OPTIONAL
         // Add Libs that MFC headers would have pulled in automatically
         // Feel free to customize this listing to your tastes
         varTool = "link.exe";
         varSwitch = "kernel32.lib user32.lib gdi32.lib winspool.lib "
                     "comdlg32.lib advapi32.lib shell32.lib ole32.lib "
                     "oleaut32.lib uuid.lib odbc32.lib odbccp32.lib";
         pConfig->AddToolSettings(varTool, varSwitch, varj);
      }
}


// Here we define one instance of the CWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global Wizardaw.
CWizardAppWiz Wizardaw;

