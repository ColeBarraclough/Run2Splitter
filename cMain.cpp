#include "cMain.h"
#include <Windows.h>
#include "proc.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()



cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Run2Splitter", wxPoint(0, 0), wxSize(250, 1025), wxSTAY_ON_TOP)
{	
	HourMinuteSecond h(0, 0, 0);	// Create Splitter Object
	HourMinuteSecond s(0, 0, 0);

	m_best_times_file = boost::filesystem::current_path();
	m_best_times_file += "/BestTimes.txt";

	m_current_best_times_file = boost::filesystem::current_path();
	m_current_best_times_file += "/CurrentBestTimes.txt";

	if (boost::filesystem::exists(m_best_times_file)) {
		boost::filesystem::ifstream file(m_best_times_file);
		boost::filesystem::ifstream file_two(m_current_best_times_file);
		std::string input;
		double temp;
		std::vector<split> subSplits(25);
		std::vector<split> testSubSplits(25);
		for (int i = 0; i <= 24; ++i) {
			subSplits.push_back(split(h, i + "", NULL_TIME, NULL_TIME));
		}
		split mainSplit(h, "Main");
		m_splitter.setMainSplit(mainSplit);
		m_splitter.setSubSplits(subSplits);
		for (int i = 0; i <= 24; ++i) {
			getline(file, input);
			getline(file, input);
			temp = atof(input.c_str());
			s.setSeconds(temp);
			getline(file, input);
			temp = atof(input.c_str());
			s.setMinutes(temp);
			getline(file, input);
			temp = atof(input.c_str());
			s.setHours(temp);
			m_splitter.getSplit(i).setBestTime(s);
		}

		for (int i = 0; i <= 24; ++i) {
			getline(file_two, input);
			getline(file_two, input);
			temp = atof(input.c_str());
			s.setSeconds(temp);
			getline(file_two, input);
			temp = atof(input.c_str());
			s.setMinutes(temp);
			getline(file_two, input);
			temp = atof(input.c_str());
			s.setHours(temp);
			m_splitter.getSplit(i).setBestRunTime(s);
		}

		file.close();
		file_two.close();
	}
	else {
		std::vector<split> subSplits(25);
		std::vector<split> testSubSplits(25);
		for (int i = 0; i <= 24; ++i) {
			subSplits.push_back(split(h, i + "", NULL_TIME, NULL_TIME));
		}
		split mainSplit(h, "Main");
		m_splitter.setMainSplit(mainSplit);
		m_splitter.setSubSplits(subSplits);
	}
	wxMessageDialog(this, m_best_times_file.c_str(), "", wxYES_NO).ShowModal();
	
	currentBest = m_splitter.getSplit(24).getBestRunTime();

	this->SetBackgroundColour(wxColour(50, 50, 50, 255)); // Create Display

	w_timer = new wxTimer(this, 10002);

	m_title = new wxStaticText(this, 10005, "Run 2", wxPoint(50, 25), wxSize(250, 50));
	m_title->SetFont(wxFont((wxFontInfo(30).Bold())));

	m_main_time = new wxStaticText(this, wxID_ANY, "", wxPoint(0, 950), wxSize(250, 100), wxTE_RIGHT);
	m_main_time->SetFont(wxFont((wxFontInfo(25).Bold().Family(wxFONTFAMILY_TELETYPE))));

	// Create a wxGrid object
	grid = new wxGrid(this, -1, wxPoint(0, 100), wxSize(250, 832));
	grid->UseNativeColHeader(false);
	grid->EnableEditing(false);
	grid->CreateGrid(25, 2);
	grid->SetDefaultCellTextColour(wxColour(0, 200, 0, 255));
	grid->SetDefaultCellBackgroundColour(wxColour(0, 0, 75, 255));
	//grid->SetDefaultCellTextColour(wxColour(0, 200, 0, 255));
	grid->SetDefaultRowSize(32, true);
	grid->SetDefaultColSize(83, true);
	grid->DisableDragColSize();
	grid->DisableDragRowSize();
	grid->SetColLabelValue(0, "");
	grid->SetColLabelValue(1, "");

	Connect(wxEVT_TIMER, wxCommandEventHandler(cMain::OnTimer));
	w_timer->Start(10);

	for (int i = 0; i <= 24; i++) {
		grid->SetCellValue(i, 1, m_splitter.getSplit(i).getBestRunTime().toString());
	}
}

cMain::~cMain() {
	w_timer->Stop();
	delete(w_timer);
}

void cMain::OnTimer(wxCommandEvent& evt)
{
	if (procId == 0) {
		procId = GetProcId(L"flashplayer_32_sa_debug.exe");
		evt.Skip();
		return;
	}
	if (!foundLevel) {
		moduleBase = GetModuleBaseAdress(procId, L"flashplayer_32_sa_debug.exe");

		//Get handle to process
		hProcess = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		//Resolve base address of pointer chain
		//dynamicPtrBaseAddr = moduleBase + 0x00D1F180;// Relative offset
		dynamicPtrBaseAddr = moduleBase + 0x00D2292C;

		//Resolve level pointer chain
		//levelOffsets = { 0x4D0, 0xC, 0x4, 0x2C, 0xB4, 0x374 };
		levelOffsets = { 0x24, 0x90, 0x7C, 0x2B0, 0xC, 0x0, 0x6C };
		levelAddr = FindMAAddy(hProcess, dynamicPtrBaseAddr, levelOffsets);
		foundLevel = true;

	}
	ReadProcessMemory(hProcess, (BYTE*)levelAddr, &levelValue, sizeof(levelValue), nullptr);

	if (oldLevelValue != 1 && levelValue == 1 && !(oldLevelValue == 25 && levelValue == 1)) {
		m_splitter.end();
		done = true;
		Reset();
	}

	if ((GetKeyState(0x52) & 0x8000) && m_splitter.getGoing()) {
		done = false;
		m_splitter.end();
		m_splitter.reset();
		ClearFirstColumn();
		if (wxMessageDialog(this, "Would you like to save your splits?", "", wxYES_NO).ShowModal() == wxID_YES) {
			SaveBestSplits();
			Reset();
		}
		else {
			Reset();
		}
	}
	if ((levelValue == 1) && (GetKeyState(32) & 0x8000) && (!m_splitter.getGoing()) && !done) {
		ClearFirstColumn();
		oldLevelValue = 1;
		m_splitter.reset();
		m_splitter.start();
	}

	if (m_splitter.getGoing()) {
		if (levelValue == 26 || (oldLevelValue == 25 && levelValue == 1) && !done) {
			grid->SetCellValue(24, 1, m_splitter.doSplit().toString());
			m_splitter.end();
			done = true;
			if (wxMessageDialog(this, "Would you like to save your splits?", "", wxYES_NO).ShowModal() == wxID_YES) {
				if (m_splitter.getMainSplit().getTime() < currentBest || currentBest == NULL_TIME) {
					currentBest = m_splitter.getMainSplit().getTime();
					Save();
				}
				else {
					SaveBestSplits();
				}
				Reset();
			}
			else
			{
				Reset();
			}
			evt.Skip();
			return;
		}
		if (m_splitter.getCurrentStanding() < DEF_TIME || m_splitter.getCurrentSplit().getBestRunTime() == NULL_TIME) {
			grid->SetCellTextColour(levelValue - 1, 0, wxColour(0, 255, 0, 255));
		}
		else {
			grid->SetCellTextColour(levelValue - 1, 0, wxColour(255, 0, 0, 255));
		}
		if (oldLevelValue != levelValue) {
			if (m_splitter.getCurrentSplit().getTime() < m_splitter.getCurrentSplit().getBestTime()) {
				grid->SetCellTextColour(levelValue - 2, 0, wxColour(255, 215, 0, 255));
			}
			grid->SetCellValue(levelValue - 2, 1, m_splitter.doSplit().toString());
			oldLevelValue = levelValue;
		}
		m_splitter.update();
		m_main_time->SetLabel(m_splitter.getMainSplit().getTime().toString());
		grid->SetCellValue(levelValue - 1, 0, (m_splitter.getCurrentStanding().toString()));
	}
	evt.Skip();
}

bool cMain::Save() {
	boost::filesystem::ofstream ofs;
	ofs.open(m_current_best_times_file, std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < 25; i++) {
		ofs << i + 1;
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestRunTime().getSeconds();
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestRunTime().getMinutes();
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestRunTime().getHours();
		ofs << "\n";
	}
	ofs.close();
	
	ofs.open(m_best_times_file, std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < 25; i++) {
		ofs << i + 1;
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestTime().getSeconds();
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestTime().getMinutes();
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestTime().getHours();
		ofs << "\n";
	}
	ofs.close();
	return true;
}

void cMain::SaveBestSplits() {
	boost::filesystem::ofstream ofs;
	ofs.open(m_best_times_file, std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < 25; i++) {
		ofs << i + 1;
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestTime().getSeconds();
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestTime().getMinutes();
		ofs << "\n";
		ofs << m_splitter.getSplit(i).getBestTime().getHours();
		ofs << "\n";
	}
	ofs.close();
}


void cMain::ClearSplits() {
	boost::filesystem::ofstream ofs;
	ofs.open(m_current_best_times_file, std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < 25; i++) {
		ofs << i + 1;
		ofs << "\n";
		ofs << -1;
		ofs << "\n";
		ofs << 0;
		ofs << "\n";
		ofs << 0;
		ofs << "\n";
	}
	ofs.close();

	ofs.open(m_best_times_file, std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < 25; i++) {
		ofs << i + 1;
		ofs << "\n";
		ofs << -1;
		ofs << "\n";
		ofs << 0;
		ofs << "\n";
		ofs << 0;
		ofs << "\n";
	}
	ofs.close();
}

void cMain::ClearFirstColumn() {
	for (int i = 0; i < 25; i++) {
		grid->SetCellValue(i, 0, "");
	}
}

void cMain::Reset() {
	ClearFirstColumn();

	HourMinuteSecond s(0, 0, 0);
	boost::filesystem::ifstream file(m_best_times_file);
	std::string input;
	boost::filesystem::ifstream file_two(m_current_best_times_file);
	double temp;

	for (int i = 0; i <= 24; ++i) {
		getline(file, input);
		getline(file, input);
		temp = atof(input.c_str());
		s.setSeconds(temp);
		getline(file, input);
		temp = atof(input.c_str());
		s.setMinutes(temp);
		getline(file, input);
		temp = atof(input.c_str());
		s.setHours(temp);
		m_splitter.getSplit(i).setBestTime(s);
	}

	for (int i = 0; i <= 24; ++i) {
		getline(file_two, input);
		getline(file_two, input);
		temp = atof(input.c_str());
		s.setSeconds(temp);
		getline(file_two, input);
		temp = atof(input.c_str());
		s.setMinutes(temp);
		getline(file_two, input);
		temp = atof(input.c_str());
		s.setHours(temp);
		m_splitter.getSplit(i).setBestRunTime(s);
	}

	file.close();
	file_two.close();
	for (int i = 0; i <= 24; i++) {
		grid->SetCellValue(i, 1, m_splitter.getSplit(i).getBestRunTime().toString());
	}
	done = false;
}