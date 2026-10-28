#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define JUMLAH_PERALATAN 5
#define MAKS_TRANSAKSI 100
#define PANJANG_NAMA 100

/* Data peralatan disimpan menggunakan array */
const char *namaPeralatan[JUMLAH_PERALATAN] = {
    "Tenda Kapasitas 4 Orang",
    "Sleeping Bag",
    "Kompor Portable",
    "Matras Camping",
    "Lampu Camping"
};

const long long hargaPeralatan[JUMLAH_PERALATAN] = {
    50000, 15000, 20000, 10000, 12000
};

/* Struktur untuk menyimpan data setiap transaksi */
typedef struct {
    char namaPenyewa[PANJANG_NAMA];
    int jumlahUnit[JUMLAH_PERALATAN];
    int lamaSewa;
    long long subtotal;
    long long diskon;
    long long totalBayar;
    int persentaseDiskon;
} Transaksi;

/* Array untuk menyimpan riwayat transaksi */
Transaksi riwayat[MAKS_TRANSAKSI];
int jumlahTransaksi = 0;

/* Membaca satu baris input dan menangani input terlalu panjang */
void bacaBaris(char teks[], int ukuran) {
    size_t panjang;
    int karakter;

    if (fgets(teks, ukuran, stdin) == NULL) {
        printf("\nInput berakhir. Program dihentikan.\n");
        exit(0);
    }

    panjang = strlen(teks);

    if (panjang > 0 && teks[panjang - 1] == '\n') {
        teks[panjang - 1] = '\0';
    } else {
        while ((karakter = getchar()) != '\n' && karakter != EOF) {
            /* Mengosongkan sisa input */
        }
    }
}

/* Membaca bilangan bulat dengan validasi */
int bacaInteger(const char *pesan, int minimum, int maksimum) {
    char input[100];
    char *akhir;
    long nilai;

    while (1) {
        printf("%s", pesan);
        bacaBaris(input, sizeof(input));

        nilai = strtol(input, &akhir, 10);

        while (isspace((unsigned char)*akhir)) {
            akhir++;
        }

        if (input[0] != '\0' &&
            akhir != input &&
            *akhir == '\0' &&
            nilai >= minimum &&
            nilai <= maksimum) {
            return (int)nilai;
        }

        printf("Input tidak valid! Masukkan angka %d sampai %d.\n",
               minimum, maksimum);
    }
}

/* Membaca nama penyewa dan menolak nama kosong */
void bacaNama(char nama[]) {
    int valid;

    do {
        printf("Masukkan nama penyewa: ");
        bacaBaris(nama, PANJANG_NAMA);

        valid = 0;

        for (int i = 0; nama[i] != '\0'; i++) {
            if (!isspace((unsigned char)nama[i])) {
                valid = 1;
                break;
            }
        }

        if (!valid) {
            printf("Nama tidak boleh kosong!\n");
        }
    } while (!valid);
}

/* Format rupiah agar mudah dibaca */
void tampilkanRupiah(long long nominal) {
    char angka[50];
    char hasil[70];
    int panjang, posisi = 0, jumlahDigit = 0;

    snprintf(angka, sizeof(angka), "%lld", nominal);
    panjang = (int)strlen(angka);

    for (int i = 0; i < panjang; i++) {
        hasil[posisi++] = angka[i];
        jumlahDigit++;

        if ((panjang - i - 1) % 3 == 0 && i != panjang - 1) {
            hasil[posisi++] = '.';
        }
    }

    hasil[posisi] = '\0';
    printf("Rp%s", hasil);
}

/* Menampilkan daftar peralatan */
void tampilkanPeralatan(void) {
    printf("\n");
    printf("===============================================================\n");
    printf("                 DAFTAR PERALATAN CAMPING\n");
    printf("===============================================================\n");
    printf("%-5s %-35s %s\n", "No.", "Nama Peralatan", "Harga/Hari");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < JUMLAH_PERALATAN; i++) {
        printf("%-5d %-35s ", i + 1, namaPeralatan[i]);
        tampilkanRupiah(hargaPeralatan[i]);
        printf("\n");
    }

    printf("===============================================================\n");
}

/* Menentukan diskon berdasarkan lama penyewaan */
int tentukanDiskon(int lamaSewa) {
    if (lamaSewa >= 5) {
        return 15;
    } else if (lamaSewa >= 3) {
        return 10;
    } else {
        return 0;
    }
}

/* Menghitung seluruh biaya penyewaan */
void hitungBiaya(Transaksi *data) {
    data->subtotal = 0;

    for (int i = 0; i < JUMLAH_PERALATAN; i++) {
        data->subtotal += hargaPeralatan[i]
                          * data->jumlahUnit[i]
                          * data->lamaSewa;
    }

    data->persentaseDiskon = tentukanDiskon(data->lamaSewa);

    data->diskon = data->subtotal
                   * data->persentaseDiskon / 100;

    data->totalBayar = data->subtotal - data->diskon;
}

/* Menampilkan struk penyewaan */
void tampilkanStruk(const Transaksi *data) {
    printf("\n");
    printf("===============================================================\n");
    printf("                     ADVENTURE CAMP\n");
    printf("                     STRUK PENYEWAAN\n");
    printf("===============================================================\n");

    printf("Nama Penyewa : %s\n", data->namaPenyewa);
    printf("Lama Sewa    : %d hari\n", data->lamaSewa);

    printf("---------------------------------------------------------------\n");
    printf("%-30s %-8s %-15s\n",
           "Peralatan", "Jumlah", "Subtotal");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < JUMLAH_PERALATAN; i++) {
        if (data->jumlahUnit[i] > 0) {
            long long subtotalItem =
                hargaPeralatan[i]
                * data->jumlahUnit[i]
                * data->lamaSewa;

            printf("%-30s %-8d ",
                   namaPeralatan[i], data->jumlahUnit[i]);
            tampilkanRupiah(subtotalItem);
            printf("\n");
        }
    }

    printf("---------------------------------------------------------------\n");
    printf("%-30s ", "Subtotal:");
    tampilkanRupiah(data->subtotal);
    printf("\n");

    printf("Diskon (%d%%): ", data->persentaseDiskon);
    tampilkanRupiah(data->diskon);
    printf("\n");

    printf("---------------------------------------------------------------\n");
    printf("%-30s ", "TOTAL BAYAR:");
    tampilkanRupiah(data->totalBayar);
    printf("\n");

    printf("===============================================================\n");
    printf("Terima kasih telah menggunakan layanan Adventure Camp!\n");
    printf("===============================================================\n");
}

/* Memproses transaksi penyewaan baru */
void sewaPeralatan(void) {
    Transaksi data = {0};
    int totalJenisDipilih = 0;

    if (jumlahTransaksi >= MAKS_TRANSAKSI) {
        printf("\nKapasitas transaksi sudah penuh (%d transaksi).\n",
               MAKS_TRANSAKSI);
        printf("Transaksi baru tidak dapat diproses.\n");
        return;
    }

    printf("\n");
    printf("===============================================================\n");
    printf("                    FORM PENYEWAAN\n");
    printf("===============================================================\n");

    bacaNama(data.namaPenyewa);

    tampilkanPeralatan();

    printf("\nMasukkan jumlah unit setiap peralatan.\n");
    printf("Masukkan 0 jika tidak ingin menyewa peralatan tersebut.\n\n");

    for (int i = 0; i < JUMLAH_PERALATAN; i++) {
        char pesan[150];

        snprintf(pesan, sizeof(pesan),
                 "Jumlah %-30s: ", namaPeralatan[i]);

        data.jumlahUnit[i] = bacaInteger(pesan, 0, 10000);

        if (data.jumlahUnit[i] > 0) {
            totalJenisDipilih++;
        }
    }

    if (totalJenisDipilih == 0) {
        printf("\nPenyewaan dibatalkan karena belum ada peralatan dipilih.\n");
        return;
    }

    data.lamaSewa = bacaInteger(
        "Lama penyewaan (hari): ", 1, 36500
    );

    /* Menghitung subtotal, diskon, dan total pembayaran */
    hitungBiaya(&data);

    /* Menampilkan struk transaksi */
    tampilkanStruk(&data);

    /* Menyimpan transaksi ke array riwayat */
    riwayat[jumlahTransaksi] = data;
    jumlahTransaksi++;

    printf("\nTransaksi berhasil disimpan!\n");
    printf("Jumlah transaksi saat ini: %d/%d\n",
           jumlahTransaksi, MAKS_TRANSAKSI);
}

/* Menampilkan riwayat transaksi dan total pendapatan */
void tampilkanRiwayat(void) {
    long long totalPendapatan = 0;

    printf("\n");
    printf("===============================================================\n");
    printf("                     RIWAYAT TRANSAKSI\n");
    printf("===============================================================\n");

    if (jumlahTransaksi == 0) {
        printf("Belum ada transaksi penyewaan.\n");
        printf("===============================================================\n");
        return;
    }

    for (int i = 0; i < jumlahTransaksi; i++) {
        printf("\nTransaksi ke-%d\n", i + 1);
        printf("Nama Penyewa : %s\n", riwayat[i].namaPenyewa);
        printf("Lama Sewa    : %d hari\n", riwayat[i].lamaSewa);
        printf("Peralatan    : ");

        int pertama = 1;

        for (int j = 0; j < JUMLAH_PERALATAN; j++) {
            if (riwayat[i].jumlahUnit[j] > 0) {
                if (!pertama) {
                    printf(", ");
                }

                printf("%s (%d unit)",
                       namaPeralatan[j],
                       riwayat[i].jumlahUnit[j]);

                pertama = 0;
            }
        }

        printf("\nTotal Bayar  : ");
        tampilkanRupiah(riwayat[i].totalBayar);
        printf("\n");

        totalPendapatan += riwayat[i].totalBayar;
        printf("---------------------------------------------------------------\n");
    }

    printf("Jumlah Transaksi : %d\n", jumlahTransaksi);
    printf("Total Pendapatan : ");
    tampilkanRupiah(totalPendapatan);
    printf("\n");
    printf("===============================================================\n");
}

/* Menampilkan menu utama */
void tampilkanMenu(void) {
    printf("\n");
    printf("========================================\n");
    printf("             ADVENTURE CAMP\n");
    printf("       SISTEM PENYEWAAN PERALATAN\n");
    printf("========================================\n");
    printf("1. Lihat Daftar Peralatan\n");
    printf("2. Sewa Peralatan\n");
    printf("3. Lihat Riwayat Transaksi\n");
    printf("4. Keluar\n");
    printf("========================================\n");
}

/* Fungsi utama program */
int main(void) {
    int pilihan;
    int programBerjalan = 1;

    printf("Selamat datang di Adventure Camp!\n");

    while (programBerjalan) {
        tampilkanMenu();

        pilihan = bacaInteger("Pilih menu [1-4]: ", 1, 4);

        switch (pilihan) {
            case 1:
                tampilkanPeralatan();
                break;

            case 2:
                sewaPeralatan();
                break;

            case 3:
                tampilkanRiwayat();
                break;

            case 4:
                printf("\n========================================\n");
                printf("Terima kasih telah menggunakan Adventure Camp.\n");
                printf("Sampai jumpa kembali!\n");
                printf("========================================\n");
                programBerjalan = 0;
                break;

            default:
                /* Pengamanan tambahan */
                printf("Pilihan menu tidak tersedia.\n");
        }
    }

    return 0;
}
