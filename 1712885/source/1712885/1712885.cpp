#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<fcntl.h> //_O_U16TEXT
#include<io.h>    //_setmode()
#include<malloc.h>
#include<string.h>
#define MaxN 2048

typedef struct
{
	wchar_t MSSV[11];
	wchar_t HoTen[31];
	wchar_t Khoa[31];
	wchar_t Email[31];
	int KhoaTuyen;
	wchar_t NgaySinh[13];
	wchar_t HinhAnh[31];
	wchar_t MoTa[1001];
	wchar_t SoThich[1001];
}SINHVIEN;

typedef struct
{
	wchar_t* Title1 = L"<title>";
	wchar_t* NameMSSV = L"\"Personal_FullName\">";
	wchar_t* Khoa = L"\"Personal_Department\">";
	wchar_t* Email = L"\"Personal_Phone\">";
	wchar_t* HinhAnh = L"Personal_HinhcanhanKhung";
	wchar_t* ThongTin = L">Thông tin cá nhân";
	wchar_t* SoThich = L">Sở thích";
	wchar_t* TestList = L"TextInList";
	wchar_t* Mota = L"Description\">";
}TUKHOA;

long countLines(FILE* fin)
{
	long count = 0;
	wchar_t line[MaxN];
	while (fgetws(line, MaxN, fin) != NULL)
	{
		count++;
	}
	return count;
}

void TachChuoi(FILE *fin, SINHVIEN* &SV, wchar_t* thongtin, wchar_t KiTuDung)	//lấy thông tin từng trường dữ liệu
{
	wchar_t ch;
	int j = 0;
	while (1)
	{
		ch = fgetwc(fin);
		if (ch == KiTuDung)
		{
			int k = j - 1;
			while (thongtin[k] == ',' && KiTuDung == '\n')		//nếu không có sở thích thì lấy kí tự ',' trong chuỗi tạo thành chuỗi rỗng
			{
				thongtin[k] = '\0';
				k--;
			}
			thongtin[j] = '\0';
			break;
		}
		thongtin[j] = ch;
		j++;
	}
}

void LayThongtinSV(FILE* fin, SINHVIEN* &SV, long count)
{
	wchar_t ch = fgetwc(fin);	//lấy kí tự trống không cần thiết, sau đó đọc thông tin để lưu
	int i;		//đếm thứ tự sinh viên
	int j = 0;		//đếm thứ tự phần tử
	for (i = 0; i < count; ++i)
	{
		TachChuoi(fin, SV, (SV + i)->MSSV, ',');
		TachChuoi(fin, SV, (SV + i)->HoTen, ',');
		TachChuoi(fin, SV, (SV + i)->Khoa, ',');
		TachChuoi(fin, SV, (SV + i)->Email, ',');
		fwscanf(fin, L"%d", &(SV + i)->KhoaTuyen);
		ch = fgetwc(fin);	//lấy kí tự ','
		TachChuoi(fin, SV, (SV + i)->NgaySinh, ',');
		TachChuoi(fin, SV, (SV + i)->HinhAnh, ',');
		//lấy thông tin mô tả
		ch = fgetwc(fin);
		if (ch != '"')
		{
			fseek(fin, -1, SEEK_CUR);
			TachChuoi(fin, SV, (SV + i)->MoTa, ',');
		}
		else
		{
			TachChuoi(fin, SV, (SV + i)->MoTa, '"');
			ch = fgetwc(fin);	//lấy kí tự ','
		}
		//lấy thông tin sở thích
		TachChuoi(fin, SV, (SV + i)->SoThich, '\n');
	}
}

wchar_t* DatTenFile(wchar_t* MSSV)		//lưu trang web đến WEBSITE\MSSV.htm
{
	wchar_t str2[] = L".html";
	wchar_t str0[] = L"WEBSITE\\";
	int len1 = wcslen(MSSV);
	int len0 = wcslen(str0);
	int len2 = wcslen(str2);
	int len = len0 + len1 + len2;
	wchar_t *FileName = (wchar_t*)calloc(len, sizeof(wchar_t));
	if (FileName != NULL)
	{
		for (int i = 0, j = 0, k = 0; i <= len; ++i)
		{
			if (i < len0)					//ghép chuỗi WEBSITE
			{
				FileName[i] = str0[i];
			}
			else if (i < len0 + len1)				//ghép chuỗi MSSV
			{
				FileName[i] = MSSV[j];
				j++;
			}
			else
			{
				FileName[i] = str2[k];		//ghép chuỗi .HTML
				k++;
			}
		}
	}
	return FileName;
}

wchar_t* LayChuoiCon(wchar_t* ChuoiCha, int lenCon)			//lấy chuỗi con từ chuỗi cha
{
	wchar_t* str = (wchar_t*)calloc(lenCon + 1, sizeof(wchar_t));
	for (int i = 0; i < lenCon; i++)
	{
		str[i] = ChuoiCha[i];
	}
	str[lenCon] = '\0';
	return str;
}

int TimVtriChuoiCon(wchar_t* ChuoiCha, wchar_t* ChuoiCon)		//lấy vị trí của chuỗi con trong chuỗi cha để chèn chuỗi con ra sau
{
	int lenCha = wcslen(ChuoiCha);
	int lenCon = wcslen(ChuoiCon);
	for (int i = 0; i < lenCha; i++)
	{
		wchar_t* str = LayChuoiCon(ChuoiCha + i, lenCon);
		if (wcscmp(str, ChuoiCon) == 0)
			return i + lenCon;
	}
	return -1;
}

void ChenChuoiCon(wchar_t* ChuoiCha, wchar_t* ChuoiChen, int VtriChen)
{
	int lenCha = wcslen(ChuoiCha);
	int lenChen = wcslen(ChuoiChen);
	wchar_t* temp = (wchar_t*)calloc(lenCha - VtriChen + 1, sizeof(wchar_t));
	wcscpy(temp, ChuoiCha + VtriChen);
	wcscpy(ChuoiCha + VtriChen, ChuoiChen);
	wcscpy(ChuoiCha + VtriChen + lenChen, temp);
	delete[] temp;
}

int TimVtriKituPhanCachSoThich(wchar_t* ChuoiCha, wchar_t KtuPhancach)
{
	int i = 0;
	while (1)
	{
		if (ChuoiCha[i] == KtuPhancach)
		{
			return i;
		}
		if (ChuoiCha[i] == '\0') return  wcslen(ChuoiCha);
		i++;
	}
}

void GhiFile(FILE* fileMauHTML, FILE* fileOut, SINHVIEN* SV)
{
	TUKHOA TuKhoa;
	wchar_t* str = (wchar_t*)calloc(MaxN, sizeof(wchar_t));
	int vitri;
	while (!feof(fileMauHTML))
	{
		fgetws(str, MaxN, fileMauHTML);
		if (wcsstr(str, TuKhoa.Title1))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.Title1);
			ChenChuoiCon(str, L"HCMUS - ", vitri);
			vitri = TimVtriChuoiCon(str, L"HCMUS - ");
			ChenChuoiCon(str, SV->HoTen, vitri);
		}
		else if (wcsstr(str, TuKhoa.NameMSSV))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.NameMSSV);
			ChenChuoiCon(str, SV->HoTen, vitri);
			vitri = TimVtriChuoiCon(str, SV->HoTen);
			ChenChuoiCon(str, L" - ", vitri);
			vitri = TimVtriChuoiCon(str, L" - ");
			ChenChuoiCon(str, SV->MSSV, vitri);
		}
		else if (wcsstr(str, TuKhoa.Khoa))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.Khoa);
			ChenChuoiCon(str, L" Khoa ", vitri);
			vitri = TimVtriChuoiCon(str, L" Khoa ");
			ChenChuoiCon(str, SV->Khoa, vitri);
		}
		else if (wcsstr(str, TuKhoa.Email))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.Email);
			ChenChuoiCon(str, L"Email: ", vitri);
			vitri = TimVtriChuoiCon(str, L"Email: ");
			ChenChuoiCon(str, SV->Email, vitri);
		}
		else if (wcsstr(str, TuKhoa.HinhAnh))
		{
			fputws(str, fileOut);
			fgetws(str, MaxN, fileMauHTML);
			vitri = TimVtriChuoiCon(str, L"<img src=\"");
			ChenChuoiCon(str, SV->HinhAnh, vitri);
		}
		else if (wcsstr(str, TuKhoa.ThongTin))
		{
			while (1)
			{
				fputws(str, fileOut);
				fgetws(str, MaxN, fileMauHTML);
				if (wcsstr(str, TuKhoa.TestList))
				{
					fputws(str, fileOut);
					fgetws(str, MaxN, fileMauHTML);
					vitri = TimVtriChuoiCon(str, L"Họ và tên: ");
					ChenChuoiCon(str, SV->HoTen, vitri);
					fputws(str, fileOut);
					fgetws(str, MaxN, fileMauHTML);
					vitri = TimVtriChuoiCon(str, L"MSSV: ");
					ChenChuoiCon(str, SV->MSSV, vitri);
					fputws(str, fileOut);
					fgetws(str, MaxN, fileMauHTML);
					vitri = TimVtriChuoiCon(str, L"Sinh viên khoa ");
					ChenChuoiCon(str, SV->Khoa, vitri);
					fputws(str, fileOut);
					fgetws(str, MaxN, fileMauHTML);
					vitri = TimVtriChuoiCon(str, L"Ngày sinh: ");
					ChenChuoiCon(str, SV->NgaySinh, vitri);
					fputws(str, fileOut);
					fgetws(str, MaxN, fileMauHTML);
					vitri = TimVtriChuoiCon(str, L"Email: ");
					ChenChuoiCon(str, SV->Email, vitri);
					break;
				}
			}
		}
		else if (wcsstr(str, TuKhoa.Mota))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.Mota);
			ChenChuoiCon(str, SV->MoTa, vitri);
		}
		else if (wcsstr(str, TuKhoa.SoThich))
		{
			while (1)
			{
				fputws(str, fileOut);
				fgetws(str, MaxN, fileMauHTML);
				if (wcsstr(str, TuKhoa.TestList)) break;
			}
			if (wcsstr(str, TuKhoa.TestList))
			{
				fputws(str, fileOut);
				wchar_t* SoThich = SV->SoThich;
				wchar_t* ChuoiCon = (wchar_t*)calloc(255, sizeof(wchar_t));		//lưu từng sở thích trong SV->SoThich
				int VtriDauChuoi = 0;
				while (1)														//xử lí từng sở thích trong trường SV->SoThich
				{
					if (SoThich[VtriDauChuoi] == '\"')							//sở thích được đặt trong ngoặc kép
					{
						int i1 = TimVtriKituPhanCachSoThich(SoThich + VtriDauChuoi, '\"');
						int i2 = TimVtriKituPhanCachSoThich(SoThich + VtriDauChuoi + 1, '\"');
						int lenCon = i2 - i1;
						ChuoiCon = LayChuoiCon(SoThich + VtriDauChuoi + 1, lenCon);
						if (ChuoiCon[0] != '\0')
						{
							fputws(L"\n\t\t\t\t\t\t\t\t<li>", fileOut);
							fputws(ChuoiCon, fileOut);
							fputws(L"</li>", fileOut);
						}
						else fputws(L" ", fileOut);
						if (SoThich[VtriDauChuoi + lenCon + 3 - 1] == ',' && SoThich[VtriDauChuoi + lenCon + 3] == '\"') VtriDauChuoi += lenCon + 3;
						else VtriDauChuoi += lenCon + 2;
						if (SoThich[VtriDauChuoi] == '\0')
						{
							break;
						}
					}
					else
					{															//sở thích không đặt trong ngoặc kép
						int i1 = TimVtriKituPhanCachSoThich(SoThich + VtriDauChuoi, SoThich[VtriDauChuoi]);
						int i2 = TimVtriKituPhanCachSoThich(SoThich + VtriDauChuoi, ',');
						if (SoThich[VtriDauChuoi] == '\0')
						{
							i2 = TimVtriKituPhanCachSoThich(SoThich + VtriDauChuoi, '\0');
						}
						int lenCon = i2 - i1;
						ChuoiCon = LayChuoiCon(SoThich + VtriDauChuoi, lenCon);
						if (ChuoiCon[0] != '\0')
						{
							fputws(L"\n\t\t\t\t\t\t\t\t<li>", fileOut);
							fputws(ChuoiCon, fileOut);
							fputws(L"</li>\n", fileOut);
						}
						else fputws(L" ", fileOut);
						if (SoThich[VtriDauChuoi + lenCon] == ',' && SoThich[VtriDauChuoi + lenCon + 1] != '\"') VtriDauChuoi += lenCon + 1;
						else VtriDauChuoi += lenCon + 1;
						if (SoThich[VtriDauChuoi] == '\0')
						{
							break;
						}
					}
				}
				fgetws(str, MaxN, fileMauHTML);
				free(ChuoiCon);
			}
		}
		fputws(str, fileOut);
	}
	free(str);
}

void ThayDoiThongTin(SINHVIEN *SV)
{
	int luachon = 0;
	wprintf(L"Bạn có muốn thay đổi khoa mà sinh viên đang học ?(nhấn phím 1 để sửa hoặc phím 0 để giữ nguyên) : ");
	wscanf_s(L"%d", &luachon);
	getwc(stdin); //comment out this line if fgetws did read the newline character from stdin
	if (luachon == 1)
	{
		wprintf(L"Nhâp lại khoa của sinh viên :");
		fgetws(SV->Khoa, 31, stdin);
		SV->Khoa[wcslen(SV->Khoa) - 1] = L'\0';
	}
	wprintf(L"Bạn có muốn thay đổi ngày sinh của sinh viên ?(nhấn phím 1 để sửa hoặc phím 0 để giữ nguyên) : ");
	wscanf_s(L"%d", &luachon);
	getwc(stdin); //comment out this line if fgetws did read the newline character from stdin
	if (luachon == 1)
	{
		wprintf(L"Nhập lại ngày sinh của sinh viên: ");
		fgetws(SV->NgaySinh, 13, stdin);
		SV->NgaySinh[wcslen(SV->NgaySinh) - 1] = L'\0';
	}
	wprintf(L"Bạn có muốn thay đổi mô tả về sinh viên ?(nhấn phím 1 để sửa hoặc phím 0 để giữ nguyên) : ");
	wscanf_s(L"%d", &luachon);
	getwc(stdin); //comment out this line if fgetws did read the newline character from stdin
	if (luachon == 1)
	{
		wprintf(L"Nhập lại mô tả về sinh viên: ");
		fgetws(SV->MoTa, 1001, stdin);
		SV->MoTa[wcslen(SV->MoTa) - 1] = L'\0';
	}

	FILE* fileMauHTML = _wfopen(L"fileMauHTML.html", L"r, ccs=UTF-8");			//file HTML mẫu
	if (!fileMauHTML)
	{
		wprintf(L"Không mở được file fileMauHTML.html");
		fclose(fileMauHTML);
		_getch();
		return;
	}
	wchar_t* NameFout = DatTenFile(SV->MSSV);
	if (NameFout == NULL) return;
	FILE* fileOut = _wfopen(NameFout, L"w, ccs=UTF-8");						//file HTML xuất ra
	if (!fileOut)
	{
		wprintf(L"không thể mở file %ls", NameFout);
		fclose(fileOut);
		_getch();
		return;
	}
	GhiFile(fileMauHTML, fileOut, SV);
	fclose(fileOut);
	fclose(fileMauHTML);
}

void main()
{
	_setmode(_fileno(stdout), _O_U16TEXT); //needed for output
	_setmode(_fileno(stdin), _O_U16TEXT); //needed for input
	//đọc thông tin từ file CSV
	FILE* fin = _wfopen(L"thongtinsv.CSV", L"r, ccs=UTF-8");
	if (!fin)
	{
		wprintf(L"Không mở được file thongtinsv.CSV");
		fclose(fin);
		_getch();
		return;
	}
	long count = countLines(fin);		//lấy số lượng sinh viên trong file
	rewind(fin);
	SINHVIEN* SV = (SINHVIEN*)calloc(count, sizeof(SINHVIEN));
	if (SV != NULL)
	{
		LayThongtinSV(fin, SV, count);
	}
	fclose(fin);
	//mở file HTML mẫu và xuất ra file HTML mới
	for (int i = 0; i < count; i++)
	{
		FILE* fileMauHTML = _wfopen(L"fileMauHTML.html", L"r, ccs=UTF-8");			//file HTML mẫu
		if (!fileMauHTML)
		{
			wprintf(L"Không mở được file fileMauHTML.html");
			fclose(fileMauHTML);
			_getch();
			return;
		}
		wchar_t* NameFout = DatTenFile((SV + i)->MSSV);
		if (NameFout == NULL) return;
		FILE* fileOut = _wfopen(NameFout, L"w, ccs=UTF-8");						//file HTML xuất ra
		if (!fileOut)
		{
			wprintf(L"không thể mở file %ls", NameFout);
			fclose(fileOut);
			_getch();
			return;
		}
		GhiFile(fileMauHTML, fileOut, SV + i);
		fclose(fileOut);
		fclose(fileMauHTML);
	}
	//Cấu hình thông tin cá nhân
	int luachon;
	wprintf(L"\nDanh sách các sinh viên :");
	for (int i = 0; i < count; i++)
	{
		wprintf(L"\n%d) %ls", i + 1, (SV + i)->MSSV);
	}
	wprintf(L"\n\nThông tin của các sinh viên trên đây đã được xuất ra trang web.");
	wprintf(L"\nBạn có muốn sửa thông tin của sinh viên nào không ?");
	wprintf(L"\nHãy lựa chọn 1 sinh viên mà bạn muốn thay đổi bằng cách chọn thứ tự của sinh viên đó hoặc nhấn 0 để thoát chương trình: ");
	wscanf_s(L"%d", &luachon);
	getwc(stdin); //comment out this line if fgetws did read the newline character from stdin
	if (luachon != 0)
	{
		luachon--;
		ThayDoiThongTin(SV + luachon);
	}
	wprintf(L"\nChương trình đã hoàn tất. Mời bạn xem trang web của các sinh viên");
	if (SV != NULL) free(SV);
	_getch();
}