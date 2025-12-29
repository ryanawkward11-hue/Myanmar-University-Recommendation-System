#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <cctype>
#include <cstdlib>
using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

string getValidatedName(string prompt)
{
    string name;
    while (true)
    {
        cout << prompt;
        getline(cin, name);

        // Trim spaces
        size_t first = name.find_first_not_of(' ');
        size_t last = name.find_last_not_of(' ');
        if (first != string::npos && last != string::npos)
            name = name.substr(first, last - first + 1);
        else
            name = "";

        if (name.empty())
        {
            cout << "Name cannot be empty! Try again.\n";
            continue;
        }

        // Check all characters
        bool valid = true;
        for (char c : name)
            if (!isalpha(c) && !isspace(c)) valid = false;

        if (!valid)
        {
            cout << "Name can only contain letters and spaces! Try again.\n";
            continue;
        }

        // Optional: max length
        if (name.length() > 30)
        {
            cout << "Name too long! Maximum 30 characters.\n";
            continue;
        }

        return name; // finally valid
    }
}

class User
{
    string name;
public:
    // Constructor accepts a name
    User(const string& validName) : name(validName)
    {
        cout << "========================================\n";
        cout << "         Hello, " << name << "!\n";
        cout << "========================================\n";
    }

    string getName() const
    {
        return name;
    }

    ~User()
    {
        cout << "\n===========================================\n";
        cout << "Thanks for using the program, " << name << "!\n";
        cout << "Good luck with your university application!\n";
        cout << "===========================================\n\n";
    }
};
void aboutProgram()
{
    cout << "\n========================================\n";
    cout << "Myanmar University Recommendation System\n";
    cout << "========================================\n";
    cout << "This program helps students explore\n";
    cout << "which Myanmar universities they are\n";
    cout << "eligible for based on STEAMS marks.\n";
    cout << "\nFlow of the program:\n";
    cout << "1. Enter STEAMS and marks\n";
    cout << "2. Program calculates totals\n";
    cout << "3. Shows eligible universities\n";
    cout << "4. Lets you choose regions & fields\n";
    cout << "=====================================\n\n";
}

void showMarksSummary(int steamsChoice, int totalMarks, int engMathTotal,
                      int engChemBioTotal, int engMathPhysChemTotal)
{
    cout << "\n--------------------------------------------------\n";
    cout << "Your Total Marks Summary:\n";
    cout << "--------------------------------------------------\n";

    cout << left << setw(45) << "Total Marks (All Subjects):"
         << totalMarks << " / 600\n";

    cout << left << setw(45) << "English + Mathematics:"
         << engMathTotal << " / 200\n";

    if (steamsChoice == 1)
        cout << left << setw(45) << "English + Chemistry + Biology:"
             << engChemBioTotal << " / 300\n";
    else
        cout << left << setw(45) << "English + Chemistry + Biology:"
             << "N/A\n";

    if (steamsChoice == 1 || steamsChoice == 2)
        cout << left << setw(45) << "English + Mathematics + Physics + Chemistry:"
             << engMathPhysChemTotal << " / 400\n";
    else
        cout << left << setw(45) << "English + Mathematics + Physics + Chemistry:"
             << "N/A\n";

    cout << "--------------------------------------------------\n";
}

void showEligibilityRules()
{
    cout << "Eligibility Rules for Myanmar Universities:\n";
    cout << "--------------------------------------------------\n";
    cout << "- Medical Universities: Total >= 450 AND must have Biology\n";
    cout << "- Yangon University of Education:\n";
    cout << "    STEAMS1 -> Total >= 380\n";
    cout << "    STEAMS2 -> Total >= 360\n";
    cout << "    STEAMS3 -> Total >= 350\n";
    cout << "- Special Universities (YTU, MTU, etc.): Total >= 240 AND Eng+Math+Phys+Chem >= 300\n";
    cout << "- University of Information Technology (UIT): Total >= 450\n";
    cout << "- Computer Universities (UCSY & UCSM): Total >= 450 OR Eng+Math >= 140\n";
    cout << "- Naypyitaw State Polytechnic University (NSPU): Total >= 450 OR Eng+Math >= 140 OR Eng+Math+Phys+Chem >= 300\n";
    cout << "- Other Universities: Total >= 240\n";
    cout << "--------------------------------------------------\n";
}

char getYesNo(string prompt)
{
    string input;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        if (input.length() == 1)
        {
            char c = tolower(input[0]);
            if (c == 'y' || c == 'n')
            {
                return c;
            }
        }

        cout << "Invalid input! Please enter only 'Y' or 'N'.\n";
    }
}

int getInt(string prompt, int min, int max)
{
    string input;
    int value;

    while (true)
    {
        cout << prompt;
        getline(cin, input);


        if (input.empty())
        {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }


        size_t start = 0;
        if (input[0] == '-' || input[0] == '+') start = 1;
        bool valid = true;
        for (size_t i = start; i < input.length(); ++i)
        {
            if (!isdigit(input[i]))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }


        try
        {
            value = stoi(input);
        }
        catch (...)
        {
            cout << "Invalid input! Number is out of range.\n";
            continue;
        }

        if (value < min || value > max)
        {
            cout << "Please enter a number between " << min << " and " << max << ".\n";
            continue;
        }

        return value;
    }
}


struct University
{
    string name;
    string shortName;
    bool isMedical;
    bool isSpecial;    // YTU, MTU, UIT, etc.
    bool isComputer;
    bool isPU;
    string field;      // Medical, IT, Engineering, Economics, Education, Language,Engineering&IT, General, etc.
    int minTotal;      // Minimum total marks required
    int engMathMin;    // Minimum English + Math marks required (0 if not applicable)
    int engMathPhysChemMin; // Minimum Eng + Math + Phys + Chem marks required (0 if not applicable)
};
// Check eligibility based on university type and marks
bool isEligible(const University &u, int totalMarks, int engMathTotal, bool hasBiology, int engMathPhysChemTotal, int steamsChoice)
{
    // Medical Universities → Biology required
    if (u.isMedical) return (totalMarks >= u.minTotal && hasBiology);

    // Yangon University of Education special STEAMS cutoff
    if (u.shortName == "YUOE")
    {
        if (steamsChoice == 1) return totalMarks >= 380;
        if (steamsChoice == 2) return totalMarks >= 360;
        if (steamsChoice == 3) return totalMarks >= 350;
    }

    // Special Universities (YTU, MTU, UTYCC, UIT, etc.)
    if (u.isSpecial) return (totalMarks >= u.minTotal && engMathPhysChemTotal >= u.engMathPhysChemMin);

    // IT/Computer Universities
    if (u.isComputer) return (totalMarks >= u.minTotal || engMathTotal >= u.engMathMin);
    // NSPU
    if (u.isPU && u.shortName == "NSPU") return (totalMarks >= u.minTotal || engMathTotal >= u.engMathMin || engMathPhysChemTotal >= u.engMathPhysChemMin);

    if (u.isPU) return (totalMarks >= 320 || engMathPhysChemTotal >= u.engMathPhysChemMin );


    // General rule
    return totalMarks >= u.minTotal;
}

bool runOriginalFlow(const string &name)
{
    char repeat;

    // STEAMS options
    int steamsChoice;
    string STEAMS[3][6] =
    {
        {"Myanmar", "English", "Mathematics", "Chemistry", "Physics", "Biology"},
        {"Myanmar", "English", "Mathematics", "Chemistry", "Physics", "Economics"},
        {"Myanmar", "English", "Mathematics", "Geography", "History", "Economics"}
    };

    // Choose STEAMS
    steamsChoice = getInt(
                       "\nSelect your STEAMS:\n"
                       "1) Myanmar, English, Mathematics, Chemistry, Physics, Biology\n"
                       "2) Myanmar, English, Mathematics, Chemistry, Physics, Economics\n"
                       "3) Myanmar, English, Mathematics, Geography, History, Economics\n"
                       "Enter choice (1-3): ", 1, 3);
    clearScreen();
    bool hasBiology = (steamsChoice == 1);

    // Enter marks
    int marks[6];
    char confirm;
    do
    {
        cout << "\nEnter your marks (40-100) for each subject:\n";
        for (int i = 0; i < 6; i++)
        {
            while (true)
            {
                cout << left << setw(14) << STEAMS[steamsChoice - 1][i] << ": ";
                int temp;
                cin >> temp;

                if (cin.fail() || temp < 40 || temp > 100)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid mark! Please enter a number between 40 and 100.\n";
                    // loop continues → subject name will print again
                }
                else
                {
                    marks[i] = temp;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
                    break;
                }
            }
        }


        // Show entered marks
        cout << "\nYou entered:\n";
        for (int i = 0; i < 6; i++)
            cout << left << setw(14) << STEAMS[steamsChoice - 1][i] << ": " << setw(3) << marks[i] << endl;

        confirm = getYesNo("\nConfirm marks? (Y/N): ");
    }
    while (confirm != 'Y' && confirm != 'y');  // repeat until user confirms
    clearScreen();

    // Calculate totals
    int totalMarks = 0;
    for (int i = 0; i < 6; i++) totalMarks += marks[i];

    int engMathTotal = marks[1] + marks[2];  // English + Math
    int engChemBioTotal = (steamsChoice == 1) ? marks[1] + marks[3] + marks[5] : 0;
    int engMathPhysChemTotal = (steamsChoice == 1 || steamsChoice == 2) ? marks[1] + marks[2] + marks[3] + marks[4] : 0;


    // Now ask user if they want to see summary
    char schoice;
    schoice= getYesNo("\nDo you want to see your Total Marks Summary? (Y/N): ");

    if (schoice == 'Y' || schoice == 'y')
    {
        showMarksSummary(steamsChoice, totalMarks, engMathTotal, engChemBioTotal, engMathPhysChemTotal);
    }

    // Regions
    string locations[15] =
    {
        "Yangon Region", "Mandalay Region", "Ayeyarwady Region", "Bago Region",
        "Magway Region", "Sagaing Region", "Tanintharyi Region",
        "Kachin State", "Kayah State", "Kayin State", "Chin State",
        "Mon State", "Rakhine State", "Shan State", "Nay Pyi Taw",
    };

    // Universities (dataset preserved; names/shortNames cleaned where obvious)
    University universities[15][30] =
    {
        {
            // Yangon Region
            {"University of Yangon","YU",false,false,false,false,"General",240,0,0},
            {"Dagon University","DU",false,false,false,false,"General",240,0,0},
            {"East Yangon University","EYU",false,false,false,false,"General",240,0,0},
            {"West Yangon University","WYU",false,false,false,false,"General",240,0,0},
            {"Yankin Educational Degree College","YEDC",false,false,false,false,"Education",240,0,0},
            {"Hlegu Educational Degree College","HEDC",false,false,false,false,"Education",240,0,0},
            {"Yangon University of Education","YUOE",false,false,false,false,"Education",0,0,0}, // handled in isEligible
            {"Thingangyun Educational Degree College","TEDC",false,false,false,false,"Education",240,0,0},
            {"Yangon University of Economics (Hlaing)","YUEco-Hlaing",false,false,false,false,"Economics",380,0,0},
            {"Yangon University of Economics (Ywar Thar Gyi)","YUEco-YTG",false,false,false,false,"Economics",350,0,0},
            {"Yangon Technological University","YTU",false,true,false,false,"Engineering",240,0,300},
            {"University of Medicine-1 (Yangon)","UM1",true,false,false,false,"Medical",450,0,0},
            {"University of Medicine-2 (Yangon)","UM2",true,false,false,false,"Medical",450,0,0},
            {"University of Dental Medicine (Yangon)","UDMY",true,false,false,false,"Medical",450,0,0},
            {"University of Pharmacy (Yangon)","UOPY",true,false,false,false,"Medical",240,0,0},
            {"University of Medical Technology (Yangon)","UMTY",true,false,false,false,"Medical",240,0,0},
            {"University of Nursing (Yangon)","UNY",true,false,false,false,"Medical",240,0,0},
            {"University of Public Health (Yangon)","UPHY",true,false,false,false,"Medical",240,0,0},
            {"Yangon University of Foreign Languages","YUFL",false,false,false,false,"Language",380,0,0},
            {"University of Computer Studies (Yangon)","UCSY",false,false,true,false,"IT",450,140,0},
            {"University of Information Technology (Yangon)","UIT",false,true,false,false,"IT",450,0,0}, // mark as special
            {"West Yangon Technological University","WYTU",false,false,false,false,"Engineering",240,0,240},
            {"Technological University (Hmawbi)","HBTU",false,false,false,false,"Engineering",240,0,240},
            {"Technological University (Thanlyin)","TTU",false,false,false,false,"Engineering",240,0,240},
            {"Government Technical College (South Dagon)","GTCSD",false,false,false,false,"Engineering",240,0,0},
            {"Myanmar Maritime University","MMU",false,false,false,false,"Engineering",420,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Mandalay Region
            {"University of Mandalay","UMdl",false,false,false,false,"General",240,0,0},
            {"Meiktila University","MeU",false,false,false,false,"General",240,0,0},
            {"Yadanabon University","YdnU",false,false,false,false,"General",240,0,0},
            {"Kyaukse University","KSU",false,false,false,false,"General",240,0,0},
            {"Myingyan University","MyU",false,false,false,false,"General",240,0,0},
            {"Mandalar University","MdlarU",false,false,false,false,"General",240,0,0},
            {"Meiktila University of Economics","MUE",false,false,false,false,"Economics",350,0,0},
            {"Mandalay Educational Degree College","MEDC",false,false,false,false,"Education",240,0,0},
            {"Meiktila Educational Degree College","MeEDC",false,false,false,false,"Education",240,0,0},
            {"Mandalay University of Foreign Languages","MUFL",false,false,false,false,"Language",380,0,0},
            {"Technological University (Mandalay)","TUM",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Kyaukse)","KTU",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Meiktila)","TUMe",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Yamethin)","TUY",false,true,false,false,"Engineering",240,0,240},
            {"University of Technology (Yatanarpon Cyber City)","UTYCC",false,true,false,false,"Engineering",420,0,280},
            {"Mandalay Technological University","MTU",false,true,false,false,"Engineering",240,0,300},
            {"Myanmar Aerospace Engineering University","MAEU",false,true,false,false,"Engineering",240,0,280},
            {"University of Medicine (Mandalay)","UMM",true,false,false,false,"Medical",450,0,0},
            {"University of Dental Medicine (Mandalay)","UDMM",true,false,false,false,"Medical",450,0,0},
            {"University of Pharmacy (Mandalay)","UOPM",true,false,false,false,"Medical",240,0,0},
            {"University of Traditional Medicine","UTM",true,false,false,false,"Medical",240,0,0},
            {"University of Nursing (Mandalay)","UNM",true,false,false,false,"Medical",240,0,0},
            {"University of Medical Technology (Mandalay)","UMTM",true,false,false,false,"Medical",240,0,0},
            {"University of Computer Studies (Mandalay)","UCSM",false,false,true,false,"IT",450,140,0},
            {"Computer University (Mandalay)","CU-Mdy",false,false,false,false,"IT",320,0,0},
            {"University of Computer Studies (Meiktila)","UCS-Me",false,false,false,false,"IT",320,0,0},
            {"Government Technical College (Mandalay)","GTC-Mdy",false,false,false,false,"Engineering",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Ayeyarwady Region
            {"Pathein University","PU",false,false,false,false,"General",240,0,0},
            {"Hinthada University","HTU",false,false,false,false,"General",240,0,0},
            {"Maubin University","MbU",false,false,false,false,"General",240,0,0},
            {"University of Computer Studies (Pathein)","UCS-Pth",false,false,false,false,"IT",320,0,0},
            {"University of Computer Studies (Hinthada)","UCS-Hin",false,false,false,false,"IT",320,0,0},
            {"Technological University (Pathein)","TU-Pth",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Hinthada)","TU-Hin",false,true,false,false,"Engineering",240,0,240},
            {"Bogale Educational Degree College","BEDC",false,false,false,false,"Education",240,0,0},
            {"Pathein Educational Degree College","PEDC",false,false,false,false,"Education",240,0,0},
            {"Myaungmya Educational Degree College","MEDC-Ay",false,false,false,false,"Education",240,0,0},
            {"Polytechnic University (Maubin)","Poly-Mb",false,false,false,true,"Engineering&IT",320,0,240},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Bago Region
            {"Bago University","BU",false,false,false,false,"General",240,0,0},
            {"University of Computer Studies (Pyay)","UCS-Pyay",false,false,false,false,"IT",320,0,0},
            {"University of Computer Studies (Taungoo)","UCS-Tgo",false,false,false,false,"IT",320,0,0},
            {"Pyay Technological University","PTU",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Taungoo)","TU-Tgo",false,true,false,false,"Engineering",240,0,240},
            {"Pyay University","PyU",false,false,false,false,"General",240,0,0},
            {"Taungoo University","TgoU",false,false,false,false,"General",240,0,0},
            {"Pyay Educational Degree College","PEDC-Py",false,false,false,false,"Education",240,0,0},
            {"Taungoo Educational Degree College","TEDC-Tgo",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Magway Region
            {"Magway University","MgwU",false,false,false,false,"General",240,0,0},
            {"Pakokku University","PkU",false,false,false,false,"General",240,0,0},
            {"Yenangyaung University","YngU",false,false,false,false,"General",240,0,0},
            {"University of Medicine (Magway)","UMM",true,false,false,false,"Medical",450,0,0},
            {"University of Computer Studies (Magway)","UCS-Mgw",false,false,false,false,"IT",320,0,0},
            {"University of Computer Studies (Pakokku)","UCS-Pk",false,false,false,false,"IT",320,0,0},
            {"Technological University (Magway)","TU-Mgw",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Pakokku)","TU-Pk",false,true,false,false,"Engineering",240,0,240},
            {"Magway Educational Degree College","MEDC-Mgw",false,false,false,false,"Education",240,0,0},
            {"Pakokku Educational Degree College","PEDC-Pk",false,false,false,false,"Education",240,0,0},
            {"University of Community Heath(Magway)","UCHM",true,false,false,false,"Medical",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Sagaing Region
            {"Sagaing University","SgU",false,false,false,false,"General",240,0,0},
            {"Monywa University","MwU",false,false,false,false,"General",240,0,0},
            {"Kalay University","KyU",false,false,false,false,"General",240,0,0},
            {"Shwebo University","ShU",false,false,false,false,"General",240,0,0},
            {"Homalin Degree College","HDC",false,false,false,false,"General",240,0,0},
            {"University of Computer Studies (Monywa)","UCS-Mw",false,false,false,false,"IT",320,0,0},
            {"University of Computer Studies (Kalay)","UCS-Ky",false,false,false,false,"IT",320,0,0},
            {"Technological University (Kalay)","TU-Ky",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Monywa)","TU-Mw",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Sagaing)","TU-Sg",false,true,false,false,"Engineering",240,0,240},
            {"Sagaing Educational Degree College","SEDC",false,false,false,false,"Education",240,0,0},
            {"Monywa Educational Degree College","MEDC-Mw",false,false,false,false,"Education",240,0,0},
            {"Katha Educational Degree College","KEDC",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Tanintharyi Region
            {"Dawei University","DwU",false,false,false,false,"General",240,0,0},
            {"Myeik University","MykU",false,false,false,false,"General",240,0,0},
            {"Polytechnic University (Dawei)","Poly-Dw",false,false,false,true,"Engineering&IT",320,0,240},
            {"Polytechnic University (Myeik)","Poly-Myk",false,false,false,true,"Engineering&IT",320,0,240},
            {"Dawei Educational Degree College","DEDC",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Kachin State
            {"Myitkyina University","MykU",false,false,false,false,"General",240,0,0},
            {"Bamaw University","BmU",false,false,false,false,"General",240,0,0},
            {"Monyin University","MnU",false,false,false,false,"General",240,0,0},
            {"Technological University (Myitkyina)","TU-Myk",false,true,false,false,"Engineering",240,0,240},
            {"University of Computer Studies (Myitkyina)","UCS-Myk",false,false,false,false,"IT",320,0,0},
            {"Polytechnic University (Bamaw)","Poly-Bm",false,false,false,true,"Engineering&IT",320,0,240},
            {"Myitkyina Educational Degree College","MEDC-Myk",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Kayah State
            {"Technological University (Loikaw)","TU-Lkw",false,true,false,false,"Engineering",240,0,240},
            {"University of Computer Studies (Loikaw)","UCS-Lkw",false,false,false,false,"IT",320,0,0},
            {"Loikaw University","LkwU",false,false,false,false,"General",240,0,0},
            {"Loikaw Educational Degree College","LEDC",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Kayin State
            {"Hpa-An University","HAU",false,false,false,false,"General",240,0,0},
            {"University of Computer Studies (Hpa-An)","UCS-HA",false,false,false,false,"IT",320,0,0},
            {"Hpa-An Educational Degree College","HA-EDC",false,false,false,false,"Education",240,0,0},
            {"Technological University (Hpa-An)","TU-HA",false,true,false,false,"Engineering",240,0,240},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Chin State
            {"Hakha University","HkU",false,false,false,false,"General",240,0,0},
            {"Sittwe University","StwU",false,false,false,false,"General",240,0,0},
            {"Kalay University","KyU-Ch",false,false,false,false,"General",240,0,0},
            {"Pakokku University","PkU-Ch",false,false,false,false,"General",240,0,0},
            {"Technological University (Kalay)","TU-Ky-Ch",false,true,false,false,"Engineering",240,0,240},
            {"Monywa Educational Degree College (Female only)","MEDC-Mw-F",false,false,false,false,"Education",240,0,0},
            {"Hakha Educational Degree College (Male only)","HEDC-M",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Mon State
            {"Mawlamyine University","MlmU",false,false,false,false,"General",240,0,0},
            {"University of Computer Studies (Thaton)","UCS-Th",false,false,false,false,"IT",320,0,0},
            {"Technological University (Mawlamyine)","TU-Mlm",false,true,false,false,"Engineering",240,0,240},
            {"Mawlamyine Educational Degree College (Male only)","MEDC-Mlm-M",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Rakhine State
            {"Sittwe University","SUt",false,false,false,false,"General",240,0,0},
            {"Taunggoke University","TgkU",false,false,false,false,"General",240,0,0},
            {"Technological University (Sittwe)","TU-Stw",false,true,false,false,"Engineering",240,0,240},
            {"University of Computer Studies (Sittwe)","UCS-Stw",false,false,false,false,"IT",320,0,0},
            {"Kyaukphyu Educational Degree College (Male only)","KEDC-M",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Shan State


            {"Taunggyi University","TgiU",false,false,false,false,"General",240,0,0},
            {"Panglong University","PlgU",false,false,false,false,"General",240,0,0},
            {"Lashio University","LshU",false,false,false,false,"General",240,0,0},
            {"Kengtung University","KgtU",false,false,false,false,"General",240,0,0},
            {"University of Medicine (Taunggyi)","UMT",true,false,false,false,"Medical",450,0,0},
            {"Technological University (Taunggyi)","TU-Tgi",false,true,false,false,"Engineering",240,0,240},
            {"University of Computer Studies (Taunggyi)","UCST",false,false,false,false,"IT",320,0,0},
            {"Polytechnic University (Lashio)","Poly-Lsh",false,false,false,true,"Engineering&IT",320,0,240},
            {"Polytechnic University (Pinlaung)","Poly-Plg",false,false,false,true,"Engineering&IT",320,0,240},
            {"Polytechnic University (Kyaingtong)","Poly-Kytg",false,false,false,true,"Engineering&IT",320,0,240},
            {"Taunggyi Educational Degree College","TEDC-Tgi",false,false,false,false,"Education",240,0,0},
            {"Lashio Educational Degree College","LEDC-Lsh",false,false,false,false,"Education",240,0,0},
            {"Kengtung Educational Degree College","KEDC-Kgt",false,false,false,false,"Education",240,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },

        {
            // Nay Pyi Taw
            {"Naypyitaw State Academy","NPT-SA",false,false,false,false,"General",240,0,0},
            {"Naypyitaw State Polytechnic University","NSPU",false,false,false,true,"Engineering&IT",450,140,300},
            {"Technological University (Kyaukse)","KTU-NPT",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Mandalay)","TUM-NPT",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Meiktila)","TUMe-NPT",false,true,false,false,"Engineering",240,0,240},
            {"Technological University (Yamethin)","TUY-NPT",false,true,false,false,"Engineering",240,0,240},
            {"Computer University (Mandalay)","CU-Mdy-NPT",false,false,false,false,"IT",320,0,0},
            {"University of Computer Studies (Meiktila)","UCS-Me-NPT",false,false,false,false,"IT",320,0,0},
            {"", "", false, false, false, false, "", 0, 0, 0}
        },
    };


// Ask if user wants to choose Region or not
    char regionConfirm;
    int regionChoice = -1;  // -1 means no region selected
    regionConfirm = getYesNo("\nDo you want to choose a specific Region? (Y/N): ");

    if (regionConfirm == 'Y' || regionConfirm == 'y')
    {
        char repeatRegion;
        do
        {
            cout << "\nSelect your State/Region (1-15):\n";
            for (int i = 0; i < 15; i++)
                cout << right << setw(2) << i + 1 << ". " << locations[i] << endl;
            regionChoice = getInt("Enter choice: ", 1, 15);

            char showAllUni = getYesNo("\nDo you want to see ALL government universities in your chosen region? (Y/N): ");
            if(showAllUni == 'Y' || showAllUni == 'y')
            {
                cout << "\nAll Government Universities in " << locations[regionChoice-1] << ":\n";
                cout << left << setw(55) << "University Name"
                     << setw(20) << "Short Name"
                     << setw(10) << "MinTotal"
                     << setw(10) << "Eng+Math"
                     << setw(20) << "Eng+Math+Phys+Chem" << endl;
                cout << string(117, '-') << endl;
                int alluniNumber =1;
                for(int i = 0; !universities[regionChoice-1][i].name.empty(); i++)
                {
                    University u = universities[regionChoice-1][i];
                    cout << right <<  setw(2) << alluniNumber << left << setw(55) << ("." + u.name)
                         << setw(20) << ("(" + u.shortName + ")");

                    int minTotal = (u.shortName == "YUOE") ?
                                   (steamsChoice==1?380:(steamsChoice==2?360:350)) :
                                   u.minTotal;

                    cout << setw(10) << (minTotal > 0 ? to_string(minTotal) : "");
                    cout << setw(10) << (u.engMathMin > 0 ? to_string(u.engMathMin) : "");
                    cout << setw(10) << (u.engMathPhysChemMin > 0 ? to_string(u.engMathPhysChemMin) : "");

                    cout << endl;
                    alluniNumber++;
                }
            }
            // Show eligible universities based on marks
            char showEligibleUni = getYesNo("\nDo you want to see ELIGIBLE universities in your chosen region? (Y/N): ");

            if(showEligibleUni == 'Y' || showEligibleUni == 'y')
            {
                cout << "\nEligible Universities in " << locations[regionChoice-1] << " based on your marks:\n";
                cout << left << setw(55) << "University Name"
                     << setw(18) << "Short Name"
                     << setw(10) << "MinTotal"
                     << setw(10) << "Eng+Math"
                     << setw(20) << "Eng+Math+Phys+Chem" << endl;
                cout << string(120, '-') << endl;

                int count = 0;
                int reuniNumber =1 ;
                for(int i = 0; !universities[regionChoice-1][i].name.empty(); i++)
                {
                    University u = universities[regionChoice-1][i];
                    if(isEligible(u, totalMarks, engMathTotal, hasBiology, engMathPhysChemTotal, steamsChoice))
                    {
                        count++;
                        cout << right << setw(2) << reuniNumber << left << setw(55) << ("." + u.name)
                             << setw(20) << ("(" + u.shortName + ")");

                        int minTotal = (u.shortName == "YUOE") ?
                                       (steamsChoice==1?380:(steamsChoice==2?360:350)) :
                                       u.minTotal;
                        cout << setw(10) << minTotal;

                        if(u.engMathMin > 0)
                            cout << setw(10) << u.engMathMin;
                        else
                            cout << setw(10) << " ";

                        if(u.engMathPhysChemMin > 0)
                            cout << setw(10) << u.engMathPhysChemMin;
                        else
                            cout << setw(10) << " ";

                        cout << endl;
                        reuniNumber++;
                    }
                }
                if(count == 0) cout << "No eligible universities based on your marks.\n";
            }


            repeatRegion = getYesNo("\nDo you want to check another region? (Y/N): ");
        }
        while(repeatRegion == 'Y' || repeatRegion == 'y');
    }

    char repeatField;
    do
    {
        cout << "\nSelect your field of interest:\n";
        cout << "1. Medical\n2. IT/Computer\n3. Engineering\n4. Economics\n5. Education\n6. Language\n7. Engineering&IT\n8. General\n";

        string chosenField;
        int fieldChoice;

        do
        {
            fieldChoice = getInt("Enter choice: ",1,8);
            switch(fieldChoice)
            {
            case 1:
                chosenField="Medical";
                break;
            case 2:
                chosenField="IT";
                break;
            case 3:
                chosenField="Engineering";
                break;
            case 4:
                chosenField="Economics";
                break;
            case 5:
                chosenField="Education";
                break;
            case 6:
                chosenField="Language";
                break;
            case 7:
                chosenField="Engineering&IT";
                break;
            case 8:
                chosenField="General";
                break;
            default :
                cout << "Invalid choice" << endl;
                fieldChoice = 0;
            }
        }
        while (fieldChoice == 0);

        int count = 0;

        if (regionChoice != -1)
        {
            for(int i = 0; !universities[regionChoice-1][i].name.empty(); i++)
            {
                University u = universities[regionChoice-1][i];
                if (u.field != chosenField) continue;
                if (!isEligible(u, totalMarks, engMathTotal, hasBiology, engMathPhysChemTotal, steamsChoice))
                    continue;
                count++;
            }
        }
        else
        {
            for(int r = 0; r < 15; r++)
            {
                for(int i = 0; !universities[r][i].name.empty(); i++)
                {
                    University u = universities[r][i];
                    if (u.field != chosenField) continue;
                    if (!isEligible(u, totalMarks, engMathTotal, hasBiology, engMathPhysChemTotal, steamsChoice))
                        continue;
                    count++;
                }
            }
        }

//  Only print table if something was found
        if(count > 0)
        {
            cout << "\nEligible Universities for field: " << chosenField << "\n";
            cout << "-----------------------------------------------------------------------------------------------------------\n";
            cout << setw(50) << "University Name"
                 << setw(15) << "Short Name"
                 << setw(12) << "Min Total"
                 << setw(12) << "Eng+Math"
                 << setw(25) << "Eng+Math+Phys+Chem" << endl;
            cout << "-----------------------------------------------------------------------------------------------------------\n";


            //actually print results
            int uniNumber = 1;
            if (regionChoice != -1)
            {
                for(int i = 0; !universities[regionChoice-1][i].name.empty(); i++)
                {
                    University u = universities[regionChoice-1][i];
                    if (u.field != chosenField) continue;
                    if (!isEligible(u, totalMarks, engMathTotal, hasBiology, engMathPhysChemTotal, steamsChoice))
                        continue;

                    int minTotal = (u.shortName == "YUOE") ?
                                   (steamsChoice==1?380:(steamsChoice==2?360:350)) :
                                   u.minTotal;

                    cout << right << setw(2) << uniNumber << left << setw(50) << ("." + u.name)
                         << setw(18) << ("(" + u.shortName + ")")
                         << setw(10) << (minTotal > 0 ? to_string(minTotal) : "")
                         << setw(10) << (u.engMathMin > 0 ? to_string(u.engMathMin) : "")
                         << setw(10) << (u.engMathPhysChemMin > 0 ? to_string(u.engMathPhysChemMin) : "")
                         << endl;
                    uniNumber++;
                }
            }
            else
            {
                for(int r = 0; r < 15; r++)
                {
                    for(int i = 0; !universities[r][i].name.empty(); i++)
                    {
                        University u = universities[r][i];
                        if (u.field != chosenField) continue;
                        if (!isEligible(u, totalMarks, engMathTotal, hasBiology, engMathPhysChemTotal, steamsChoice))
                            continue;

                        int minTotal = (u.shortName == "YUOE") ?
                                       (steamsChoice==1?380:(steamsChoice==2?360:350)) :
                                       u.minTotal;

                        cout << right << setw(2) << uniNumber << left << setw(50) << ("." + u.name)
                             << setw(18) << ("(" + u.shortName + ")")
                             << setw(10) << (minTotal > 0 ? to_string(minTotal) : "")
                             << setw(10) << (u.engMathMin > 0 ? to_string(u.engMathMin) : "")
                             << setw(20) << (u.engMathPhysChemMin > 0 ? to_string(u.engMathPhysChemMin) : "")
                             << endl;
                        uniNumber++;
                    }
                }
            }
        }
        else
        {
            cout << "No eligible universities found for this field.\n";
        }
// Ask user if they want to choose another field
        repeatField =getYesNo("\nDo you want to check another field? (Y/N): ");
    }
    while(repeatField == 'Y' || repeatField == 'y');

    char backToMenu = getYesNo("\nDo you want to go back to the main menu? (Y/N): ");
    clearScreen();
    if (backToMenu == 'Y' || backToMenu == 'y')
    {
        // simply return to main(), which shows the menu again
        return true;

    }
    else
    {
        return false;
    }
    return true;
}

int main()
{
    int choice;
    bool exitProgram = false;

    while (!exitProgram)
    {
        cout << "\n====================================\n";
        cout << "       Matric2Uni Main Menu   \n";
        cout << "====================================\n";
        cout << "1. Start the Program\n";
        cout << "2. About the Program\n";
        cout << "3. See University Eligibility Rules\n";
        cout << "4. Exit\n";
        cout << "====================================\n";
        choice = getInt("Enter your choice: ", 1, 4);
        clearScreen();
        switch (choice)
        {
        case 1:
        {
            cout << "*=======================================================*\n";
            cout << "Welcome to our Myanmar Universities Recommendation System\n";
            cout << "*=======================================================*\n";
            string name = getValidatedName("Enter your name: ");
            User currentUser(name);
            exitProgram = !runOriginalFlow(name);
            break;
        }
        case 2:
        {
            aboutProgram();
            char back = getYesNo("\nDo you want to go back to the main menu? (Y/N): ");
            clearScreen();
            if (back == 'n')
            {
                cout << "Exiting program. Goodbye!\n";
                exitProgram = true;
            }
            break;
        }
        case 3:
        {
            showEligibilityRules();
            char back = getYesNo("\nDo you want to go back to the main menu? (Y/N): ");
            clearScreen();
            if (back == 'n')
            {
                cout << "Exiting program. Goodbye!\n";
                exitProgram = true;
            }
            break;
        }

        case 4:
            cout << "Exiting program. Goodbye!\n";
            exitProgram = true;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
    //written by Yé Yint Hein from Group-1 C++ project//
}


