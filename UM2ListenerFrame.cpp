/*
 * UM2ListenerFrame.cpp
 *
 *  Created on: 7 feb 2015
 *      Author: johan
 */


#include "UM2ListenerFrame.h"

#include <wx/splitter.h>
#include <wx/rawbmp.h>


enum
{
ID_Hello = 1
};

wxDEFINE_EVENT(CMD_AddText, wxCommandEvent);

wxBEGIN_EVENT_TABLE(UM2ListenerFrame, wxFrame)
EVT_MENU(ID_Hello, UM2ListenerFrame::OnHello)
EVT_MENU(wxID_EXIT, UM2ListenerFrame::OnExit)
EVT_MENU(wxID_ABOUT, UM2ListenerFrame::OnAbout)
EVT_COMMAND(wxID_ANY, CMD_AddText, UM2ListenerFrame::handle_add_text )
wxEND_EVENT_TABLE()


UM2ListenerFrame::UM2ListenerFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
wxMenu *menuFile = new wxMenu;
menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
"Help string shown in status bar for this menu item");
menuFile->AppendSeparator();
menuFile->Append(wxID_EXIT);
wxMenu *menuHelp = new wxMenu;
menuHelp->Append(wxID_ABOUT);
wxMenuBar *menuBar = new wxMenuBar;
menuBar->Append( menuFile, "&File" );
menuBar->Append( menuHelp, "&Help" );
SetMenuBar( menuBar );
CreateStatusBar();
SetStatusText( "Welcome to wxWidgets!" );

//wxPanel *panel = new wxPanel(this, -1);
//wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

//wxPanel *midPan = new wxPanel(panel, wxID_ANY);
//midPan->SetBackgroundColour(wxColour("red"));
//
//wxTextCtrl* text = new wxTextCtrl(panel, wxID_ANY,"",wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
//
//vbox->Add(midPan, 1, wxEXPAND | wxALL, 20);
//vbox->Add(text, 1, wxEXPAND | wxALL, 20);

wxSplitterWindow * spw = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
wxPanel *midPan = new wxPanel(spw, wxID_ANY);
midPan->SetBackgroundColour(wxColour("red"));
text = new wxTextCtrl(spw, wxID_ANY,"",wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

spw->SplitVertically(midPan, text, 100);
spw->SetSashGravity(0.5);
//panel->SetSizer(vbox);

wxBitmap bmp(40,20,24);
wxNativePixelData pd(bmp);

}

void UM2ListenerFrame::post_to_log( const std::string& str ){
	std::cout << str;
	wxCommandEvent* event = new wxCommandEvent(CMD_AddText);
	event->SetString(str);
	QueueEvent( event );
}
void UM2ListenerFrame::handle_add_text( wxCommandEvent& event ){
	text->AppendText( event.GetString() );
}

void UM2ListenerFrame::OnExit(wxCommandEvent& event)
{
Close( true );
}

void UM2ListenerFrame::OnAbout(wxCommandEvent& event)
{
wxMessageBox( "This is a wxWidgets' Hello world sample",
"About Hello World", wxOK | wxICON_INFORMATION );
}

void UM2ListenerFrame::OnHello(wxCommandEvent& event)
{
wxLogMessage("Hello world from wxWidgets!");
}


