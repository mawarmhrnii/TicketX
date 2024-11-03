#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char Text[255]; 

typedef struct { // Struct untuk mendaftarkan atribut yang ada pada file inventory.txt
    Text name;
    int r_price, r_stock, r_sold, r_revenue;
    int v_price, v_stock, v_sold, v_revenue;
} Inventory;

typedef struct { // Struct untuk mendaftarkan atribut yang ada pada file sales.txt
    char date[7];
    Text name;
    Text type;
    Text show;
    int amount, price, bill;
} Sale;

void main_menu() { // Fungsi untuk mencetak menu utama
	puts("\nMenu pada aplikasi Ticketing");
	puts("==============================");
    puts("1. Tampilkan Data Transaksi");
    puts("2. Tampilkan Laporan Penjualan Tiket");
    puts("3. Tambahkan Data Transaksi");
    puts("4. Hapus Data Transaksi");
    puts("5. Edit Data Transaksi");
    puts("6. Urutkan Data Transaksi");
    puts("7. Cari Data Transaksi");
    puts("0. Keluar\n");
}

int main()
{
    // Inisialisasi pointer untuk struct Inventory dan Sale
    Inventory* inventory = (Inventory*) malloc(0);
    Sale* sales = (Sale*) malloc(0);
    int inv_len=0, sale_len=0;
    char buffer[256];

    // INVENTORY
    FILE* reader = fopen("inventory.txt", "r"); // Membuka dan membaca file inventory.txt
    fgets(buffer, 255, reader);    
    fgets(buffer, 255, reader);     
    while (fgets(buffer, 255, reader)) {
        Inventory inv;

        // Tiket Reguler
        char* token = strtok(buffer, ";");
        strcpy(inv.name, strtok(NULL, ";"));
        inv.r_price = atoi(strtok(NULL, ";"));
        inv.r_stock = atoi(strtok(NULL, ";"));
        inv.r_sold = atoi(strtok(NULL, ";"));
        inv.r_revenue = atoi(strtok(NULL, ";"));
        
        fgets(buffer, 255, reader);
        {                                               // remove '\r' and '\n'
            int len = strlen(buffer);
            for (size_t a = 0; a < len; a++) {
                if (buffer[a] == '\r' || buffer[a] == '\n') {
                    for(size_t b = a; b < len; b++)
                        buffer[b] = buffer[b + 1];
                    len--; a--;
                }

            }
        }

        // Tiket VIP
        token = strtok(buffer, ";");
        token = strtok(NULL, ";");
        inv.v_price = atoi(strtok(NULL, ";"));
        inv.v_stock = atoi(strtok(NULL, ";"));
        inv.v_sold = atoi(strtok(NULL, ";"));
        inv.v_revenue = atoi(strtok(NULL, ";"));
        
        inventory = (Inventory*) realloc(inventory, ((++inv_len) * sizeof(Inventory)));
        inventory[inv_len-1] = inv;
        fgets(buffer, 255, reader);
    }

    /* SALES */
    reader = fopen("sales.txt", "r"); // Membuka dan membaca file sales.txt 
    fgets(buffer, 255, reader);     
    fgets(buffer, 255, reader);     
    while (fgets(buffer, 255, reader)) {
        Sale sale;

        {       
            int len = strlen(buffer);
            for (size_t a = 0; a < len; a++) {
                if (buffer[a] == '\r' || buffer[a] == '\n') {
                    for(size_t b = a; b < len; b++)
                        buffer[b] = buffer[b + 1];
                    len--; a--;
                }

            }
        }
        strcpy(sale.date, strtok(buffer, ";"));
        strcpy(sale.name, strtok(NULL, ";"));
        strcpy(sale.type, strtok(NULL, ";"));
        strcpy(sale.show, strtok(NULL, ";"));
        sale.amount = atoi(strtok(NULL, ";"));
        sale.price = atoi(strtok(NULL, ";"));
        sale.bill = atoi(strtok(NULL, ";"));

        sales = (Sale*) realloc(sales, ((++sale_len) * sizeof(Sale)));
        sales[sale_len-1] = sale;
    }

    /* CHOOSE MENU */
    while (1) {
        main_menu();
        int flag;
        printf("Pilih menu: "); 
		scanf("%d", &flag); // Input untuk memilih menu utama
        printf("\n\n");

        if (flag == 1) { // Menu tampilkan transaksi data transaksi
            for (int i=0; i<sale_len; i++)
                printf("%2d. [%s] %-20s | %-20s | %-20s | %2d ticket(s) | $%d | $%d\n",
                        i+1, sales[i].date, sales[i].name, sales[i].show,
                        sales[i].type, sales[i].amount, sales[i].price, sales[i].bill);
        }
        
        else if (flag == 2) { // Menu tampilkan data laporan penjualan
            for (int i=0; i<inv_len; i++) {
                printf("%2d. %s\n", i+1, inventory[i].name);
                printf("    Reguler -> Stok: %4d/%4d | Harga: $%2d | Terjual: %2d | Total pendapatan: $%4d\n", // Laporan untuk tiket reguler
                        inventory[i].r_stock, (inventory[i].r_sold + inventory[i].r_stock),
                        inventory[i].r_price, inventory[i].r_sold, (inventory[i].r_price * inventory[i].r_sold));
                printf("    VIP     -> Stok: %4d/%4d | Harga: $%2d | Terjual: %2d | Total pendapatan: $%4d\n", // Laporan untuk tiket VIP
                        inventory[i].v_stock, (inventory[i].v_sold + inventory[i].v_stock),
                        inventory[i].v_price, inventory[i].v_sold, (inventory[i].v_price * inventory[i].v_sold));
                printf("\n");
            }
        }
        
        else if (flag == 3) { // Menu untuk menambah data transaksi
            getchar();
            Text date, name, type, show, amt;
            printf("Tanggal: "); fgets(date, 255, stdin); date[strlen(date)-1] = '\0';
            printf("Event: "); fgets(show, 255, stdin); show[strlen(show)-1] = '\0';
            printf("Jenis [VIP/Reguler]: "); fgets(type, 255, stdin); type[strlen(type)-1] = '\0';
            printf("Pembeli: "); fgets(name, 255, stdin); name[strlen(name)-1] = '\0';
            printf("Jumlah: "); fgets(amt, 255, stdin); int amount = atoi(amt);
            
            Sale sale; sale.amount = amount;
            strcpy(sale.date, date); strcpy(sale.name, name);
            strcpy(sale.type, type); strcpy(sale.show,show);
            for (int i=0; i<inv_len; i++) {
                if (strcmp(show,inventory[i].name) == 0) { 
                    if (strcmp(type, "VIP") == 0) { // Untuk memodifikasi stok tiket VIP pada file inventory.txt
                        if (inventory[i].v_stock > 0) {
                            inventory[i].v_stock -= amount;
                            inventory[i].v_sold += amount;
                            sale.price = inventory[i].v_price;
                            sale.bill += amount * sale.price;
                            inventory[i].v_revenue += sale.bill;
                        }
                        else {
                            puts("STOK HABIS!");
                            sale.bill = -404;
                        }
                    }

                    else if (strcmp(type, "Reguler") == 0) { // Untuk memodifikasi stok tiket Reguler pada file inventory.txt
                        if (inventory[i].r_stock > 0) {
                            inventory[i].r_stock -= amount;
                            inventory[i].r_sold += amount;
                            sale.price = inventory[i].r_price;
                            sale.bill += amount * sale.price;
                            inventory[i].r_revenue += sale.bill;
                        }
                        else {
                            puts("STOK HABIS!");
                            sale.bill = -404;
                        }
                    }
                }
            }
			
            if (sale.bill == 0)
                puts("TIKET TIDAK VALID!");
            else {
                sales = (Sale*) realloc(sales, (++sale_len) * sizeof(Sale));
                sales[sale_len-1] = sale;
                puts("Transaksi berhasil");
            }


        }
        
        else if (flag == 4) {  // Menu untuk menghapus data transaksi
            getchar();
            char _ch[5];
            printf("Transaksi no: "); fgets(_ch, 4, stdin);
            int idx = atoi(_ch);
            if (idx < 0 || idx > sale_len)
                puts("Invalid index!!");
            else {
                for (int i=idx-1; i<sale_len-1; i++)
                    sales[i] = sales[i+1];
                sale_len--;
                sales = (Sale*) realloc(sales, sale_len * sizeof(Sale));
                puts("Berhasil");
            }
        }
        
        else if (flag == 5) { // Menu untuk mengedit data transaksi
            getchar();
            char _ch[5];
            printf("Transaksi no: "); fgets(_ch, 4, stdin);
            int idx = atoi(_ch);
            if (idx < 0 || idx > sale_len)
                puts("Invalid index!!");
            else {
                Text date, name, type, show, amt; int amount;
                printf("Tanggal     -> "); fgets(date, 255, stdin); date[strlen(date)-1] = '\0';
                printf("Event       -> "); fgets(show, 255, stdin); show[strlen(show)-1] = '\0';
                printf("Jenis tiket -> "); fgets(type, 255, stdin); type[strlen(type)-1] = '\0';
                printf("Nama        -> "); fgets(name, 255, stdin); name[strlen(name)-1] = '\0';
                printf("Jumlah baru -> "); fgets(amt, 255, stdin); amount = atoi(amt);
                
                Sale sale = sales[idx-1];
                for (int i=0; i<inv_len; i++) {
                    if (strcmp(inventory[i].name, sale.show) == 0) {
                        if (strcmp(sale.type, "VIP") == 0) {
                            inventory[i].v_revenue -= sale.amount * sale.price;
                            inventory[i].v_sold -= sale.amount;
                            inventory[i].v_stock += sale.amount;
                        } else {
                            inventory[i].r_revenue -= sale.amount * sale.price;
                            inventory[i].r_sold -= sale.amount;
                            inventory[i].r_stock += sale.amount;
                        }
                        break;
                    }
                }

                for (int i=0; i<inv_len; i++) {
                    if (strcmp(inventory[i].name, show) == 0) {
                        if (strcmp(type, "VIP") == 0) {
                            inventory[i].v_revenue += amount * inventory[i].v_price;
                            sale.bill = amount * inventory[i].v_price;
                            sale.price = inventory[i].v_price;
                            inventory[i].v_sold += amount;
                            inventory[i].v_stock -= amount;
                        } else {
                            inventory[i].r_revenue += amount * inventory[i].r_price;
                            sale.bill = amount * inventory[i].r_price;
                            sale.price = inventory[i].r_price;
                            inventory[i].r_sold += amount;
                            inventory[i].r_stock -= amount;
                        }
                        break;
                    }
                }

                strcpy(sale.date, date);
                strcpy(sale.show, show);
                strcpy(sale.type, type);
                strcpy(sale.name, name);
                sale.amount = amount;
                sales[idx-1] = sale;
                puts("Berhasil");
            }
        }
        
        else if (flag == 6) { // Menu untuk mengurutkan data transaksi
            /* SORT TRANSACTIONS BY TOTAL BILL */
            int min_bill = 0;
            for (int i=0; i<sale_len-1; i++) {
                min_bill = i;
                for (int j=i+1; j<sale_len; j++)
                    if (sales[j].bill > sales[min_bill].bill)
                        min_bill = j;
                    
                if (min_bill != i) {
                    Sale tmp = sales[min_bill];
                    sales[min_bill] = sales[i];
                    sales[i] = tmp;
                }
            }


            /* SORT INVENTORY BY TOTAL SOLD */
            int min_sold = 0;
            for (int i=0; i<inv_len-1; i++) {
                min_sold = i;
                for (int j=i+1; j<inv_len; j++)
                    if ((inventory[j].r_sold + inventory[j].v_sold) >
                        (inventory[min_sold].r_sold + inventory[min_sold].v_sold))
                        min_sold = j;
                
                if (min_sold != i) {
                    Inventory tmp = inventory[min_sold];
                    inventory[min_sold] = inventory[i];
                    inventory[i] = tmp;
                }
            }

            puts("Berhasil mengurutkan sales berdasarkan total bayar (descending)");
            puts("Berhasil mengurutkan inventory berdasarkan total terjual (descending)\n");
            puts("Kembali ke menu '1' untuk melihat data yang telah diurutkan");
        }
        
        else if (flag == 7) { // Menu untuk mencari data transaksi
            int idx = -404;
            int srch;
            Text date, name, type, show;
            getchar();
            
            printf("Cari berdasarkan:\n");
            printf("1.Tanggal\n");
            printf("2.Nama pelanggan\n");
            printf("Pilih menu:");
            scanf("%d", &srch);
            
            if(srch==1) // Mencari data berdasarkan tanggal
            printf("Tanggal     -> "); fgets(date, 255, stdin); date[strlen(date)-1] = '\0'; 
            
            if(srch==2) // Mencari data berdasarkan nama
            printf("Nama        -> "); fgets(name, 255, stdin); name[strlen(name)-1] = '\0';
            
            for (int i=0; i<sale_len; i++) // Untuk mengecek apakah data yang dicari terdaftar pada file
                if (strcmp(sales[i].date, date) == 0 ||
                    strcmp(sales[i].name, name) == 0)
                    idx = i;
            
            if (idx < 0)
                puts("\nTransaksi tidak ditemukan!");
            else
                printf("\nTransaksi ditemukan pada indeks %d\n", idx+1);
        }
                
        else if (flag == 0) {
            puts("\nEXIT PROGRAM\n");
            break;
        }

        else
            puts("Invalid command!");
    }

    //  Sales.txt
    FILE* writer = fopen("sales.txt", "w"); // Untuk membaca dan menuliskan data pada file sales.txt
    Text fmt = "Tanggal transaksi ; Nama pelanggan ; Jenis tiket ; Nama event; Jumlah tiket; Harga tiket; Total pembayaran\n\n";
    fputs(fmt, writer);

    for (int i=0; i<sale_len; i++)
        fprintf(writer, "%s;%s;%s;%s;%d;%d;%d\n",
                sales[i].date, sales[i].name, sales[i].type, sales[i].show,
                sales[i].amount, sales[i].price, sales[i].bill);

    // Inventory.txt 
    writer = fopen("inventory.txt", "w"); // Untuk membaca dan menuliskan data pada file inventory.txt
    strcpy(fmt,"Jenis tiket ; Nama event ; Harga tiket; Stok tersedia; Jumlah tiket terjual; Total pendapatan\n\n");
    fputs(fmt, writer);

    for (int i=0; i<inv_len; i++) {
        fprintf(writer, "Reguler Ticket;%s;%d;%d;%d;%d\n",
                inventory[i].name, inventory[i].r_price,
                inventory[i].r_stock, inventory[i].r_sold, inventory[i].r_revenue);
        fprintf(writer, "VIP Ticket;%s;%d;%d;%d;%d\n\n",
                inventory[i].name, inventory[i].v_price,
                inventory[i].v_stock, inventory[i].v_sold, inventory[i].v_revenue);
    }
    

    // Close File
    fclose(reader);
    fclose(writer);
    free(sales);
    free(inventory);

    return 0;
}
