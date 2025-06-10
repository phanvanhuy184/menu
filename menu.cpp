#include <iostream>
#include <vector>
#include <string> // Van su dung string cho menu, thong bao,...
#include <iomanip> // For fixed and setprecision
#include <cstdio>  // For fopen, fclose, fwrite, fread, fprintf, fscanf
#include <cstdlib> // For system
#include <cstring> // For strcpy, strlen
#include <conio.h>   // Ho tro _kbhit() va _getch()
#include <windows.h> // Ho tro cac ham console cua Windows
#include <ctime>   // For time functions

using namespace std; // Van dung namespace std cho tien loi

// Dinh nghia kich thuoc toi da cho cac chuoi
#define MAX_MA_KH 10
#define MAX_TEN_KH 30
#define MAX_LOAI_GD 20
#define MAX_GHI_CHU 50
#define MAX_THOI_GIAN_STR 80 // Cho chuoi hien thi thoi gian

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

// Struct KhachHang (thay vi class de de hon voi C-style I/O)
struct KhachHang {
    char maKH[MAX_MA_KH + 1]; // +1 cho ky tu null ket thuc chuoi
    char tenKH[MAX_TEN_KH + 1];
    double soDu;

    // Constructor mac dinh
    KhachHang() {
        strcpy(maKH, "");
        strcpy(tenKH, "");
        soDu = 0.0;
    }
};

// Struct GiaoDich (thay vi class de de hon voi C-style I/O)
struct GiaoDich {
    time_t thoiGian;
    char loaiGiaoDich[MAX_LOAI_GD + 1];
    char maKHNguon[MAX_MA_KH + 1];
    char maKHDich[MAX_MA_KH + 1]; // Co the de trong
    double soTien;
    char ghiChu[MAX_GHI_CHU + 1]; // Co the de trong

    // Constructor mac dinh
    GiaoDich() {
        thoiGian = 0;
        strcpy(loaiGiaoDich, "");
        strcpy(maKHNguon, "");
        strcpy(maKHDich, "");
        soTien = 0.0;
        strcpy(ghiChu, "");
    }
};

// Lop quan ly ngan hang
class QuanLyNganHang {
private:
    vector<KhachHang> danhSachKH;
    vector<GiaoDich> danhSachGD;
    const char* TEN_FILE_KH = "khachhang.dat"; // Ten file du lieu khach hang
    const char* TEN_FILE_GD = "giaodich.log";  // Ten file nhat ky giao dich (text file)

public:
    QuanLyNganHang() {
        docDuLieuKhachHangTuFile();
        docDuLieuGiaoDichTuFile();
    }

    ~QuanLyNganHang() {
        ghiDuLieuKhachHangVaoFile();
        ghiDuLieuGiaoDichVaoFile();
    }

    // --- Quan ly file KhachHang (nhap phan voi fwrite/fread) ---
    void ghiDuLieuKhachHangVaoFile() const {
        FILE* outFile = fopen(TEN_FILE_KH, "wb"); // "wb" - write binary
        if (outFile == NULL) {
            SetColor(12);
            cout << "\nLoi: Khong the mo file Khach Hang de ghi du lieu!\n";
            ResetColor();
            return;
        }

        // Ghi so luong khach hang
        size_t count = danhSachKH.size();
        fwrite(&count, sizeof(size_t), 1, outFile);

        // Ghi tung khach hang
        for (const auto& kh : danhSachKH) {
            fwrite(&kh, sizeof(KhachHang), 1, outFile);
        }

        fclose(outFile);
        SetColor(10);
        cout << "\nDu lieu khach hang da duoc luu vao file thanh cong!\n";
        ResetColor();
    }

    void docDuLieuKhachHangTuFile() {
        FILE* inFile = fopen(TEN_FILE_KH, "rb"); // "rb" - read binary
        if (inFile == NULL) {
            SetColor(12);
            cout << "\nFile du lieu khach hang khong ton tai hoac khong the mo. Bat dau voi danh sach trong.\n";
            ResetColor();
            return;
        }

        danhSachKH.clear(); // Xoa du lieu hien tai truoc khi doc moi

        // Doc so luong khach hang
        size_t count;
        fread(&count, sizeof(size_t), 1, inFile);

        // Doc tung khach hang
        for (size_t i = 0; i < count; ++i) {
            KhachHang kh;
            fread(&kh, sizeof(KhachHang), 1, inFile);
            danhSachKH.push_back(kh);
        }

        fclose(inFile);
        SetColor(10);
        cout << "\nDu lieu khach hang da duoc doc tu file thanh cong!\n";
        ResetColor();
    }

    // --- Quan ly file GiaoDich (nhat ky dang text voi fprintf/fgets) ---
    void ghiDuLieuGiaoDichVaoFile() const {
        // "a" - append, mo file de ghi them vao cuoi file.
        // Neu file khong ton tai, no se duoc tao.
        FILE* outFile = fopen(TEN_FILE_GD, "a");
        if (outFile == NULL) {
            SetColor(12);
            cout << "\nLoi: Khong the mo file Giao Dich de ghi du lieu!\n";
            ResetColor();
            return;
        }
        fclose(outFile); // Dong file "a" vua mo
        outFile = fopen(TEN_FILE_GD, "w"); // Mo lai voi "w"
        if (outFile == NULL) {
            SetColor(12);
            cout << "\nLoi: Khong the mo file Giao Dich de ghi du lieu (che do w)!\n";
            ResetColor();
            return;
        }

        for (const auto& gd : danhSachGD) {
            char thoiGianStr[MAX_THOI_GIAN_STR];
            strftime(thoiGianStr, sizeof(thoiGianStr), "%Y-%m-%d %H:%M:%S", localtime(&gd.thoiGian));

            // Ghi du lieu theo dinh dang text
            fprintf(outFile, "%s|%s|%s|%s|%.2f|%s\n",
                    thoiGianStr,
                    gd.loaiGiaoDich,
                    gd.maKHNguon,
                    gd.maKHDich,
                    gd.soTien,
                    gd.ghiChu);
        }

        fclose(outFile);
        SetColor(10);
        cout << "\nDu lieu giao dich da duoc luu vao file log thanh cong!\n";
        ResetColor();
    }

    void docDuLieuGiaoDichTuFile() {
        FILE* inFile = fopen(TEN_FILE_GD, "r"); // "r" - read text
        if (inFile == NULL) {
            SetColor(12);
            cout << "\nFile nhat ky giao dich khong ton tai hoac khong the mo. Bat dau voi danh sach trong.\n";
            ResetColor();
            return;
        }

        danhSachGD.clear(); // Xoa du lieu hien tai truoc khi doc moi

        char lineBuffer[500]; // Buffer de doc tung dong
        while (fgets(lineBuffer, sizeof(lineBuffer), inFile) != NULL) {
            GiaoDich gd;
            char thoiGianStr[MAX_THOI_GIAN_STR];
            long long rawTime;
            sscanf(lineBuffer, "%[^|]|%[^|]|%[^|]|%[^|]|%lf|%[^\n]\n",
                   thoiGianStr,
                   gd.loaiGiaoDich,
                   gd.maKHNguon,
                   gd.maKHDich,
                   &gd.soTien,
                   gd.ghiChu);
            long long timestamp;
            int num_scanned = sscanf(lineBuffer, "%lld|%[^|]|%[^|]|%[^|]|%lf|%[^\n]\n",
                                     &timestamp,
                                     gd.loaiGiaoDich,
                                     gd.maKHNguon,
                                     gd.maKHDich,
                                     &gd.soTien,
                                     gd.ghiChu);

            if (num_scanned >= 5) { // It nhat 5 truong phai duoc doc
                gd.thoiGian = (time_t)timestamp;
                danhSachGD.push_back(gd);
            }
        }

        fclose(inFile);
        SetColor(10);
        cout << "\nDu lieu giao dich da duoc doc tu file log thanh cong!\n";
        ResetColor();
    }


    // --- Cac chuc nang quan ly khach hang va giao dich ---
    void themKhachHang() {
        char ma[MAX_MA_KH + 1], ten[MAX_TEN_KH + 1];
        double du;

        SetColor(11);
        cout << "\n  === TAO TAI KHOAN MOI ===\n";
        ResetColor();

        // Xoa bo dem dau vao truoc khi doc chuoi
        fflush(stdin); // C-style alternative to cin.ignore()

        SetColor(10);
        cout << "Nhap ma khach hang: ";
        ResetColor();
        fgets(ma, sizeof(ma), stdin);
        ma[strcspn(ma, "\n")] = 0; // Xoa ky tu newline

        // Kiem tra ma khach hang da ton tai chua
        for (const auto& kh : danhSachKH) {
            if (strcmp(kh.maKH, ma) == 0) {
                SetColor(12);
                cout << "\nLoi: Ma khach hang da ton tai. Vui long nhap ma khac!\n";
                ResetColor();
                cout << "\nNhan phim bat ky de tiep tuc...";
                _getch();
                return;
            }
        }

        SetColor(10);
        cout << "Nhap ten khach hang: ";
        ResetColor();
        fgets(ten, sizeof(ten), stdin);
        ten[strcspn(ten, "\n")] = 0; // Xoa ky tu newline

        SetColor(10);
        cout << "Nhap so du ban dau: ";
        ResetColor();
        // Doc so voi scanf, khong can fflush(stdin) sau do neu dung fgets
        if (scanf("%lf", &du) != 1) {
            SetColor(12);
            cout << "Du lieu khong hop le. Vui long nhap so du la mot so.\n";
            ResetColor();
            // Xoa du lieu sai trong bo dem
            while (getchar() != '\n');
            cout << "\nNhan phim bat ky de tiep tuc...";
            _getch();
            return;
        }

        KhachHang newKH;
        strcpy(newKH.maKH, ma);
        strcpy(newKH.tenKH, ten);
        newKH.soDu = du;
        danhSachKH.push_back(newKH);

        SetColor(10);
        cout << "\nTao tai khoan moi thanh cong!\n";
        ResetColor();
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void timKhachHang() const {
        char ma[MAX_MA_KH + 1];
        SetColor(11);
        cout << "\n  === TIM TAI KHOAN ===\n";
        ResetColor();

        fflush(stdin);
        SetColor(10);
        cout << "Nhap ma khach hang can tim: ";
        ResetColor();
        fgets(ma, sizeof(ma), stdin);
        ma[strcspn(ma, "\n")] = 0;

        bool found = false;
        for (const auto& kh : danhSachKH) {
            if (strcmp(kh.maKH, ma) == 0) {
                SetColor(14);
                cout << "\nThong tin khach hang:\n";
                cout << setw(10) << left << "Ma KH"
                     << setw(20) << left << "Ten KH"
                     << setw(15) << right << "So du" << endl;
                cout << string(45, '-') << endl;
                ResetColor();
                cout << setw(10) << left << kh.maKH
                     << setw(20) << left << kh.tenKH
                     << setw(15) << right << fixed << setprecision(2) << kh.soDu << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            SetColor(12);
            cout << "\nKhong tim thay khach hang co ma " << ma << endl;
            ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void xoaKhachHang() {
        char ma[MAX_MA_KH + 1];
        SetColor(11);
        cout << "\n  === XOA TAI KHOAN ===\n";
        ResetColor();

        fflush(stdin);
        SetColor(10);
        cout << "Nhap ma khach hang can xoa: ";
        ResetColor();
        fgets(ma, sizeof(ma), stdin);
        ma[strcspn(ma, "\n")] = 0;

        bool found = false;
        for (auto it = danhSachKH.begin(); it != danhSachKH.end(); ++it) {
            if (strcmp(it->maKH, ma) == 0) {
                SetColor(10);
                cout << "\nDa xoa tai khoan cua khach hang " << it->tenKH << " (" << it->maKH << ").\n";
                ResetColor();
                danhSachKH.erase(it);
                found = true;
                break;
            }
        }

        if (!found) {
            SetColor(12);
            cout << "\nKhong tim thay khach hang co ma " << ma << endl;
            ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void capNhatThongTin() {
        char ma[MAX_MA_KH + 1];
        SetColor(11);
        cout << "\n  === CAP NHAT THONG TIN TAI KHOAN ===\n";
        ResetColor();

        fflush(stdin);
        SetColor(10);
        cout << "Nhap ma khach hang can cap nhat: ";
        ResetColor();
        fgets(ma, sizeof(ma), stdin);
        ma[strcspn(ma, "\n")] = 0;

        bool found = false;
        for (auto& kh : danhSachKH) {
            if (strcmp(kh.maKH, ma) == 0) {
                char newTen[MAX_TEN_KH + 1];
                SetColor(10);
                cout << "Nhap ten moi cho khach hang (de trong neu khong doi): ";
                ResetColor();
                fgets(newTen, sizeof(newTen), stdin);
                newTen[strcspn(newTen, "\n")] = 0; // Xoa ky tu newline

                if (strlen(newTen) > 0) { // Neu ten moi khong rong
                    strcpy(kh.tenKH, newTen);
                }

                SetColor(10);
                cout << "\nCap nhat thong tin thanh cong!\n";
                ResetColor();
                found = true;
                break;
            }
        }

        if (!found) {
            SetColor(12);
            cout << "\nKhong tim thay khach hang co ma " << ma << endl;
            ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void rutTien() {
        char ma[MAX_MA_KH + 1];
        double soTien;

        SetColor(11);
        cout << "\n  === RUT TIEN TU TAI KHOAN ===\n";
        ResetColor();

        fflush(stdin);
        SetColor(10);
        cout << "Nhap ma khach hang: ";
        ResetColor();
        fgets(ma, sizeof(ma), stdin);
        ma[strcspn(ma, "\n")] = 0;

        for (auto& kh : danhSachKH) {
            if (strcmp(kh.maKH, ma) == 0) {
                SetColor(10);
                cout << "Nhap so tien can rut: ";
                ResetColor();
                if (scanf("%lf", &soTien) != 1) {
                    SetColor(12);
                    cout << "Du lieu khong hop le. Vui long nhap so tien la mot so.\n";
                    ResetColor();
                    while (getchar() != '\n');
                    cout << "\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    return;
                }
                while (getchar() != '\n'); // Clear newline

                if (soTien > 0 && soTien <= kh.soDu) {
                    kh.soDu -= soTien;

                    GiaoDich newGD;
                    newGD.thoiGian = time(0);
                    strcpy(newGD.loaiGiaoDich, "Rut");
                    strcpy(newGD.maKHNguon, ma);
                    strcpy(newGD.maKHDich, "---"); // Khong co ma dich
                    newGD.soTien = soTien;
                    strcpy(newGD.ghiChu, "Rut tien mat");
                    danhSachGD.push_back(newGD);

                    SetColor(10);
                    cout << "\nRut tien thanh cong! So du moi: " << fixed << setprecision(2) << kh.soDu << endl;
                    ResetColor();
                } else {
                    SetColor(12);
                    cout << "\nSo tien khong hop le hoac vuot qua so du! So du hien tai: " << fixed << setprecision(2) << kh.soDu << endl;
                    ResetColor();
                }
                cout << "\nNhan phim bat ky de tiep tuc...";
                _getch();
                return;
            }
        }

        SetColor(12);
        cout << "\nKhong tim thay khach hang co ma " << ma << endl;
        ResetColor();
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void guiTien() {
        char ma[MAX_MA_KH + 1];
        double soTien;

        SetColor(11);
        cout << "\n  === GUI TIEN VAO TAI KHOAN ===\n";
        ResetColor();

        fflush(stdin);
        SetColor(10);
        cout << "Nhap ma khach hang: ";
        ResetColor();
        fgets(ma, sizeof(ma), stdin);
        ma[strcspn(ma, "\n")] = 0;

        for (auto& kh : danhSachKH) {
            if (strcmp(kh.maKH, ma) == 0) {
                SetColor(10);
                cout << "Nhap so tien can gui: ";
                ResetColor();
                if (scanf("%lf", &soTien) != 1) {
                    SetColor(12);
                    cout << "Du lieu khong hop le. Vui long nhap so tien la mot so.\n";
                    ResetColor();
                    while (getchar() != '\n');
                    cout << "\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    return;
                }
                while (getchar() != '\n');

                if (soTien > 0) {
                    kh.soDu += soTien;

                    GiaoDich newGD;
                    newGD.thoiGian = time(0);
                    strcpy(newGD.loaiGiaoDich, "Gui");
                    strcpy(newGD.maKHNguon, ma);
                    strcpy(newGD.maKHDich, "---");
                    newGD.soTien = soTien;
                    strcpy(newGD.ghiChu, "Gui tien mat");
                    danhSachGD.push_back(newGD);

                    SetColor(10);
                    cout << "\nGui tien thanh cong! So du moi: " << fixed << setprecision(2) << kh.soDu << endl;
                    ResetColor();
                } else {
                    SetColor(12);
                    cout << "\nSo tien khong hop le!\n";
                    ResetColor();
                }
                cout << "\nNhan phim bat ky de tiep tuc...";
                _getch();
                return;
            }
        }

        SetColor(12);
        cout << "\nKhong tim thay khach hang co ma " << ma << endl;
        ResetColor();
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void chuyenKhoan() {
        char maNguon[MAX_MA_KH + 1], maDich[MAX_MA_KH + 1];
        double soTien;

        SetColor(11);
        cout << "\n  === CHUYEN KHOAN GIUA CAC TAI KHOAN ===\n";
        ResetColor();

        fflush(stdin);
        SetColor(10);
        cout << "Nhap ma khach hang nguon: ";
        ResetColor();
        fgets(maNguon, sizeof(maNguon), stdin);
        maNguon[strcspn(maNguon, "\n")] = 0;

        SetColor(10);
        cout << "Nhap ma khach hang dich: ";
        ResetColor();
        fgets(maDich, sizeof(maDich), stdin);
        maDich[strcspn(maDich, "\n")] = 0;

        KhachHang* nguon = nullptr;
        KhachHang* dich = nullptr;

        for (auto& kh : danhSachKH) {
            if (strcmp(kh.maKH, maNguon) == 0) nguon = &kh;
            if (strcmp(kh.maKH, maDich) == 0) dich = &kh;
        }

        if (!nguon || !dich) {
            SetColor(12);
            cout << "\nKhong tim thay khach hang nguon hoac dich!\n";
            ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc...";
            _getch();
            return;
        }
        if (strcmp(nguon->maKH, dich->maKH) == 0) {
            SetColor(12);
            cout << "\nKhong the chuyen tien cho chinh tai khoan cua minh!\n";
            ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc...";
            _getch();
            return;
        }

        SetColor(10);
        cout << "Nhap so tien can chuyen: ";
        ResetColor();
        if (scanf("%lf", &soTien) != 1) {
            SetColor(12);
            cout << "Du lieu khong hop le. Vui long nhap so tien la mot so.\n";
            ResetColor();
            while (getchar() != '\n');
            cout << "\nNhan phim bat ky de tiep tuc...";
            _getch();
            return;
        }
        while (getchar() != '\n');

        if (soTien > 0 && soTien <= nguon->soDu) {
            nguon->soDu -= soTien;
            dich->soDu += soTien;

            GiaoDich newGD;
            newGD.thoiGian = time(0);
            strcpy(newGD.loaiGiaoDich, "ChuyenKhoan");
            strcpy(newGD.maKHNguon, maNguon);
            strcpy(newGD.maKHDich, maDich);
            sprintf(newGD.ghiChu, "%s -> %s", maNguon, maDich); // Tao ghi chu
            newGD.soTien = soTien;
            danhSachGD.push_back(newGD);

            SetColor(10);
            cout << "\nChuyen tien thanh cong!\n";
            cout << "So du tai khoan nguon (" << nguon->maKH << "): " << fixed << setprecision(2) << nguon->soDu << endl;
            cout << "So du tai khoan dich (" << dich->maKH << "): " << fixed << setprecision(2) << dich->soDu << endl;
            ResetColor();
        } else {
            SetColor(12);
            cout << "\nSo tien khong hop le hoac vuot qua so du cua tai khoan nguon (" << fixed << setprecision(2) << nguon->soDu << ")!\n";
            ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void kiemTraSoDu() const {
        char ma[MAX_MA_KH + 1];
        SetColor(11);
        cout << "\n  === KIEM TRA SO DU ===\n";
        ResetColor();

        fflush(stdin);
        SetColor(10);
        cout << "Nhap ma khach hang can kiem tra so du: ";
        ResetColor();
        fgets(ma, sizeof(ma), stdin);
        ma[strcspn(ma, "\n")] = 0;

        bool found = false;
        for (const auto& kh : danhSachKH) {
            if (strcmp(kh.maKH, ma) == 0) {
                SetColor(14);
                cout << "\nSo du hien tai cua tai khoan " << kh.tenKH << " (" << kh.maKH << ") la: "
                     << fixed << setprecision(2) << kh.soDu << endl;
                ResetColor();
                found = true;
                break;
            }
        }

        if (!found) {
            SetColor(12);
            cout << "\nKhong tim thay khach hang co ma " << ma << endl;
            ResetColor();
        }
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void hienThiDanhSach() const {
        if (danhSachKH.empty()) {
            SetColor(12);
            cout << "\nDanh sach khach hang trong!\n";
            ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc...";
            _getch();
            return;
        }

        SetColor(11);
        cout << "\n  === DANH SACH KHACH HANG ===\n";
        ResetColor();

        SetColor(14);
        cout << setw(10) << left << "Ma KH"
             << setw(20) << left << "Ten KH"
             << setw(15) << right << "So du" << endl;
        cout << string(45, '-') << endl;
        ResetColor();

        for (const auto& kh : danhSachKH) {
            SetColor(7);
            cout << setw(10) << left << kh.maKH
                 << setw(20) << left << kh.tenKH
                 << setw(15) << right << fixed << setprecision(2) << kh.soDu << endl;
        }
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void hienThiLogGiaoDich() const {
        if (danhSachGD.empty()) {
            SetColor(12);
            cout << "\nKhong co giao dich nao duoc ghi!\n";
            ResetColor();
            cout << "\nNhan phim bat ky de tiep tuc...";
            _getch();
            return;
        }

        SetColor(11);
        cout << "\n  === LICH SU GIAO DICH ===\n";
        ResetColor();

        SetColor(14);
        cout << setw(20) << left << "Thoi gian"
             << setw(15) << left << "Loai GD"
             << setw(15) << left << "Ma KH Nguon"
             << setw(15) << left << "Ma KH Dich"
             << setw(15) << right << "So tien" << endl;
        cout << string(90, '-') << endl;
        ResetColor();

        for (const auto& gd : danhSachGD) {
            SetColor(7);
            char buffer[MAX_THOI_GIAN_STR];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&gd.thoiGian));
            cout << setw(20) << left << buffer
                 << setw(15) << left << gd.loaiGiaoDich
                 << setw(15) << left << gd.maKHNguon;
            if (strcmp(gd.maKHDich, "---") != 0) { // Chi hien thi ma dich neu co
                cout << setw(15) << left << gd.maKHDich;
            } else {
                cout << setw(15) << left << "---";
            }
            cout << setw(15) << right << fixed << setprecision(2) << gd.soTien;
            if (strlen(gd.ghiChu) > 0) {
                cout << " (" << gd.ghiChu << ")";
            }
            cout << endl;
        }
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }
};

// --- Hien thi va Xu ly Menu Dong ---
void hienThiMenuChinh() {
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
        "Thoat chuong trinh"
    };

    int currentSelection = 0;
    int menuStartY = 5;

    while (true) {
        system("cls");

        SetColor(11);
        cout << "\n  ===== HE THONG QUAN LY TAI KHOAN =====\n";
        ResetColor();

        for (int i = 0; i < menuItems.size(); ++i) {
            Gotoxy(0, menuStartY + i);
            if (i == currentSelection) {
                SetColor(10);
                cout << ">> " << menuItems[i] << " <<";
            } else {
                SetColor(14);
                cout << "   " << menuItems[i] << "   ";
            }
            ResetColor();
        }

        int key = _getch();

        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
                case 72: // Up arrow
                    currentSelection = (currentSelection - 1 + menuItems.size()) % menuItems.size();
                    break;
                case 80: // Down arrow
                    currentSelection = (currentSelection + 1) % menuItems.size();
                    break;
            }
        } else if (key == 13) { // Enter key
            if (currentSelection == menuItems.size() - 1) {
                SetColor(10);
                cout << "\nCam on ban da su dung chuong trinh!\n";
                ResetColor();
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
    SetConsoleTitle(TEXT("He Thong Quan Ly Tai Khoan"));
    hienThiMenuChinh();
    return 0;
}
