#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cctype>

using namespace std;

#define MAX_SO_TAI_KHOAN 15
#define MAX_TEN_KH 50
#define MAX_EMAIL 50
#define MAX_SO_DIEN_THOAI 15
#define MAX_CAN_CUOC_CONG_DAN 20
#define MAX_LOAI_GD 20
#define MAX_GHI_CHU 50
#define MAX_THOI_GIAN_STR 80

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
    char soTaiKhoan[MAX_SO_TAI_KHOAN + 1];
    char ten[MAX_TEN_KH + 1];
    char email[MAX_EMAIL + 1];
    char soDienThoai[MAX_SO_DIEN_THOAI + 1];
    char canCuocCongDan[MAX_CAN_CUOC_CONG_DAN + 1];
    double soDu;

    KhachHang() {
        strcpy(soTaiKhoan, "");
        strcpy(ten, "");
        strcpy(email, "");
        strcpy(soDienThoai, "");
        strcpy(canCuocCongDan, "");
        soDu = 0.0;
    }
};

struct GiaoDich {
    time_t thoiGian;
    char loaiGiaoDich[MAX_LOAI_GD + 1];
    char soTaiKhoanNguon[MAX_SO_TAI_KHOAN + 1];
    char soTaiKhoanDich[MAX_SO_TAI_KHOAN + 1];
    double soTien;
    char ghiChu[MAX_GHI_CHU + 1];

    GiaoDich() {
        thoiGian = 0;
        strcpy(loaiGiaoDich, "");
        strcpy(soTaiKhoanNguon, "");
        strcpy(soTaiKhoanDich, "");
        soTien = 0.0;
        strcpy(ghiChu, "");
    }
};

class QuanLyNganHang {
private:
    vector<KhachHang> danhSachKH;
    vector<GiaoDich> danhSachGD;
    const char* TEN_FILE_KH = "khachhang.dat";
    const char* TEN_FILE_GD = "giaodich.log";

    bool isSoTaiKhoanUnique(const char* stk) const {
        for (const auto& kh : danhSachKH) {
            if (strcmp(kh.soTaiKhoan, stk) == 0) {
                return false;
            }
        }
        return true;
    }

    void generateUniqueSoTaiKhoan(char* buffer) {
        long long randomNumber;
        do {
            randomNumber = 1000000000LL + (rand() % 9000000000LL);
            sprintf(buffer, "%lld", randomNumber);
        } while (!isSoTaiKhoanUnique(buffer));
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
        FILE* outFile = fopen(TEN_FILE_KH, "wb");
        if (outFile == NULL) return;
        size_t count = danhSachKH.size();
        fwrite(&count, sizeof(size_t), 1, outFile);
        for (const auto& kh : danhSachKH) {
            fwrite(&kh, sizeof(KhachHang), 1, outFile);
        }
        fclose(outFile);
    }

    void docDuLieuKhachHangTuFile() {
        FILE* inFile = fopen(TEN_FILE_KH, "rb");
        if (inFile == NULL) return;
        danhSachKH.clear();
        size_t count;
        if (fread(&count, sizeof(size_t), 1, inFile) != 1) {
            fclose(inFile);
            return;
        }
        for (size_t i = 0; i < count; ++i) {
            KhachHang kh;
            if (fread(&kh, sizeof(KhachHang), 1, inFile) != 1) break;
            danhSachKH.push_back(kh);
        }
        fclose(inFile);
    }

    void ghiDuLieuGiaoDichVaoFile() const {
        FILE* outFile = fopen(TEN_FILE_GD, "w");
        if (outFile == NULL) return;
        for (const auto& gd : danhSachGD) {
            fprintf(outFile, "%lld|%s|%s|%s|%.2f|%s\n",
                    (long long)gd.thoiGian, gd.loaiGiaoDich,
                    gd.soTaiKhoanNguon, gd.soTaiKhoanDich,
                    gd.soTien, gd.ghiChu);
        }
        fclose(outFile);
    }

    void docDuLieuGiaoDichTuFile() {
        FILE* inFile = fopen(TEN_FILE_GD, "r");
        if (inFile == NULL) return;
        danhSachGD.clear();
        char lineBuffer[500];

        while (fgets(lineBuffer, sizeof(lineBuffer), inFile) != NULL) {
            GiaoDich gd;
            long long timestamp;
            char tempLoai[MAX_LOAI_GD + 1] = "";
            char tempSTKNguon[MAX_SO_TAI_KHOAN + 1] = "";
            char tempSTKDich[MAX_SO_TAI_KHOAN + 1] = "";
            char tempGhiChu[MAX_GHI_CHU + 1] = "";

            int num_scanned = sscanf(lineBuffer, "%lld|%[^|]|%[^|]|%[^|]|%lf|%[^\n]",
                                     &timestamp, tempLoai, tempSTKNguon, tempSTKDich, &gd.soTien, tempGhiChu);

            if (num_scanned >= 5) {
                gd.thoiGian = (time_t)timestamp;
                strcpy(gd.loaiGiaoDich, tempLoai);
                strcpy(gd.soTaiKhoanNguon, tempSTKNguon);
                strcpy(gd.soTaiKhoanDich, tempSTKDich);
                strcpy(gd.ghiChu, tempGhiChu);
                danhSachGD.push_back(gd);
            }
        }
        fclose(inFile);
    }

    void themKhachHang() {
        KhachHang newKH;
        SetColor(11); cout << "\n  === TAO TAI KHOAN MOI ===\n"; ResetColor();

        generateUniqueSoTaiKhoan(newKH.soTaiKhoan);
        SetColor(10); cout << "So tai khoan moi duoc tao: " << newKH.soTaiKhoan << endl; ResetColor();

        fflush(stdin);
        SetColor(10); cout << "Nhap ho va ten: "; ResetColor();
        fgets(newKH.ten, sizeof(newKH.ten), stdin);
        newKH.ten[strcspn(newKH.ten, "\n")] = 0;

        SetColor(10); cout << "Nhap Email: "; ResetColor();
        fgets(newKH.email, sizeof(newKH.email), stdin);
        newKH.email[strcspn(newKH.email, "\n")] = 0;

        do {
            SetColor(10); cout << "Nhap So Dien Thoai (10 so, bat dau tu 0): "; ResetColor();
            fgets(newKH.soDienThoai, sizeof(newKH.soDienThoai), stdin);
            newKH.soDienThoai[strcspn(newKH.soDienThoai, "\n")] = 0;

            if (strlen(newKH.soDienThoai) == 10 && newKH.soDienThoai[0] == '0') {
                bool is_digit = true;
                for (int i = 0; i < strlen(newKH.soDienThoai); ++i) {
                    if (!isdigit(newKH.soDienThoai[i])) {
                        is_digit = false;
                        break;
                    }
                }
                if (is_digit) {
                    break;
                }
            }
            SetColor(12); cout << "So dien thoai khong hop le. Vui long nhap 10 chu so va bat dau bang '0'.\n"; ResetColor();
        } while (true);

        do {
            SetColor(10); cout << "Nhap Can Cuoc Cong Dan (CCCD - 12 so): "; ResetColor();
            fgets(newKH.canCuocCongDan, sizeof(newKH.canCuocCongDan), stdin);
            newKH.canCuocCongDan[strcspn(newKH.canCuocCongDan, "\n")] = 0;

            if (strlen(newKH.canCuocCongDan) == 12) {
                bool is_digit = true;
                for (int i = 0; i < strlen(newKH.canCuocCongDan); ++i) {
                    if (!isdigit(newKH.canCuocCongDan[i])) {
                        is_digit = false;
                        break;
                    }
                }
                if (is_digit) {
                    break;
                }
            }
            SetColor(12); cout << "So Can Cuoc Cong Dan khong hop le. Vui long nhap 12 chu so.\n"; ResetColor();
        } while (true);

        SetColor(10); cout << "Nhap so du ban dau: "; ResetColor();
        while (scanf("%lf", &newKH.soDu) != 1 || newKH.soDu < 0) {
            SetColor(12); cout << "Du lieu khong hop le. Vui long nhap so du la mot so khong am.\n"; ResetColor();
            while (getchar() != '\n');
            SetColor(10); cout << "Nhap so du ban dau: "; ResetColor();
        }
        while (getchar() != '\n');

        danhSachKH.push_back(newKH);
        SetColor(10); cout << "\nTao tai khoan moi thanh cong!\n"; ResetColor();
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void timKhachHang() const {
        char stk[MAX_SO_TAI_KHOAN + 1];
        SetColor(11); cout << "\n  === TIM TAI KHOAN ===\n"; ResetColor();

        fflush(stdin);
        SetColor(10); cout << "Nhap so tai khoan can tim: "; ResetColor();
        fgets(stk, sizeof(stk), stdin);
        stk[strcspn(stk, "\n")] = 0;

        bool found = false;
        for (const auto& kh : danhSachKH) {
            if (strcmp(kh.soTaiKhoan, stk) == 0) {
                SetColor(14);
                cout << "\nThong tin khach hang:\n";
                cout << "----------------------------------------\n";
                ResetColor();
                cout << "So tai khoan        : " << kh.soTaiKhoan << endl;
                cout << "Ho ten              : " << kh.ten << endl;
                cout << "Email               : " << kh.email << endl;
                cout << "So dien thoai       : " << kh.soDienThoai << endl;
                cout << "CCCD                : " << kh.canCuocCongDan << endl;
                cout << "So du               : " << fixed << setprecision(2) << kh.soDu << " VND" << endl;
                SetColor(14);
                cout << "----------------------------------------\n";
                ResetColor();
                found = true;
                break;
            }
        }

        if (!found) {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << endl; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void xoaKhachHang() {
        char stk[MAX_SO_TAI_KHOAN + 1];
        SetColor(11); cout << "\n  === XOA TAI KHOAN ===\n"; ResetColor();

        fflush(stdin);
        SetColor(10); cout << "Nhap so tai khoan can xoa: "; ResetColor();
        fgets(stk, sizeof(stk), stdin);
        stk[strcspn(stk, "\n")] = 0;

        bool found = false;
        for (auto it = danhSachKH.begin(); it != danhSachKH.end(); ++it) {
            if (strcmp(it->soTaiKhoan, stk) == 0) {
                SetColor(10); cout << "\nDa xoa tai khoan cua khach hang " << it->ten << " (" << it->soTaiKhoan << ").\n"; ResetColor();
                danhSachKH.erase(it);
                found = true;
                break;
            }
        }

        if (!found) {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << endl; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void capNhatThongTin() {
        char stk[MAX_SO_TAI_KHOAN + 1];
        SetColor(11); cout << "\n  === CAP NHAT THONG TIN TAI KHOAN ===\n"; ResetColor();

        fflush(stdin);
        SetColor(10); cout << "Nhap so tai khoan can cap nhat: "; ResetColor();
        fgets(stk, sizeof(stk), stdin);
        stk[strcspn(stk, "\n")] = 0;

        bool found = false;
        for (auto& kh : danhSachKH) {
            if (strcmp(kh.soTaiKhoan, stk) == 0) {
                found = true;
                SetColor(14); cout << "\nTim thay tai khoan: " << kh.ten << ". Nhap thong tin moi (de trong de giu lai thong tin cu).\n"; ResetColor();

                char newTen[MAX_TEN_KH + 1];
                char newEmail[MAX_EMAIL + 1];
                char newSDT[MAX_SO_DIEN_THOAI + 1];
                char newCCCD[MAX_CAN_CUOC_CONG_DAN + 1];

                SetColor(10); cout << "Ten moi [" << kh.ten << "]: "; ResetColor();
                fgets(newTen, sizeof(newTen), stdin);
                newTen[strcspn(newTen, "\n")] = 0;
                if (strlen(newTen) > 0) strcpy(kh.ten, newTen);

                SetColor(10); cout << "Email moi [" << kh.email << "]: "; ResetColor();
                fgets(newEmail, sizeof(newEmail), stdin);
                newEmail[strcspn(newEmail, "\n")] = 0;
                if (strlen(newEmail) > 0) strcpy(kh.email, newEmail);

                do {
                    SetColor(10); cout << "So dien thoai moi [" << kh.soDienThoai << "] (10 so, bat dau tu 0, de trong de giu cu): "; ResetColor();
                    fgets(newSDT, sizeof(newSDT), stdin);
                    newSDT[strcspn(newSDT, "\n")] = 0;

                    if (strlen(newSDT) == 0) {
                        break;
                    } else if (strlen(newSDT) == 10 && newSDT[0] == '0') {
                        bool is_digit = true;
                        for (int i = 0; i < strlen(newSDT); ++i) {
                            if (!isdigit(newSDT[i])) {
                                is_digit = false;
                                break;
                            }
                        }
                        if (is_digit) {
                            strcpy(kh.soDienThoai, newSDT);
                            break;
                        }
                    }
                    SetColor(12); cout << "So dien thoai moi khong hop le. Vui long nhap 10 chu so va bat dau bang '0', hoac de trong.\n"; ResetColor();
                } while (true);

                do {
                    SetColor(10); cout << "CCCD moi [" << kh.canCuocCongDan << "] (12 so, de trong de giu cu): "; ResetColor();
                    fgets(newCCCD, sizeof(newCCCD), stdin);
                    newCCCD[strcspn(newCCCD, "\n")] = 0;

                    if (strlen(newCCCD) == 0) {
                        break;
                    } else if (strlen(newCCCD) == 12) {
                        bool is_digit = true;
                        for (int i = 0; i < strlen(newCCCD); ++i) {
                            if (!isdigit(newCCCD[i])) {
                                is_digit = false;
                                break;
                            }
                        }
                        if (is_digit) {
                            strcpy(kh.canCuocCongDan, newCCCD);
                            break;
                        }
                    }
                    SetColor(12); cout << "So Can Cuoc Cong Dan moi khong hop le. Vui long nhap 12 chu so, hoac de trong.\n"; ResetColor();
                } while (true);

                SetColor(10); cout << "\nCap nhat thong tin thanh cong!\n"; ResetColor();
                break;
            }
        }

        if (!found) {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << endl; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void rutTien() {
        char stk[MAX_SO_TAI_KHOAN + 1];
        double soTien;
        SetColor(11); cout << "\n  === RUT TIEN TU TAI KHOAN ===\n"; ResetColor();

        fflush(stdin);
        SetColor(10); cout << "Nhap so tai khoan: "; ResetColor();
        fgets(stk, sizeof(stk), stdin);
        stk[strcspn(stk, "\n")] = 0;

        for (auto& kh : danhSachKH) {
            if (strcmp(kh.soTaiKhoan, stk) == 0) {
                SetColor(10); cout << "Nhap so tien can rut: "; ResetColor();
                while (scanf("%lf", &soTien) != 1 || soTien <= 0) {
                    SetColor(12); cout << "So tien khong hop le. Vui long nhap mot so duong.\n"; ResetColor();
                    while (getchar() != '\n');
                    SetColor(10); cout << "Nhap so tien can rut: "; ResetColor();
                }
                while (getchar() != '\n');

                if (soTien <= kh.soDu) {
                    kh.soDu -= soTien;
                    GiaoDich newGD;
                    newGD.thoiGian = time(0);
                    strcpy(newGD.loaiGiaoDich, "Rut");
                    strcpy(newGD.soTaiKhoanNguon, stk);
                    strcpy(newGD.soTaiKhoanDich, "---");
                    newGD.soTien = soTien;
                    strcpy(newGD.ghiChu, "Rut tien mat");
                    danhSachGD.push_back(newGD);

                    SetColor(10); cout << "\nRut tien thanh cong! So du moi: " << fixed << setprecision(2) << kh.soDu << endl; ResetColor();
                } else {
                    SetColor(12); cout << "\nSo du khong du! So du hien tai: " << fixed << setprecision(2) << kh.soDu << endl; ResetColor();
                }
                cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
                return;
            }
        }
        SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << endl; ResetColor();
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void guiTien() {
        char stk[MAX_SO_TAI_KHOAN + 1];
        double soTien;
        SetColor(11); cout << "\n  === GUI TIEN VAO TAI KHOAN ===\n"; ResetColor();

        fflush(stdin);
        SetColor(10); cout << "Nhap so tai khoan: "; ResetColor();
        fgets(stk, sizeof(stk), stdin);
        stk[strcspn(stk, "\n")] = 0;

        for (auto& kh : danhSachKH) {
            if (strcmp(kh.soTaiKhoan, stk) == 0) {
                SetColor(10); cout << "Nhap so tien can gui: "; ResetColor();
                while (scanf("%lf", &soTien) != 1 || soTien <= 0) {
                    SetColor(12); cout << "So tien khong hop le. Vui long nhap mot so duong.\n"; ResetColor();
                    while (getchar() != '\n');
                    SetColor(10); cout << "Nhap so tien can gui: "; ResetColor();
                }
                while (getchar() != '\n');

                kh.soDu += soTien;
                GiaoDich newGD;
                newGD.thoiGian = time(0);
                strcpy(newGD.loaiGiaoDich, "Gui");
                strcpy(newGD.soTaiKhoanNguon, stk);
                strcpy(newGD.soTaiKhoanDich, "---");
                newGD.soTien = soTien;
                strcpy(newGD.ghiChu, "Gui tien mat");
                danhSachGD.push_back(newGD);

                SetColor(10); cout << "\nGui tien thanh cong! So du moi: " << fixed << setprecision(2) << kh.soDu << endl; ResetColor();
                cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
                return;
            }
        }
        SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << endl; ResetColor();
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void chuyenKhoan() {
        char stkNguon[MAX_SO_TAI_KHOAN + 1], stkDich[MAX_SO_TAI_KHOAN + 1];
        double soTien;
        SetColor(11); cout << "\n  === CHUYEN KHOAN GIUA CAC TAI KHOAN ===\n"; ResetColor();

        fflush(stdin);
        SetColor(10); cout << "Nhap so tai khoan nguon: "; ResetColor();
        fgets(stkNguon, sizeof(stkNguon), stdin);
        stkNguon[strcspn(stkNguon, "\n")] = 0;

        SetColor(10); cout << "Nhap so tai khoan dich: "; ResetColor();
        fgets(stkDich, sizeof(stkDich), stdin);
        stkDich[strcspn(stkDich, "\n")] = 0;

        if (strcmp(stkNguon, stkDich) == 0) {
            SetColor(12); cout << "\nKhong the chuyen tien cho chinh minh!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }

        KhachHang* nguon = nullptr;
        KhachHang* dich = nullptr;

        for (auto& kh : danhSachKH) {
            if (strcmp(kh.soTaiKhoan, stkNguon) == 0) nguon = &kh;
            if (strcmp(kh.soTaiKhoan, stkDich) == 0) dich = &kh;
        }

        if (!nguon) {
            SetColor(12); cout << "\nKhong tim thay so tai khoan nguon!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }
        if (!dich) {
            SetColor(12); cout << "\nKhong tim thay so tai khoan dich!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }

        SetColor(10); cout << "Nhap so tien can chuyen: "; ResetColor();
        while (scanf("%lf", &soTien) != 1 || soTien <= 0) {
            SetColor(12); cout << "So tien khong hop le. Vui long nhap mot so duong.\n"; ResetColor();
            while (getchar() != '\n');
            SetColor(10); cout << "Nhap so tien can chuyen: "; ResetColor();
        }
        while (getchar() != '\n');

        if (soTien <= nguon->soDu) {
            nguon->soDu -= soTien;
            dich->soDu += soTien;

            GiaoDich newGD;
            newGD.thoiGian = time(0);
            strcpy(newGD.loaiGiaoDich, "ChuyenKhoan");
            strcpy(newGD.soTaiKhoanNguon, stkNguon);
            strcpy(newGD.soTaiKhoanDich, stkDich);
            sprintf(newGD.ghiChu, "%s -> %s", nguon->ten, dich->ten);
            newGD.soTien = soTien;
            danhSachGD.push_back(newGD);

            SetColor(10);
            cout << "\nChuyen tien thanh cong!\n";
            cout << "So du tai khoan nguon (" << nguon->soTaiKhoan << "): " << fixed << setprecision(2) << nguon->soDu << endl;
            cout << "So du tai khoan dich (" << dich->soTaiKhoan << "): " << fixed << setprecision(2) << dich->soDu << endl;
            ResetColor();
        } else {
            SetColor(12); cout << "\nSo du khong du! (" << fixed << setprecision(2) << nguon->soDu << ")!\n"; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tiep..."; _getch();
    }

    void kiemTraSoDu() const {
        char stk[MAX_SO_TAI_KHOAN + 1];
        SetColor(11); cout << "\n  === KIEM TRA SO DU ===\n"; ResetColor();

        fflush(stdin);
        SetColor(10); cout << "Nhap so tai khoan can kiem tra: "; ResetColor();
        fgets(stk, sizeof(stk), stdin);
        stk[strcspn(stk, "\n")] = 0;

        bool found = false;
        for (const auto& kh : danhSachKH) {
            if (strcmp(kh.soTaiKhoan, stk) == 0) {
                SetColor(14);
                cout << "\nSo du hien tai cua tai khoan " << kh.ten << " (" << kh.soTaiKhoan << ") la: "
                     << fixed << setprecision(2) << kh.soDu << " VND" << endl;
                ResetColor();
                found = true;
                break;
            }
        }

        if (!found) {
            SetColor(12); cout << "\nKhong tim thay khach hang co so tai khoan " << stk << endl; ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void hienThiDanhSach() const {
        if (danhSachKH.empty()) {
            SetColor(12); cout << "\nDanh sach khach hang trong!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }

        SetColor(11); cout << "\n                                 === DANH SACH KHACH HANG ===\n"; ResetColor();
        SetColor(14);
        cout << string(125, '-') << endl;
        cout << setw(12) << left << "So TK"
             << setw(25) << left << "Ho Ten"
             << setw(25) << left << "Email"
             << setw(15) << left << "So DT"
             << setw(15) << left << "CCCD"
             << setw(20) << right << "So Du (VND)" << endl;
        cout << string(125, '-') << endl;
        ResetColor();

        for (const auto& kh : danhSachKH) {
            SetColor(7);
            cout << setw(12) << left << kh.soTaiKhoan
                 << setw(25) << left << kh.ten
                 << setw(25) << left << kh.email
                 << setw(15) << left << kh.soDienThoai
                 << setw(15) << left << kh.canCuocCongDan
                 << setw(20) << right << fixed << setprecision(2) << kh.soDu << endl;
        }
        cout << string(125, '-') << endl;
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }

    void hienThiLogGiaoDich() const {
        if (danhSachGD.empty()) {
            SetColor(12); cout << "\nKhong co giao dich nao duoc ghi!\n"; ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            return;
        }

        SetColor(11); cout << "\n                               === LICH SU GIAO DICH ===\n"; ResetColor();
        SetColor(14);
        cout << string(110, '-') << endl;
        cout << setw(20) << left << "Thoi gian"
             << setw(15) << left << "Loai GD"
             << setw(15) << left << "STK Nguon"
             << setw(15) << left << "STK Dich"
             << setw(20) << right << "So tien (VND)"
             << setw(25) << left << "  Ghi Chu" << endl;
        cout << string(110, '-') << endl;
        ResetColor();

        for (const auto& gd : danhSachGD) {
            SetColor(7);
            char buffer[MAX_THOI_GIAN_STR];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&gd.thoiGian));
            cout << setw(20) << left << buffer
                 << setw(15) << left << gd.loaiGiaoDich
                 << setw(15) << left << gd.soTaiKhoanNguon
                 << setw(15) << left << gd.soTaiKhoanDich
                 << setw(20) << right << fixed << setprecision(2) << gd.soTien
                 << "  " << left << gd.ghiChu << endl;
        }
        cout << string(110, '-') << endl;
        cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
    }
};
void hienThiMenuChinh() {
    srand(time(0));
    QuanLyNganHang ql;

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
        "Hien thi lich su giao dich",
        "Luu & Thoat"
    };

    int currentSelection = 0;
    int menuStartY = 3;

    while (true) {
        system("cls");
        SetColor(11);
        Gotoxy(25, 1);
        cout << "===== HE THONG QUAN LY TAI KHOAN NGAN HANG =====";
        ResetColor();

        for (int i = 0; i < menuItems.size(); ++i) {
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
                case 72: currentSelection = (currentSelection - 1 + menuItems.size()) % menuItems.size(); break;
                case 80: currentSelection = (currentSelection + 1) % menuItems.size(); break;
            }
        } else if (key == 13) {
            system("cls");
            if (currentSelection == menuItems.size() - 1) {
                SetColor(10);
                cout << "\nLuu du lieu va thoat chuong trinh...\n";
                ResetColor();
                Sleep(1500);
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
