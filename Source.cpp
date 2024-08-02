
#include <wx/wxprec.h>
#include <wx/time.h> 
#include <wx/event.h>
//#include <string>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    void OnExit(wxCommandEvent& event);
    void OnSnooze(wxCommandEvent& event);
    void OnAlarm(wxCommandEvent& event);
    void OnTime(wxCommandEvent& event);
    void OnAlarmTime(wxCommandEvent& event);
    void UpdateTime(wxTimerEvent& event);
    wxString getTime();
    int wxStringToSec(wxString);
    void doAlarm(wxTimerEvent& event);
    int asciiToInt(wxUniChar);
    int aSec = 0;
    int cSec = 0;
    bool alarmOff = false;
    void openSnooze(wxUpdateUIEvent& event);
    void closeSnooze(wxTimerEvent& event);
    wxString intToTime(int);
    //void OnAlarmEnter(wxCommandEvent& event);
    wxButton *snoozeButton = nullptr;
    wxButton* setAlarmButton = nullptr;
    wxStaticBox* clockBox = nullptr;
    wxTextCtrl* setAlarmText = nullptr;
    wxStaticBox* alarmClockTitle = nullptr;
    wxString alarmTime = "NULL";
    wxTimer timer;
    wxTimer aTimer;
    wxTimer sTimer;
    wxFont font, font2;

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Snooze = 1
};
enum
{
    ID_Alarm = 2
};
enum
{
    ID_Time = 3
};
enum
{
    ID_AlarmTime = 4
};
enum
{
    ID_Timer = 5
};
enum
{
    ID_Alarming = 6
};
enum
{
    ID_sTimer = 7
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_BUTTON(ID_Snooze, MyFrame::OnSnooze)
EVT_MENU(ID_Alarm, MyFrame::OnAlarm)
EVT_MENU(ID_Time, MyFrame::OnTime)
EVT_MENU(ID_AlarmTime, MyFrame::OnAlarmTime)
EVT_TIMER(ID_Timer, MyFrame::UpdateTime)
EVT_TIMER(ID_Alarming, MyFrame::doAlarm)
EVT_UPDATE_UI(ID_Snooze, MyFrame::openSnooze)
EVT_TIMER(ID_sTimer, MyFrame::closeSnooze)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}
MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Alarm Clock"),
    timer (this, ID_Timer),
    aTimer (this, ID_Alarming),
    sTimer (this, ID_sTimer)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    menuFile->Append(ID_Time,"&Time");
    menuFile->Append(ID_AlarmTime, "&Alarm");
    wxMenuBar *menuBar = new wxMenuBar;
     menuBar->Append(menuFile, "&Alarm Clock");
     SetMenuBar( menuBar );
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnAlarmTime, this, 4);
    Bind(wxEVT_MENU, &MyFrame::OnTime, this, 3);
    Bind(wxEVT_BUTTON, &MyFrame::OnAlarm, this, 2);
    Bind(wxEVT_BUTTON, &MyFrame::OnSnooze, this, 1);

    wxFont font(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_SLANT, wxFONTWEIGHT_EXTRAHEAVY, false);
    wxFont font2(23, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD, false);
    alarmClockTitle = new wxStaticBox(this, wxID_ANY, "Alarm Clock", wxPoint(10, 10), wxSize(240, 60));
    alarmClockTitle->SetFont(font);
    setAlarmButton = new wxButton(this, 2, "Set Alarm", wxPoint(20, 200), wxSize(100, 40));
    setAlarmText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(20, 250), wxSize(100, 40));
    snoozeButton = new wxButton(this, 1, "Snooze\n30 Seconds", wxPoint(140, 200), wxSize(100, 90));
    clockBox = new wxStaticBox(this,wxID_ANY, getTime(), wxPoint(40, 80), wxSize(180, 110));
    clockBox->SetFont(font2);
    timer.Start(1000);
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void MyFrame::OnSnooze(wxCommandEvent& event)
{
  
    alarmOff = false;  
    wxMessageBox("snooze", "snooze");
    aSec = wxStringToSec(getTime()) + 30;
    cSec = wxStringToSec(getTime());
    alarmTime = intToTime(aSec);
    aTimer.Start(1000);
    event.Skip();
}
void MyFrame::OnAlarm(wxCommandEvent& event)
{
    alarmTime = setAlarmText->GetValue();
    setAlarmText->Clear();
    bool format = true;
    for (int i = 0; i < alarmTime.size(); i++)
    {
        if (i == 0 || i == 1 || i == 3 || i == 4 || i == 6 || i == 7)
        {
            if (alarmTime[i] != '1' && alarmTime[i] != '2' && alarmTime[i] != '3' && alarmTime[i] != '4' && alarmTime[i] != '5' &&
                alarmTime[i] != '6' && alarmTime[i] != '7' && alarmTime[i] != '8' && alarmTime[i] != '9' && alarmTime[i] != '0')
            {
                format = false;
            }
        }
        if (i == 2 || i == 5)
        {
            if (alarmTime[i] != ':')
            {
                format = false;
            }
        }
    }
    if (alarmTime.size() != 8)
    {
        format = false;
    }
    if (format == false)
    {
        alarmTime = "NULL";
        wxMessageBox("INPUTED ALARM INCORRECT\nMUST ME\nXX:XX:XX", "WARNING");
    }
    if(format == true)
    {
        alarmOff = false;
        aSec = wxStringToSec(alarmTime);
        cSec = wxStringToSec(getTime());
        aTimer.Start(1000);
    }
    event.Skip();
}
void MyFrame::OnTime(wxCommandEvent& event)
{
        wxMessageBox(getTime(), "Time");
        event.Skip();
}
void MyFrame::OnAlarmTime(wxCommandEvent& event)
{
    if (alarmTime == "NULL")
    {
        wxMessageBox("No Alarm Set", "Alarm Time");
    }
    else
    {
        wxMessageBox(alarmTime, "Alarm Time");
    }
    event.Skip();
}
wxString MyFrame::getTime()
{
    return wxDateTime::Now().FormatTime();
}
void MyFrame::UpdateTime(wxTimerEvent& event)
{
    clockBox->Destroy();
    wxFont font2(23, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD, false);
    clockBox = new wxStaticBox(this, 3, getTime(), wxPoint(40, 80), wxSize(180, 110));
    clockBox->SetFont(font2);
    event.Skip();
}
int MyFrame::wxStringToSec(wxString str)
{
    int arr[6];
    int index = 0;
    for (int i = 0; i < 8; i++)
    {
        if (str[i] != ':')
        {
            arr[index] = asciiToInt(str[i]);
            index++;
        }
    }
    int sec = 0;
    sec = sec + ((arr[0] * 10) * 3600);
    sec = sec + (arr[1] * 3600);
    sec = sec + ((arr[2] * 10) * 60);
    sec = sec + (arr[3] * 60);
    sec = sec + (arr[4] * 10);
    sec = sec + arr[5];
    return sec;
}
void MyFrame::doAlarm(wxTimerEvent& event)
{
    cSec++;
    if (cSec > aSec && cSec > 86399) // if time in 00:00:00 makes it so the counter doesent go 23:60+:60+ 
    {
        cSec = 0;
    }
    if (aSec == cSec) // if alarm time is set to current time alarm goes off
    {
        alarmOff = true;
        wxMessageBox("ALARM", "ALARM");
        aTimer.Stop();
        alarmTime = "NULL";
    }
    event.Skip();
}
int MyFrame::asciiToInt(wxUniChar c)
{
    if (c == '1')
    {
        return 1;
    }
    else if (c == '2')
    {
        return 2;
    }
    else if (c == '3')
    {
        return 3;
    }
    else if (c == '4')
    {
        return 4;
    }
    else if (c == '5')
    {
        return 5;
    }
    else if (c == '6')
    {
        return 6;
    }
    else if (c == '7')
    {
        return 7;
    }
    else if (c == '8')
    {
        return 8;
    }
    else if (c == '9')
    {
        return 9;
    }
    else
    {
        return 0;
    }
}
void MyFrame::openSnooze(wxUpdateUIEvent& event)
{
    event.Enable(false);
    if (alarmOff == true)
    {
        event.Enable(true);
        sTimer.StartOnce(699);/// some numbers work and others dont want to work
    }
}
void MyFrame::closeSnooze(wxTimerEvent& event)
{
        if (alarmOff)
        {
            wxLogMessage("Snooze Button Off");
        }   
        alarmOff = false;
}
wxString MyFrame::intToTime(int num)
{
    wxString time, returnTime;
    time = getTime();
    int hour1, hour2, min1, min2, sec1, sec2, i = 3;
    hour1 = asciiToInt(time[0]);
    hour2 = asciiToInt(time[1]);
    min1 = asciiToInt(time[3]);
    min2 = asciiToInt(time[4]);
    sec1 = asciiToInt(time[6]);
    sec2 = asciiToInt(time[7]);
    while (i > 0)
    {
        if (sec1 < 5)
        {
            sec1++;
            i--;
        }
        else
        {
            sec1 = 0;
            if (min2 < 9)
            {
                min2++;
                i--;
            }
            else
            {
                min2 = 0;
                if (min1 < 5)
                {
                    min1++;
                    i--;
                }
                else
                {
                    min1 = 0;
                    if (hour2 < 9)
                    {
                        hour2++;
                        i--;
                    }
                    else
                    {
                        hour2 = 0;
                        if (hour1 < 23)
                        {
                            hour1++;
                            i--;
                        }
                        else
                        {
                            hour1 = 0;
                            i--;
                        }
                    }
                }
            }
        }
    }
    returnTime << hour1 << hour2 << ':' << min1 << min2 << ':' << sec1 << sec2;
    return returnTime;
}
