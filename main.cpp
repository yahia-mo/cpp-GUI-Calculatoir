#include <wx/wx.h>
#include <vector>

class Calculator : public wxFrame {
public:
    Calculator() : wxFrame(NULL, wxID_ANY, "Simple Calculator",
                           wxDefaultPosition, wxSize(300,400)) {
        wxPanel* panel = new wxPanel(this);

        display = new wxTextCtrl(panel, wxID_ANY, "",
                                 wxPoint(10,10), wxSize(260,40),
                                 wxTE_RIGHT);

        wxGridSizer* grid = new wxGridSizer(4, 4, 5, 5);

        wxString buttons[16] = 
        {
            "7","8","9","/",
            "4","5","6","*",
            "1","2","3","-",
            "0","C","=","+"
        };

        for(int i = 0; i < 16; i++) 
        {
            wxButton* btn = new wxButton(panel, 1000+i, buttons[i]);
            grid->Add(btn, 1, wxEXPAND);
            Bind(wxEVT_BUTTON, &Calculator::OnButtonClicked, this, 1000+i);
        }

        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
        vbox->Add(display, 0, wxEXPAND | wxALL, 10);
        vbox->Add(grid, 1, wxEXPAND | wxALL, 10);

        panel->SetSizer(vbox);
    }

private:
    wxTextCtrl* display;

    void OnButtonClicked(wxCommandEvent& event) 
    {
        wxButton* btn = (wxButton*)event.GetEventObject();
        wxString value = btn->GetLabel();
        
        if (display->GetValue() == "Error") {
            display->Clear();
        }   

        if(value == "C") {
            display->Clear();
        } else if(value == "=") {
            Calculate();
        } else {
            display->AppendText(value);
        }
    }

    void Calculate() {
        wxString expr = display->GetValue();
        if (expr.IsEmpty()) return;

        std::vector<double> numbers;
        std::vector<wxChar> operators;
        wxString currentNumber = "";

        for (size_t i = 0; i < expr.length(); i++) {
            wxChar c = expr[i];
            
            if (wxIsdigit(c) || c == '.') 
            {
                currentNumber += c;
            } 
            else if (c == '+' || c == '-' || c == '*' || c == '/')
            {
                if (!currentNumber.IsEmpty())
                {
                    double num;
                    currentNumber.ToDouble(&num);
                    numbers.push_back(num); 
                }
                operators.push_back(c);     
                currentNumber = "";        
            }
        }
        
       
        if (!currentNumber.IsEmpty())
        {
            double num;
            currentNumber.ToDouble(&num);
            numbers.push_back(num);
        }

        if (numbers.empty()) return;

        double result = numbers[0]; 
        
        for (size_t i = 0; i < operators.size(); i++)
        {
            if (i + 1 >= numbers.size()) break; 
            
            wxChar op = operators[i];
            double nextNum = numbers[i+1];

            if (op == '+') result += nextNum;
            else if (op == '-') result -= nextNum;
            else if (op == '*') result *= nextNum;
            else if (op == '/') {
                if (nextNum != 0) {
                    result /= nextNum;
                } else {
                    display->SetValue("Error");
                    return;
                }
            }
        }

       
        display->SetValue(wxString::Format("%.2f", result));
    }
};

class MyApp : public wxApp 
{
public:
    virtual bool OnInit() 
    {
        Calculator* calc = new Calculator();
        calc->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);