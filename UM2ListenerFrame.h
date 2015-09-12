/*
 * UM2ListenerFrame.h
 *
 *  Created on: 7 feb 2015
 *      Author: johan
 */

#ifndef UM2LISTENERFRAME_H_
#define UM2LISTENERFRAME_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

wxDECLARE_EVENT(CMD_AddText, wxCommandEvent);

class UM2ListenerFrame: public wxFrame
{
public:
UM2ListenerFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
void handle_add_text( wxCommandEvent& event );
void post_to_log( const std::string& str );
private:
void OnHello(wxCommandEvent& event);
void OnExit(wxCommandEvent& event);
void OnAbout(wxCommandEvent& event);
wxDECLARE_EVENT_TABLE();

wxTextCtrl* text;
};




#endif /* UM2LISTENERFRAME_H_ */
