#include <iostream>
#include <iomanip>  // Để định dạng output (setw, fixed, setprecision)
#include <fstream>  // Cho các thao tác với file (ifstream, ofstream)
#include <limits>   // Cho numeric_limits (dùng để xóa buffer input)
#include <ctime>    // Cho các hàm liên quan đến thời gian (time_t, localtime, strftime, time)
#include <cctype>   // Cho các hàm kiểm tra ký tự (isdigit)
#include <cstdlib>  // Cho malloc và free, exit
#include <cstring>  // Cho strlen, strcmp, strcpy_s, strncpy_s
#include <cstdio>   // Cho _snprintf_s (hoặc snprintf)

#include <windows.h> // Để sử dụng HANDLE, COORD, SetConsoleCursorPosition, SetConsoleTextAttribute, Sleep, SetConsoleOutputCP, SetConsoleTitle
#include <conio.h>   // Để sử dụng _getch()

using namespace std;

#define MAX_SO_TAI_KHOAN 15
#define MAX_TEN_KH 50
#define MAX_EMAIL 50
#define MAX_SO_DIEN_THOAI 15
#define MAX_CAN_CUOC_CONG_DAN 20
#define MAX_LOAI_GD 20
#define MAX_GHI_CHU 50
#define MAX_THOI_GIAN_STR 80
string sddd(long long beo)
{
    string sdu = "";
    string sdu1 = "";
    int chec = 0;
    while(beo != 0){
        sdu = char(beo%10+'0')+ sdu;
        chec++;
        if (chec % 3 == 0) sdu = '.' + sdu, chec = 0;
        beo /= 10;
    }
    if (sdu[0] == '.'){
        for (int poo = 1; poo < sdu.size(); ++poo)
            sdu1 += sdu[poo];
    }
    else sdu1 = sdu;
    return sdu1;
}
void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void ResetColor() {
    SetColor(7);
}

void Gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct KhachHang {
    char soTaiKhoan[MAX_SO_TAI_KHOAN];
    char ten[MAX_TEN_KH];
    char email[MAX_EMAIL];
    char soDienThoai[MAX_SO_DIEN_THOAI];
    char canCuocCongDan[MAX_CAN_CUOC_CONG_DAN];
    long long soDu;
};

struct GiaoDich {
    time_t thoiGian;
    char loaiGiaoDich[MAX_LOAI_GD];
    char soTaiKhoanNguon[MAX_SO_TAI_KHOAN];
    char soTaiKhoanDich[MAX_SO_TAI_KHOAN];
    long long soTien;
    char ghiChu[MAX_GHI_CHU];
};
struct KhachHangNode {
    KhachHang data;
    struct KhachHangNode* next;
    struct KhachHangNode* prev;
};

struct GiaoDichNode {
    GiaoDich data;
    struct GiaoDichNode* next;
    struct GiaoDichNode* prev;
};

class KhachHangList {
public:
    struct KhachHangNode* head;
    struct KhachHangNode* tail;

    KhachHangList() : head(nullptr), tail(nullptr) {}

    ~KhachHangList() {
        struct KhachHangNode* current = head;
        while (current != nullptr) {
            struct KhachHangNode* nextNode = current->next;
            free(current);
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
    }


    KhachHangList(const KhachHangList& other) : head(nullptr), tail(nullptr) {
        struct KhachHangNode* current_other = other.head;
        while (current_other != nullptr) {
            add(current_other->data);
            current_other = current_other->next;
        }
    }

    KhachHangList& operator=(const KhachHangList& other) {
        if (this == &other) {
            return *this;
        }

        struct KhachHangNode* current = head;
        while (current != nullptr) {
            struct KhachHangNode* nextNode = current->next;
            free(current);
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;

        struct KhachHangNode* current_other = other.head;
        while (current_other != nullptr) {
            add(current_other->data);
            current_other = current_other->next;
        }
        return *this;
    }

    void add(KhachHang kh) {
        struct KhachHangNode* newNode = (struct KhachHangNode*) malloc(sizeof(struct KhachHangNode));
        if (newNode == nullptr) {
            SetColor(12); cout << "Loi: Khong du bo nho de tao node KhachHang.\n"; ResetColor();
            exit(EXIT_FAILURE);
        }
        newNode->data = kh;
        newNode->next = nullptr;
        newNode->prev = nullptr;

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }


    bool isEmpty() const {
        return head == nullptr;
    }

    bool remove(const char* soTaiKhoan) {
        struct KhachHangNode* nodeToDelete = nullptr;
        for (struct KhachHangNode* current = head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, soTaiKhoan) == 0) {
                nodeToDelete = current;
                break;
            }
        }

        if (nodeToDelete == nullptr) {
            return false;
        }

        if (nodeToDelete->prev != nullptr) {
            nodeToDelete->prev->next = nodeToDelete->next;
        } else {
            head = nodeToDelete->next;
        }

        if (nodeToDelete->next != nullptr) {
            nodeToDelete->next->prev = nodeToDelete->prev;
        } else {
            tail = nodeToDelete->prev;
        }

        free(nodeToDelete);
        return true;
    }
};

class GiaoDichList {
public:
    struct GiaoDichNode* head;
    struct GiaoDichNode* tail;

    GiaoDichList() : head(nullptr), tail(nullptr) {}


    ~GiaoDichList() {
        struct GiaoDichNode* current = head;
        while (current != nullptr) {
            struct GiaoDichNode* nextNode = current->next;
            free(current);
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
    }

    GiaoDichList(const GiaoDichList& other) : head(nullptr), tail(nullptr) {
        struct GiaoDichNode* current_other = other.head;
        while (current_other != nullptr) {
            add(current_other->data);
            current_other = current_other->next;
        }
    }

    GiaoDichList& operator=(const GiaoDichList& other) {
        if (this == &other) {
            return *this;
        }

        struct GiaoDichNode* current = head;
        while (current != nullptr) {
            struct GiaoDichNode* nextNode = current->next;
            free(current);
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;

        struct GiaoDichNode* current_other = other.head;
        while (current_other != nullptr) {
            add(current_other->data);
            current_other = current_other->next;
        }
        return *this;
    }

    void add(GiaoDich gd) {
        struct GiaoDichNode* newNode = (struct GiaoDichNode*) malloc(sizeof(struct GiaoDichNode));
        if (newNode == nullptr) {
            SetColor(12); cout << "Loi: Khong du bo nho de tao node GiaoDich.\n"; ResetColor();
            exit(EXIT_FAILURE);
        }
        newNode->data = gd;
        newNode->next = nullptr;
        newNode->prev = nullptr;

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};

class QuanLyNganHang {
private:
    KhachHangList danhSachKH;
    GiaoDichList danhSachGD;

    const char* TEN_FILE_KH = "khachhang.txt";
    const char* TEN_FILE_GD = "giaodich.log";

    bool isSoTaiKhoanUnique(const char* stk) const {
        struct KhachHangNode* current = danhSachKH.head;
        while (current != nullptr) {
            if (strcmp(current->data.soTaiKhoan, stk) == 0) {
                return false;
            }
            current = current->next;
        }
        return true;
    }

    void generateUniqueSoTaiKhoan(char* buffer) {
        long long randomNumber;
        char temp_stk[MAX_SO_TAI_KHOAN];
        do {
            randomNumber = 1000000000LL + (rand() % 9000000000LL);
            _snprintf_s(temp_stk, MAX_SO_TAI_KHOAN, _TRUNCATE, "%lld", randomNumber);
        } while (!isSoTaiKhoanUnique(temp_stk));
        strcpy_s(buffer, MAX_SO_TAI_KHOAN, temp_stk);
    }

public:
    QuanLyNganHang() {
        docDuLieuKhachHangTuFile();
        docDuLieuGiaoDichTuFile();
    }

    ~QuanLyNganHang() {
        ghiDuLieuKhachHangVaoFile();
        ghiDuLieuGiaoDichVaoFile();
    }

    void ghiDuLieuKhachHangVaoFile() const {
        ofstream outFile(TEN_FILE_KH);
        if (!outFile.is_open()) {
            SetColor(12); cout << "Loi: Khong the mo file khachhang.txt de ghi.\n"; ResetColor();
            return;
        }
        struct KhachHangNode* current = danhSachKH.head;
        while (current != nullptr) {

            outFile << current->data.soTaiKhoan << "|"
                    << current->data.ten << "|"
                    << current->data.email << "|"
                    << current->data.soDienThoai << "|"
                    << current->data.canCuocCongDan << "|"
                    << (long long)(current->data.soDu) << "\n";
            current = current->next;
        }
        outFile.close();
    }

    void docDuLieuKhachHangTuFile() {
        ifstream inFile(TEN_FILE_KH);
        if (!inFile.is_open()) return;

        danhSachKH = KhachHangList();

        char line[512];
        while (inFile.getline(line, sizeof(line))) {
            KhachHang kh;
            char* context = nullptr;
            char* token = strtok_s(line, "|", &context);

            if (token) { strcpy_s(kh.soTaiKhoan, MAX_SO_TAI_KHOAN, token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { strcpy_s(kh.ten, MAX_TEN_KH, token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { strcpy_s(kh.email, MAX_EMAIL, token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { strcpy_s(kh.soDienThoai, MAX_SO_DIEN_THOAI, token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { strcpy_s(kh.canCuocCongDan, MAX_CAN_CUOC_CONG_DAN, token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { kh.soDu = atof(token); } else { continue; }

            danhSachKH.add(kh);
        }
        inFile.close();
    }

    void ghiDuLieuGiaoDichVaoFile() const {
        ofstream outFile(TEN_FILE_GD);
        if (!outFile.is_open()) {
            SetColor(12); cout << "Loi: Khong the mo file giaodich.log de ghi.\n"; ResetColor();
            return;
        }
        struct GiaoDichNode* current = danhSachGD.head;
        while (current != nullptr) {
            outFile << (long long)current->data.thoiGian << "|"
                    << current->data.loaiGiaoDich << "|"
                    << current->data.soTaiKhoanNguon << "|"
                    << current->data.soTaiKhoanDich << "|" << (long long)(current->data.soTien) << "|"
                    << current->data.ghiChu << "\n";
            current = current->next;
        }
        outFile.close();
    }

    void docDuLieuGiaoDichTuFile() {
        ifstream inFile(TEN_FILE_GD);
        if (!inFile.is_open()) return;

        danhSachGD = GiaoDichList();

        char line[512];
        while (inFile.getline(line, sizeof(line))) {
            GiaoDich gd;
            char* context = nullptr;
            char* token = strtok_s(line, "|", &context);

            if (token) { gd.thoiGian = (time_t)atoll(token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { strcpy_s(gd.loaiGiaoDich, MAX_LOAI_GD, token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { strcpy_s(gd.soTaiKhoanNguon, MAX_SO_TAI_KHOAN, token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { strcpy_s(gd.soTaiKhoanDich, MAX_SO_TAI_KHOAN, token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { gd.soTien = atof(token); } else { continue; }
            token = strtok_s(nullptr, "|", &context);
            if (token) { strcpy_s(gd.ghiChu, MAX_GHI_CHU, token); } else { continue; }

            danhSachGD.add(gd);
        }
        inFile.close();
    }

    void themKhachHang() {
        KhachHang newKH;
        SetColor(11); cout << "\n  === TAO TAI KHOAN MOI ===\n"; ResetColor();

        generateUniqueSoTaiKhoan(newKH.soTaiKhoan);
        SetColor(10); cout << "So tai khoan moi duoc tao: " << newKH.soTaiKhoan << "\n"; ResetColor();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        SetColor(10); cout << "Nhap ho va ten: "; ResetColor();
        cin.getline(newKH.ten, MAX_TEN_KH);

        SetColor(10); cout << "Nhap Email: "; ResetColor();
        cin.getline(newKH.email, MAX_EMAIL);

        do {
            SetColor(10); cout << "Nhap So Dien Thoai (10 so, bat dau tu 0): "; ResetColor();
            cin.getline(newKH.soDienThoai, MAX_SO_DIEN_THOAI);

            bool is_valid = true;
            if (strlen(newKH.soDienThoai) == 10 && newKH.soDienThoai[0] == '0') {
                for (int i = 0; i < strlen(newKH.soDienThoai); ++i) {
                    if (!isdigit(newKH.soDienThoai[i])) {
                        is_valid = false;
                        break;
                    }
                }
            } else {
                is_valid = false;
            }

            if (is_valid) {
                break;
            }
            SetColor(12); cout << "So dien thoai khong hop le. Vui long nhap 10 chu so va bat dau bang '0'.\n"; ResetColor();
        } while (true);

        do {
            SetColor(10); cout << "Nhap Can Cuoc Cong Dan (CCCD - 12 so): "; ResetColor();
            cin.getline(newKH.canCuocCongDan, MAX_CAN_CUOC_CONG_DAN);

            bool is_valid = true;
            if (strlen(newKH.canCuocCongDan) == 12) {
                for (int i = 0; i < strlen(newKH.canCuocCongDan); ++i) {
                    if (!isdigit(newKH.canCuocCongDan[i])) {
                        is_valid = false;
                        break;
                    }
                }
            } else {
                is_valid = false;
            }

            if (is_valid) {
                break;
            }
            SetColor(12); cout << "So Can Cuoc Cong Dan khong hop le. Vui long nhap 12 chu so.\n"; ResetColor();
        } while (true);

        SetColor(10); cout << "Nhap so du ban dau: "; ResetColor();
        while (!(cin >> newKH.soDu) || newKH.soDu < 0) {
            SetColor(12); cout << "Du lieu khong hop le. Vui long nhap so du la mot so khong am.\n"; ResetColor();
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            SetColor(10); cout << "Nhap so du ban dau: "; ResetColor();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        danhSachKH.add(newKH);
        SetColor(10); cout << "\nTao tai khoan moi thanh cong!\n"; ResetColor();
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void timKhachHang() const {
        char stk[MAX_SO_TAI_KHOAN];
        SetColor(11); cout << "\n  === TIM TAI KHOAN ===\n"; ResetColor();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        SetColor(10); cout << "Nhap so tai khoan can tim: "; ResetColor();
        cin.getline(stk, MAX_SO_TAI_KHOAN);

        struct KhachHangNode* foundNode = nullptr;
        for (struct KhachHangNode* current = danhSachKH.head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, stk) == 0) {
                foundNode = current;
                break;
            }
        }

        if (foundNode != nullptr) {
            SetColor(14);
            cout << "\nThong tin khach hang:\n";
            cout << "----------------------------------------\n";
            ResetColor();
            cout << "So tai khoan        : " << foundNode->data.soTaiKhoan << "\n";
            cout << "Ho ten              : " << foundNode->data.ten << "\n";
            cout << "Email               : " << foundNode->data.email << "\n";
            cout << "So dien thoai       : " << foundNode->data.soDienThoai << "\n";
            cout << "CCCD                : " << foundNode->data.canCuocCongDan << "\n";
            cout << "So du               : " << foundNode->data.soDu << " VND" << "\n";
            SetColor(14);
            cout << "----------------------------------------\n";
            ResetColor();
        } else {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << "\n"; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void xoaKhachHang() {
        char stk[MAX_SO_TAI_KHOAN];
        SetColor(11); cout << "\n  === XOA TAI KHOAN ===\n"; ResetColor();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        SetColor(10); cout << "Nhap so tai khoan can xoa: "; ResetColor();
        cin.getline(stk, MAX_SO_TAI_KHOAN);

        struct KhachHangNode* khToDelete = nullptr;
        for (struct KhachHangNode* current = danhSachKH.head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, stk) == 0) {
                khToDelete = current;
                break;
            }
        }

        if (khToDelete != nullptr) {
            SetColor(10); cout << "\nDa xoa tai khoan cua khach hang " << khToDelete->data.ten << " (" << khToDelete->data.soTaiKhoan << ").\n"; ResetColor();
            danhSachKH.remove(stk);
        } else {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << "\n"; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void capNhatThongTin() {
        char stk[MAX_SO_TAI_KHOAN];
        SetColor(11); cout << "\n  === CAP NHAT THONG TIN TAI KHOAN ===\n"; ResetColor();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        SetColor(10); cout << "Nhap so tai khoan can cap nhat: "; ResetColor();
        cin.getline(stk, MAX_SO_TAI_KHOAN);

        struct KhachHangNode* khNode = nullptr;
        for (struct KhachHangNode* current = danhSachKH.head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, stk) == 0) {
                khNode = current;
                break;
            }
        }

        if (khNode != nullptr) {
            SetColor(14); cout << "\nTim thay tai khoan: " << khNode->data.ten << ". Nhap thong tin moi (de trong de giu lai thong tin cu).\n"; ResetColor();

            char newTen[MAX_TEN_KH];
            char newEmail[MAX_EMAIL];
            char newSDT[MAX_SO_DIEN_THOAI];
            char newCCCD[MAX_CAN_CUOC_CONG_DAN];

            SetColor(10); cout << "Ten moi [" << khNode->data.ten << "]: "; ResetColor();
            cin.getline(newTen, MAX_TEN_KH);
            if (strlen(newTen) > 0) {
                strcpy_s(khNode->data.ten, MAX_TEN_KH, newTen);
            }

            SetColor(10); cout << "Email moi [" << khNode->data.email << "]: "; ResetColor();
            cin.getline(newEmail, MAX_EMAIL);
            if (strlen(newEmail) > 0) {
                strcpy_s(khNode->data.email, MAX_EMAIL, newEmail);
            }

            do {
                SetColor(10); cout << "So dien thoai moi [" << khNode->data.soDienThoai << "] (10 so, bat dau tu 0, de trong de giu cu): "; ResetColor();
                cin.getline(newSDT, MAX_SO_DIEN_THOAI);

                if (strlen(newSDT) == 0) {
                    break;
                }

                bool is_valid = true;
                if (strlen(newSDT) == 10 && newSDT[0] == '0') {
                    for (int i = 0; i < strlen(newSDT); ++i) {
                        if (!isdigit(newSDT[i])) {
                            is_valid = false;
                            break;
                        }
                    }
                } else {
                    is_valid = false;
                }

                if (is_valid) {
                    strcpy_s(khNode->data.soDienThoai, MAX_SO_DIEN_THOAI, newSDT);
                    break;
                }
                SetColor(12); cout << "So dien thoai moi khong hop le. Vui long nhap 10 chu so va bat dau bang '0', hoac de trong.\n"; ResetColor();
            } while (true);

            do {
                SetColor(10); cout << "CCCD moi [" << khNode->data.canCuocCongDan << "] (12 so, de trong de giu cu): "; ResetColor();
                cin.getline(newCCCD, MAX_CAN_CUOC_CONG_DAN);

                if (strlen(newCCCD) == 0) {
                    break;
                }

                bool is_valid = true;
                if (strlen(newCCCD) == 12) {
                    for (int i = 0; i < strlen(newCCCD); ++i) {
                        if (!isdigit(newCCCD[i])) {
                            is_valid = false;
                            break;
                        }
                    }
                } else {
                    is_valid = false;
                }

                if (is_valid) {
                    strcpy_s(khNode->data.canCuocCongDan, MAX_CAN_CUOC_CONG_DAN, newCCCD);
                    break;
                }
                SetColor(12); cout << "So Can Cuoc Cong Dan moi khong hop le. Vui long nhap 12 chu so, hoac de trong.\n"; ResetColor();
            } while (true);

            SetColor(10); cout << "\nCap nhat thong tin thanh cong!\n"; ResetColor();
        } else {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << "\n"; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void rutTien() {
        char stk[MAX_SO_TAI_KHOAN];
        long long soTien;
        SetColor(11); cout << "\n  === RUT TIEN TU TAI KHOAN ===\n"; ResetColor();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        SetColor(10); cout << "Nhap so tai khoan: "; ResetColor();
        cin.getline(stk, MAX_SO_TAI_KHOAN);

        struct KhachHangNode* khNode = nullptr;
        for (struct KhachHangNode* current = danhSachKH.head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, stk) == 0) {
                khNode = current;
                break;
            }
        }

        if (khNode != nullptr) {
            SetColor(10); cout << "Nhap so tien can rut: "; ResetColor();
            while (!(cin >> soTien) || soTien <= 0) {
                SetColor(12); cout << "So tien khong hop le. Vui long nhap mot so duong.\n"; ResetColor();
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                SetColor(10); cout << "Nhap so tien can rut: "; ResetColor();
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (soTien <= khNode->data.soDu) {
                khNode->data.soDu -= soTien;
                GiaoDich newGD;
                newGD.thoiGian = time(0);
                strcpy_s(newGD.loaiGiaoDich, MAX_LOAI_GD, "Rut");
                strcpy_s(newGD.soTaiKhoanNguon, MAX_SO_TAI_KHOAN, stk);
                strcpy_s(newGD.soTaiKhoanDich, MAX_SO_TAI_KHOAN, "---");
                newGD.soTien = soTien;
                strcpy_s(newGD.ghiChu, MAX_GHI_CHU, "Rut tien mat");
                danhSachGD.add(newGD);

                SetColor(10); cout << "\nRut tien thanh cong! So du moi: " << sddd(khNode->data.soDu) << "\n"; ResetColor();
            } else {

                SetColor(12); cout << "\nSo du khong du! So du hien tai: " << sddd(khNode->data.soDu) << "\n"; ResetColor();
            }
        } else {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << "\n"; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void guiTien() {
        char stk[MAX_SO_TAI_KHOAN];
        long long soTien;
        SetColor(11); cout << "\n  === GUI TIEN VAO TAI KHOAN ===\n"; ResetColor();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        SetColor(10); cout << "Nhap so tai khoan: "; ResetColor();
        cin.getline(stk, MAX_SO_TAI_KHOAN);

        struct KhachHangNode* khNode = nullptr;
        for (struct KhachHangNode* current = danhSachKH.head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, stk) == 0) {
                khNode = current;
                break;
            }
        }

        if (khNode != nullptr) {
            SetColor(10); cout << "Nhap so tien can gui: "; ResetColor();
            while (!(cin >> soTien) || soTien <= 0) {
                SetColor(12); cout << "So tien khong hop le. Vui long nhap mot so duong.\n"; ResetColor();
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                SetColor(10); cout << "Nhap so tien can gui: "; ResetColor();
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            khNode->data.soDu += soTien;
            GiaoDich newGD;
            newGD.thoiGian = time(0);
            strcpy_s(newGD.loaiGiaoDich, MAX_LOAI_GD, "Gui");
            strcpy_s(newGD.soTaiKhoanNguon, MAX_SO_TAI_KHOAN, stk);
            strcpy_s(newGD.soTaiKhoanDich, MAX_SO_TAI_KHOAN, "---");
            newGD.soTien = soTien;
            strcpy_s(newGD.ghiChu, MAX_GHI_CHU, "Gui tien mat");
            danhSachGD.add(newGD);

            SetColor(10); cout << "\nGui tien thanh cong! So du moi: " << sddd(khNode->data.soDu) << "\n"; ResetColor();
        } else {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << "\n"; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void chuyenKhoan() {
        char stkNguon[MAX_SO_TAI_KHOAN], stkDich[MAX_SO_TAI_KHOAN];
        long long soTien;
        SetColor(11); cout << "\n  === CHUYEN KHOAN GIUA CAC TAI KHOAN ===\n"; ResetColor();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        SetColor(10); cout << "Nhap so tai khoan nguon: "; ResetColor();
        cin.getline(stkNguon, MAX_SO_TAI_KHOAN);

        SetColor(10); cout << "Nhap so tai khoan dich: "; ResetColor();
        cin.getline(stkDich, MAX_SO_TAI_KHOAN);

        if (strcmp(stkNguon, stkDich) == 0) {
            SetColor(12); cout << "\nKhong the chuyen tien cho chinh minh!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }

        struct KhachHangNode* nguonNode = nullptr;
        for (struct KhachHangNode* current = danhSachKH.head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, stkNguon) == 0) {
                nguonNode = current;
                break;
            }
        }

        struct KhachHangNode* dichNode = nullptr;
        for (struct KhachHangNode* current = danhSachKH.head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, stkDich) == 0) {
                dichNode = current;
                break;
            }
        }

        if (nguonNode == nullptr) {
            SetColor(12); cout << "\nKhong tim thay so tai khoan nguon!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }
        if (dichNode == nullptr) {
            SetColor(12); cout << "\nKhong tim thay so tai khoan dich!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }

        SetColor(10); cout << "Nhap so tien can chuyen: "; ResetColor();
        while (!(cin >> soTien) || soTien <= 0) {
            SetColor(12); cout << "So tien khong hop le. Vui long nhap mot so duong.\n"; ResetColor();
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            SetColor(10); cout << "Nhap so tien can chuyen: "; ResetColor();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (soTien <= nguonNode->data.soDu) {
            nguonNode->data.soDu -= soTien;
            dichNode->data.soDu += soTien;

            GiaoDich newGD;
            newGD.thoiGian = time(0);
            strcpy_s(newGD.loaiGiaoDich, MAX_LOAI_GD, "ChuyenKhoan");
            strcpy_s(newGD.soTaiKhoanNguon, MAX_SO_TAI_KHOAN, stkNguon);
            strcpy_s(newGD.soTaiKhoanDich, MAX_SO_TAI_KHOAN, stkDich);

            char temp_ghiChu[MAX_GHI_CHU];
            _snprintf_s(temp_ghiChu, MAX_GHI_CHU, _TRUNCATE, "%s -> %s", nguonNode->data.ten, dichNode->data.ten);
            strcpy_s(newGD.ghiChu, MAX_GHI_CHU, temp_ghiChu);

            newGD.soTien = soTien;
            danhSachGD.add(newGD);

            SetColor(10);
            cout << "\nChuyen tien thanh cong!\n";
            cout << "So du tai khoan nguon (" << nguonNode->data.soTaiKhoan << "): " << sddd(nguonNode->data.soDu) << "\n";
            cout << "So du tai khoan dich (" << dichNode->data.soTaiKhoan << "): " << sddd(dichNode->data.soDu) << "\n";
            ResetColor();
        } else {
            SetColor(12); cout << "\nSo du khong du! (" << sddd(nguonNode->data.soDu) << ")!\n"; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void kiemTraSoDu() const {
        char stk[MAX_SO_TAI_KHOAN];
        SetColor(11); cout << "\n  === KIEM TRA SO DU ===\n"; ResetColor();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        SetColor(10); cout << "Nhap so tai khoan can kiem tra: "; ResetColor();
        cin.getline(stk, MAX_SO_TAI_KHOAN);

        struct KhachHangNode* khNode = nullptr;
        for (struct KhachHangNode* current = danhSachKH.head; current != nullptr; current = current->next) {
            if (strcmp(current->data.soTaiKhoan, stk) == 0) {
                khNode = current;
                break;
            }
        }

        if (khNode != nullptr) {
            SetColor(14);
            cout << "\nSo du hien tai cua tai khoan " << khNode->data.ten << " (" << khNode->data.soTaiKhoan << ") la: "
                 << sddd(khNode->data.soDu) << " VND" << "\n";
            ResetColor();
        } else {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << "\n"; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void hienThiDanhSach() const {
        if (danhSachKH.isEmpty()) {
            SetColor(12); cout << "\nDanh sach khach hang trong!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }

        SetColor(11); cout << "\n                                === DANH SACH KHACH HANG ===\n"; ResetColor();
        SetColor(14);
        for (int i = 0; i < 125; ++i) { cout << '-'; } cout << "\n";
        cout << setw(12) << left << "So TK"
             << setw(25) << left << "Ho Ten"
             << setw(25) << left << "Email"
             << setw(15) << left << "So DT"
             << setw(15) << left << "CCCD"
             << setw(20) << right << "So Du (VND)" << "\n";
        for (int i = 0; i < 125; ++i) { cout << '-'; } cout << "\n";
        ResetColor();

        struct KhachHangNode* current = danhSachKH.head;
        while (current != nullptr) {
            SetColor(7);

            cout << setw(12) << left << current->data.soTaiKhoan
                 << setw(25) << left << current->data.ten
                 << setw(25) << left << current->data.email
                 << setw(15) << left << current->data.soDienThoai
                 << setw(15) << left << current->data.canCuocCongDan
                 << setw(20) << right << sddd(current->data.soDu) << "\n";
            current = current->next;
        }
        for (int i = 0; i < 125; ++i) { cout << '-'; } cout << "\n";
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void hienThiLogGiaoDich() const {
        if (danhSachGD.isEmpty()) {
            SetColor(12); cout << "\nKhong co giao dich nao duoc ghi!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }
        SetColor(11); cout << "\n                                === LICH SU GIAO DICH ===\n"; ResetColor();
        SetColor(14);
        for (int i = 0; i < 110; ++i) { cout << '-'; } cout << "\n";
        cout << setw(20) << left << "Thoi gian"
             << setw(15) << left << "Loai GD"
             << setw(15) << left << "STK Nguon"
             << setw(15) << left << "STK Dich"
             << setw(20) << right << "So tien (VND)"
             << setw(25) << left << "  Ghi Chu" << "\n";
        for (int i = 0; i < 110; ++i) { cout << '-'; } cout << "\n";
        ResetColor();

        struct GiaoDichNode* current = danhSachGD.head;
        while (current != nullptr) {
            SetColor(7);
            char buffer[MAX_THOI_GIAN_STR];
            struct tm timeinfo;
            localtime_s(&timeinfo, &current->data.thoiGian);
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

            cout << setw(20) << left << buffer
                 << setw(15) << left << current->data.loaiGiaoDich
                 << setw(15) << left << current->data.soTaiKhoanNguon
                 << setw(15) << left << current->data.soTaiKhoanDich
                 << setw(20) << right << current->data.soTien
                 << "  " << left << current->data.ghiChu << "\n";
            current = current->next;
        }
        for (int i = 0; i < 110; ++i) { cout << '-'; } cout << "\n";
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }
};

void hienThiMenuChinh() {
    srand(time(0));
    QuanLyNganHang ql;

    const char* menuItems[] = {
        "Tao tai khoan moi",
        "Tim tai khoan",
        "Xoa tai khoan",
        "Cap nhat thong tin tai khoan",
        "Rut tien",
        "Gui tien",
        "Chuyen khoan",
        "Kiem tra so du",
        "Hien thi danh sach tai khoan",
        "Hien thi lich su giao dich",
        "Luu & Thoat"
    };
    int numMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);

    int currentSelection = 0;
    int menuStartY = 3;

    while (true) {
        system("cls");

        SetColor(11);
        Gotoxy(25, 1);
        cout << "===== HE THONG QUAN LY TAI KHOAN NGAN HANG =====";
        ResetColor();

        for (int i = 0; i < numMenuItems; ++i) {
            Gotoxy(25, menuStartY + i);
            if (i == currentSelection) {
                SetColor(10);
                cout << ">> " << menuItems[i] << " <<";
            } else {
                SetColor(14);
                cout << "    " << menuItems[i];
            }
            ResetColor();
        }

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
                case 72:
                    currentSelection = (currentSelection - 1 + numMenuItems) % numMenuItems;
                    break;
                case 80:
                    currentSelection = (currentSelection + 1) % numMenuItems;
                    break;
            }
        } else if (key == 13) {
            system("cls");
            if (currentSelection == numMenuItems - 1) {
                SetColor(10);
                cout << "\nLuu du lieu va thoat chuong trinh...\n";
                ResetColor();
                Sleep(1000);
                return;
            }

            switch (currentSelection) {
                case 0: ql.themKhachHang(); break;
                case 1: ql.timKhachHang(); break;
                case 2: ql.xoaKhachHang(); break;
                case 3: ql.capNhatThongTin(); break;
                case 4: ql.rutTien(); break;
                case 5: ql.guiTien(); break;
                case 6: ql.chuyenKhoan(); break;
                case 7: ql.kiemTraSoDu(); break;
                case 8: ql.hienThiDanhSach(); break;
                case 9: ql.hienThiLogGiaoDich(); break;
            }
        }
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleTitle(TEXT("He Thong Quan Ly Tai Khoan Ngan Hang"));
    hienThiMenuChinh();
    return 0;
}
