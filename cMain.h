#pragma once
#include "wx/wx.h"
#include "splitter.h"
#include "iterator.h"
#include "wx/custombgwin.h"
#include <thread>
#include <wx/grid.h>
#include <fstream>
#include <wx/arrstr.h>
#include "cMain.h"
#include <Windows.h>
#include "proc.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
public:
	//const char *m_best_times_file = "C:\\Users\\ColeB\\Documents\\BestSplitTimes.txt";
	boost::filesystem::path m_best_times_file;
	boost::filesystem::path m_current_best_times_file;
	
	wxButton *m_btn1 = nullptr;
	wxTextCtrl *m_txt1 = nullptr;
	wxTextCtrl* m_txt2 = nullptr;
	wxStaticText* m_title = nullptr;
	wxStaticText* m_main_time = nullptr;
	wxGrid* grid = nullptr;
	wxListBox *m_list1 = nullptr;
	wxListBox *m_list2 = nullptr;
	wxTimer *w_timer = nullptr;
	wxButton *m_btn2 = nullptr;
	splitter m_splitter;
	bool foundLevel = false;
	bool done = false;
	int levelValue = 1;
	int oldLevelValue = 1;
	HourMinuteSecond currentBest;

	DWORD procId;
	uintptr_t moduleBase;
	HANDLE hProcess;
	uintptr_t dynamicPtrBaseAddr;
	std::vector<unsigned int> levelOffsets;
	uintptr_t levelAddr;

	void OnTimer(wxCommandEvent& event);

	bool Save();

	void Save(const splitter& s, const char* filename);

	void Reset(splitter& s, const char* filename);

	void SaveBestSplits();

	void ClearSplits();

	void ClearFirstColumn();


	void Reset();

	wxDECLARE_EVENT_TABLE();
};

