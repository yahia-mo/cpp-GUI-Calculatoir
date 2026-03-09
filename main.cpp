#include <wx/wx.h>
#include <vector>

class Calculator : public wxFrame {
public:
    Calculator() : wxFrame(NULL, wxID_ANY, "Simple Calculator",
                           wxDefaultPosition, wxSize(300,400)) 
    {
        wxPanel* panel = new wxPanel(this);

        display = new wxTextCtrl(panel, wxID_ANY, "",
                                 wxPoint(10,10), wxSize(260,40),
                                 wxTE_RIGHT);
        wxGridSizer* top_grid = new wxGridSizer(1,2,5,5);
        wxButton* topBottom = new wxButton(panel, 1100, "Delete");
        top_grid->Add(topBottom, 1, wxEXPAND);
        Bind(wxEVT_BUTTON, &Calculator::OnButtonClicked, this, 1100);
        
        wxButton* clear = new wxButton(panel, 1101, "Clear");
        top_grid->Add(clear, 1, wxEXPAND);
        Bind(wxEVT_BUTTON, &Calculator::OnButtonClicked, this, 1101);

        wxGridSizer* grid = new wxGridSizer(4, 4, 5, 5);

        wxString buttons[16] = 
        {
            "7","8","9","/",
            "4","5","6","*",
            "1","2","3","-",
            "0",".","=","+"
        };

        for(int i = 0; i < 16; i++) 
        {
            wxButton* btn = new wxButton(panel, 1000+i, buttons[i]);
            grid->Add(btn, 1, wxEXPAND);
            Bind(wxEVT_BUTTON, &Calculator::OnButtonClicked, this, 1000+i);
        }

        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
        vbox->Add(display, 0, wxEXPAND | wxALL, 10);
        vbox->Add(top_grid,0,wxEXPAND | wxALL, 10);

        vbox->Add(grid, 1, wxEXPAND | wxALL, 10);

        panel->SetSizer(vbox);
    }

private:
    wxTextCtrl* display;

    void OnButtonClicked(wxCommandEvent& event) 
    {
        wxButton* btn = (wxButton*)event.GetEventObject();
        wxString value = btn->GetLabel();
        
        if (display->GetValue() == "Math Error") {
            display->Clear();
        }   

        if(value == "Clear") {
            display->Clear();
        } else if(value == "=") {
            Calculate();
        }
        else if(value =="Delete")
        {
            wxString value = display->GetValue();
            if (!value.IsEmpty())
            {   
                value.RemoveLast();
                display->SetValue(value);

            }
        
        }
        else if(value == "+" || value == "-" || value == "*" || value == "/" || value == ".")
        {
            wxString text = display->GetValue();

            if (text.IsEmpty()) return;

            wxChar last = text.Last();

            if (last == '+' || last == '-' || last == '*' || last == '/' || last == '.')
                return;

            display->AppendText(value);
        }
         else
        {
            display->AppendText(value);
        }
    }

    void Calculate() {
        wxString expr = display->GetValue();
        if (expr.IsEmpty()) return;

        std::vector<double> numbers;
        std::vector<wxChar> operators;
        wxString currentNumber = "";

        for (size_t i = 0; i < expr.length(); i++)
        {
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
                    // new line here .
                    
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
        
        // check if there is an extra operation
        
        if (numbers.size() <= operators.size())
        {
            operators.pop_back();
        }
        
        // Priority for multiply and divide 
        for (size_t i = 0; i < operators.size(); )
        {
            if (operators[i]=='*' || operators[i]=='/')
            {
                double a = numbers[i];
                double b = numbers[i+1];

                if (operators[i]=='/' && b==0)
                {
                    display->SetValue("Math Error");
                    return;
                }

                double res = (operators[i]=='*') ? (a*b) : (a/b);

                numbers.erase(numbers.begin()+i);
                numbers.erase(numbers.begin()+i);

                numbers.insert(numbers.begin()+i, res);
                operators.erase(operators.begin()+i);
            }
            else i++;
        }

        
        double final_result = numbers[0]; 
    
        for (size_t i = 0; i < operators.size(); i++)
        {
            // if (i + 1 >= numbers.size()) break; 
            
            wxChar op = operators[i];
            double nextNum = numbers[i+1];

            if (op == '+') final_result += nextNum;
            else if (op == '-') final_result -= nextNum;
        
        }

        if (int (final_result) == final_result)
        {
            display->SetValue(wxString::Format("%d", (int)final_result));
        }else
        {
        display->SetValue(wxString::Format("%.2f", final_result));
        }
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