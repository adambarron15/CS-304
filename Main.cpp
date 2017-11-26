#include <wx/wx.h>
#include <wx/calctrl.h>
#include <wx/datetime.h>
#include <wx/grid.h>
#include <wx/choice.h>
#include <wx/stattext.h>

const int ID_SEARCH = 1000;

/**
   A frame that contains a text control.
*/


//TextFrame Class
class TextFrame : public wxFrame
{
public:
   /**
      Constructs the text control.
   */
   TextFrame(wxString str);
   void OnSearch(wxCommandEvent& event);


private:
   wxArrayString airports,num_tickets;
   wxString starting_loc,destination_loc,depart_date,return_date,num_child,num_adult;
   wxDateTime dd, rd;
   wxChoice* start_box;
   wxChoice* dest_box;
   wxChoice* num_adult_choice;
   wxChoice* num_child_choice;
   wxGrid* data_grid;
   wxMessageDialog* loc_error;

   wxCalendarCtrl* leave_cal;
   wxCalendarCtrl* return_cal;

   DECLARE_EVENT_TABLE()

};

/**
   An application that shows a frame with a text control.
*/
class TextApp : public wxApp
{
public:
   /**
      Constructs and shows the frame.
      @return true
   */
   virtual bool OnInit();
private:
   TextFrame* frame;

};

DECLARE_APP(TextApp)

IMPLEMENT_APP(TextApp)

BEGIN_EVENT_TABLE(TextFrame, wxFrame)
   EVT_BUTTON(ID_SEARCH, TextFrame::OnSearch)
END_EVENT_TABLE()



//TextFrame Constructor
TextFrame::TextFrame(wxString str)
   : wxFrame(NULL, -1, str)
{

    wxPoint myPoint(100,100);
    wxSize mySize(70,50);

    //Labels for UI items
    wxStaticText* dep_loc_label = new wxStaticText(this,001,wxString::FromUTF8("Flying from:"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE_HORIZONTAL,wxString::FromUTF8("Flying from Label"));
    wxStaticText* arr_loc_label = new wxStaticText(this,002,wxString::FromUTF8("Flying to:"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE_HORIZONTAL,wxString::FromUTF8("Flying to Label"));
    wxStaticText* dep_date_label = new wxStaticText(this,003,wxString::FromUTF8("Departing on:"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE_HORIZONTAL,wxString::FromUTF8("Departing on Label"));
    wxStaticText* arr_date_label = new wxStaticText(this,004,wxString::FromUTF8("Returning on:"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE_HORIZONTAL,wxString::FromUTF8("Returning on Label"));
    wxStaticText* num_adult_label = new wxStaticText(this,005,wxString::FromUTF8("Adults:"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE_HORIZONTAL,wxString::FromUTF8("Adults Label"));
    wxStaticText* num_child_label = new wxStaticText(this,006,wxString::FromUTF8("Children:"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE_HORIZONTAL,wxString::FromUTF8("Children Label"));

    loc_error = new wxMessageDialog(this,(wxString::FromUTF8("YOU MUST SELECT BOTH A DEPATURE AND AN ARRIVAL AIRPORT!")),
                                    (wxString::FromUTF8("ERROR GETTING RESULTS")),wxOK|wxICON_EXCLAMATION,wxDefaultPosition);

    //Destinations in the StartLocation and Destination List
    airports.Add(wxString::FromUTF8("LAX"));
    airports.Add(wxString::FromUTF8("OKC"));
    airports.Add(wxString::FromUTF8("OJC"));
    airports.Add(wxString::FromUTF8("ORD"));
    airports.Add(wxString::FromUTF8("JFK"));
    airports.Add(wxString::FromUTF8("MDA"));

    num_tickets.Add(wxString::FromUTF8("1"));
    num_tickets.Add(wxString::FromUTF8("2"));
    num_tickets.Add(wxString::FromUTF8("3"));
    num_tickets.Add(wxString::FromUTF8("4"));
    num_tickets.Add(wxString::FromUTF8("5"));
    num_tickets.Add(wxString::FromUTF8("6"));

    //Choice Boxes of airports to leave from and fly to
    start_box = new wxChoice(this,100,wxDefaultPosition,mySize,airports,1,wxDefaultValidator,wxString::FromUTF8("Starting Location"));
    dest_box = new wxChoice(this,101,wxDefaultPosition,mySize,airports,1,wxDefaultValidator,wxString::FromUTF8("Destination"));

    //Choice boxes of number of passengers
    num_adult_choice = new wxChoice(this,102,wxDefaultPosition,mySize,num_tickets,1,wxDefaultValidator,wxString::FromUTF8("Number of Adults"));
    num_child_choice = new wxChoice(this,103,wxDefaultPosition,mySize,num_tickets,1,wxDefaultValidator,wxString::FromUTF8("Number of Children"));

    leave_cal = new wxCalendarCtrl(this,200,wxDefaultDateTime,wxDefaultPosition,wxDefaultSize,wxCAL_SUNDAY_FIRST,wxString::FromUTF8("Departure Date"));
    return_cal = new wxCalendarCtrl(this,201,wxDefaultDateTime,wxDefaultPosition,wxDefaultSize,wxCAL_SUNDAY_FIRST,wxString::FromUTF8("Return Date"));

    //Button for searching for airline tickets based on result of the main frame
    wxButton* search_button = new wxButton(this,ID_SEARCH,wxString::FromUTF8("SEARCH"));

    //Grid for storing results returned from the API
    data_grid = new wxGrid(this,300,wxDefaultPosition,wxDefaultSize,wxWANTS_CHARS,wxString::FromUTF8("Results"));
    data_grid->CreateGrid(5,7);
    data_grid->SetColLabelValue(0,wxString::FromUTF8("Price"));
    data_grid->SetColLabelValue(1,wxString::FromUTF8("Leaving"));
    data_grid->SetColLabelValue(2,wxString::FromUTF8("Destination"));
    data_grid->SetColLabelValue(3,wxString::FromUTF8("Depart Date"));
    data_grid->SetColLabelValue(4,wxString::FromUTF8("Return Date"));
    data_grid->SetColLabelValue(5,wxString::FromUTF8("Adult(s)"));
    data_grid->SetColLabelValue(6,wxString::FromUTF8("Child(s)"));

    //The box sizers of the main frame
    wxBoxSizer* frame_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* column_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* grid_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* choice_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* frame_sizer_column1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* frame_sizer_column2 = new wxBoxSizer(wxHORIZONTAL);


    frame_sizer_column1->Add(dep_loc_label,400,wxALIGN_LEFT);
    frame_sizer_column1->Add(start_box,401,wxALIGN_LEFT);
    frame_sizer_column1->Add(arr_loc_label,404,wxALIGN_RIGHT);
    frame_sizer_column1->Add(dest_box,405,wxALIGN_RIGHT);

    frame_sizer_column2->Add(dep_date_label,402,wxALIGN_LEFT);
    frame_sizer_column2->Add(leave_cal,403,wxALIGN_LEFT);
    frame_sizer_column2->Add(arr_date_label,406,wxALIGN_RIGHT);
    frame_sizer_column2->Add(return_cal,407,wxALIGN_RIGHT);

    choice_sizer->Add(num_adult_label,408,wxALIGN_LEFT);
    choice_sizer->Add(num_adult_choice,409,wxALIGN_LEFT);
    choice_sizer->Add(num_child_label,410,wxALIGN_RIGHT);
    choice_sizer->Add(num_child_choice,411,wxALIGN_RIGHT);


    column_sizer->Add(frame_sizer_column1,412,wxALIGN_CENTER);
    column_sizer->Add(frame_sizer_column2,413,wxALIGN_CENTER);


    grid_sizer->Add(data_grid,414,wxALIGN_CENTER);

    button_sizer->Add(search_button);

    //Adds the column and button sizer to the frame sizer
    frame_sizer->Add(column_sizer,415,wxALIGN_CENTER);
    frame_sizer->Add(choice_sizer,417,wxALIGN_CENTER);
    frame_sizer->Add(grid_sizer,416,wxALIGN_CENTER);
    frame_sizer->Add(button_sizer,418,wxALIGN_CENTER);


    SetAutoLayout(true);
    SetSizer(frame_sizer);
    frame_sizer->FitInside(this);
    frame_sizer->Fit(this);


}

bool TextApp::OnInit()
{
   frame = new TextFrame(wxString::FromUTF8("Flight Search"));
   frame->Show(true);
   return true;
}


void TextFrame::OnSearch(wxCommandEvent& event)
{
   starting_loc = start_box->GetString(start_box->GetSelection());
   destination_loc = dest_box->GetString(dest_box->GetSelection());
   num_child = num_child_choice->GetString(num_child_choice->GetSelection());
   num_adult = num_adult_choice->GetString(num_adult_choice->GetSelection());
   dd = leave_cal->GetDate();
   rd = return_cal->GetDate();
   depart_date = dd.FormatISODate();
   return_date = rd.FormatISODate();

   if(starting_loc ==(wxString::FromUTF8(""))||destination_loc ==(wxString::FromUTF8(""))){
    loc_error->ShowModal();
    loc_error->Destroy();
   }
   else
   {
    for(int i = 0; i < 5;i++){
        data_grid->SetCellValue(i,0,wxString::FromUTF8("$156"));
        data_grid->SetCellValue(i,1,starting_loc);
        data_grid->SetCellValue(i,2,destination_loc);
        data_grid->SetCellValue(i,3,depart_date);
        data_grid->SetCellValue(i,4,return_date);
        data_grid->SetCellValue(i,5,num_adult);
        data_grid->SetCellValue(i,6,num_child);
    }
   }

}


