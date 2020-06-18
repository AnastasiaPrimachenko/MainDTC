#include <iostream>
using namespace std;

int main()
{
    int set = 0;
    cout << "Choose a dataset:" << endl;
    cout << "1 - Breast Cancer Wisconsin Dataset" << endl;
    cout << "2 - Mushroom Dataset" << endl;
    cout << "3 - Ionosphere Dataset" << endl;
    cout << "4 - Divorce predictors Dataset" << endl;
    cout << "5 - Iris dataset" << endl;
    cout << "6 - Congressional Voting Records Dataset" << endl;
    cin >> set;
    switch (set) {
    case 1:
        system("C:\\Users\\User\\Documents\\MVS\\курсовая\\Main\\Debug\\MainCW.exe");
        break;
    case 2:
        system("C:\\Users\\User\\Documents\\MVS\\курсовая\\Main\\Debug\\MainMushroom.exe");
        break;
    case 3:
        system("C:\\Users\\User\\Documents\\MVS\\курсовая\\Main\\Debug\\MainIonosphere.exe");
        break;
    case 4:
        system("C:\\Users\\User\\Documents\\MVS\\курсовая\\Main\\Debug\\MainDivorce.exe");
        break;
    case 5:
        system("C:\\Users\\User\\Documents\\MVS\\курсовая\\Main\\Debug\\MainIris.exe");
        break;
    case 6:
        system("C:\\Users\\User\\Documents\\MVS\\курсовая\\Main\\Debug\\MainVote.exe");
        break;
    default:
        cout << "Choose a number from 1 to 6" << endl;
        break;
    }
}
