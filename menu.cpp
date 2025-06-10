#include <iostream>
#include <vector>
#include <string>
#include <windows.h> // Ho tro cac ham console cua Windows
#include <conio.h>   // Ho tro _kbhit() va _getch()

using namespace std;

// Ham tro giup de dat mau cho chu
void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Ham reset ve mau mac dinh
void ResetColor() {
    SetColor(7); // Mac dinh trang tren nen den
}

// Ham tro giup de dat vi tri con tro
void Gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hienThiMenuChinh() {
    vector<string> menuItems = {
        "Tao tai khoan moi",
        "Tim tai khoan",
        "Xoa tai khoan",
        "Cap nhat thong tin tai khoan",
        "Rut tien",
        "Gui tien",
        "Chuyen khoan",
        "Kiem tra so du",
        "Hien thi danh sach tai khoan",
        "Thoat chuong trinh"
    };

    int currentSelection = 0; // Chi muc cua lua chon hien tai (bat dau tu 0)
    int menuStartY = 5;       // Toa do Y bat dau cua cac muc menu tren console

    while (true) {
        system("cls"); // Xoa console de ve lai menu

        SetColor(11); // Mau xanh nhat cho tieu de
        cout << "\n  ===== HE THONG QUAN LY TAI KHOAN =====\n";
        ResetColor();

        // Hien thi cac muc menu
        for (int i = 0; i < menuItems.size(); ++i) {
            Gotoxy(0, menuStartY + i); // Di chuyen con tro den dau dong cua tung muc
            if (i == currentSelection) {
                SetColor(10); // Mau xanh la cho muc duoc chon
                cout << ">> " << menuItems[i] << " <<";
            } else {
                SetColor(14); // Mau vang cho cac muc khong duoc chon
                cout << "   " << menuItems[i] << "   ";
            }
            ResetColor();
        }

        // Cho mot lan nhan phim
        int key = _getch();

        // Xu ly cac phim mui ten va Enter
        if (key == 0 || key == 224) { // Phim dac biet (phim mui ten)
            key = _getch(); // Lay ma phim mo rong
            switch (key) {
                case 72: // Mui ten len (Up arrow)
                    currentSelection = (currentSelection - 1 + menuItems.size()) % menuItems.size();
                    break;
                case 80: // Mui ten xuong (Down arrow)
                    currentSelection = (currentSelection + 1) % menuItems.size();
                    break;
            }
        } else if (key == 13) { // Phim Enter
            system("cls"); // Xoa man hinh truoc khi thuc hien hanh dong

            // In ra lua chon da chon (day la noi ban se goi ham tuong ung)
            SetColor(9); // Mau xanh duong
            cout << "\nBan da chon: " << menuItems[currentSelection] << endl;
            ResetColor();

            // Kiem tra neu la tuy chon "Thoat chuong trinh"
            if (currentSelection == menuItems.size() - 1) {
                SetColor(10);
                cout << "\nCam on ban da su dung chuong trinh!\n";
                ResetColor();
                return; // Thoat khoi ham va ket thuc chuong trinh
            }

            cout << "\nNhan phim bat ky de quay lai menu...";
            _getch(); // Cho mot phim nhan de nguoi dung xem thong bao
        }
    }
}

int main() {
    SetConsoleTitle(TEXT("Menu Quan Ly Tai Khoan")); // Dat tieu de cho cua so console
    hienThiMenuChinh(); // Goi ham hien thi menu chinh
    return 0;
}
