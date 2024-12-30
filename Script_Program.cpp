#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <algorithm>
#include <iomanip>
#include <set>

using namespace std;

int pilih_data, ntipe, npendahulu, jumlahjadwalkan;
int sumberdaya[100], kegiatan[100], finished[100];
int jadwaljadi[1000][100], terjadwal[1000][100];
int selesai, jadwalkan, indeks, banyak, penuhi, prejadwal;
int st[100], ft[100], tnow, sum, tercepat, sda[100];
float x[1000];
vector<int> durasi;
vector<int> persediaansDaya;
vector<vector<int>> kebutuhansDaya;
vector<vector<int>> pendahulu;
ofstream outputFile("output.txt");

void greeting(){
    cout << endl;
    cout << "\n PROGRAM SKRIPSI" << endl;
    cout << "\n HYBRID ALGORITMA ARTIFICIAL BEE COLONY (ABC) DAN TABU SEARCH (TS) UNTUK"
            "\n MENYELESAIKAN RESOURCE-CONSTRAINED PROJECT SCHEDULING PROBLEM (RCPSP)" << endl;
    cout << "\n Dibuat oleh : Akrom Fuadi (082011233079)" << endl;
    cout << endl;
    outputFile << endl;
    outputFile << "\n PROGRAM SKRIPSI" << endl;
    outputFile << "\n HYBRID ALGORITMA ARTIFICIAL BEE COLONY (ABC) DAN TABU SEARCH (TS) UNTUK"
                  "\n MENYELESAIKAN RESOURCE-CONSTRAINED PROJECT SCHEDULING PROBLEM (RCPSP)" << endl;
    outputFile << "\n Dibuat oleh : Akrom Fuadi (082011233079)" << endl;
    outputFile << endl;
}

bool isValidInput(string input) {
    for (char c : input) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// PSEUDO 4.2.1
void inputData(int &input) {
    string pilih_data;
    do {
        cout << "\n Pilihan data sampel yang akan di running : \n" << endl;
        cout << "  1. Data Proyek dengan 10 Aktivitas " << endl;
        cout << "  2. Data Proyek dengan 25 Aktivitas " << endl;
        cout << "  3. Data Proyek dengan 60 Aktivitas " << endl;
        cout << "\n Pilih data sampel (gunakan angka 1/2/3) " << endl;
        cout << " Jawab : ";
        cin >> pilih_data;
        if (!isValidInput(pilih_data) || (pilih_data != "1" && pilih_data != "2" && pilih_data != "3")) {
            cout << "    >>> Input tidak sesuai ketentuan. Mohon isi ulang." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        } 
        else {
            input = stoi(pilih_data);
            break;
        }
    } while (true);
}

void readDurasi(vector<int>& durasi, int pilih_data) {
    ifstream file;
    if(pilih_data == 1){
        file.open("data durasi 10 aktivitas.txt");
    } 
    else if(pilih_data == 2){
        file.open("data durasi 25 aktivitas.txt");
    } 
    else if(pilih_data == 3){
        file.open("data durasi 60 aktivitas.txt");
    }
    int value;
    while (file >> value) {
        durasi.push_back(value);
    }
    file.close();
}

void readPersediaansDaya(vector<int>& persediaan, int pilih_data) {
    ifstream file;
    if(pilih_data == 1){
        file.open("data persediaan 10 aktivitas.txt");
    } 
    else if(pilih_data == 2){
        file.open("data persediaan 25 aktivitas.txt");
    } 
    else if(pilih_data == 3){
        file.open("data persediaan 60 aktivitas.txt");
    }
    int value;
    while (file >> value) {
        persediaan.push_back(value);
    }
    file.close();
}

void readKebutuhansDaya(vector<vector<int>>& kebutuhan, int pilih_data) {
    ifstream file;
    if(pilih_data == 1){
        file.open("data kebutuhan 10 aktivitas.txt");
    } 
    else if(pilih_data == 2){
        file.open("data kebutuhan 25 aktivitas.txt");
    } 
    else if(pilih_data == 3){
        file.open("data kebutuhan 60 aktivitas.txt");
    }
    string line;
    while (getline(file, line)) {
        vector<int> row;
        istringstream iss(line);
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        kebutuhan.push_back(row);
    }
    file.close();
}

void readPendahulu(vector<vector<int>>& pendahulu, int pilih_data) {
    ifstream file;
    if(pilih_data == 1){
        file.open("data pendahulu 10 aktivitas.txt");
    } 
    else if(pilih_data == 2){
        file.open("data pendahulu 25 aktivitas.txt");
    } 
    else if(pilih_data == 3){
        file.open("data pendahulu 60 aktivitas.txt");
    }
    string line;
    while (getline(file, line)) {
        vector<int> row;
        istringstream iss(line);
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        pendahulu.push_back(row);
    }
    file.close();
}

void showdata(int pilih_data){
    readDurasi(durasi, pilih_data);
    readPersediaansDaya(persediaansDaya, pilih_data);
    readKebutuhansDaya(kebutuhansDaya, pilih_data);
    readPendahulu(pendahulu, pilih_data);
    //cout << "\n Data yang akan digunakan adalah sebagai berikut, \n" << endl;
    outputFile << "\n Data yang akan digunakan adalah sebagai berikut, \n" << endl;
    for (size_t i = 0; i < durasi.size(); ++i) {
        //cout << " " << i + 1 << ". Aktivitas " << i + 1 << " :" << endl;
        //cout << "    - Durasi                : " << durasi[i] << endl;
        //cout << "    - Kebutuhan sumber daya : ";
        outputFile << " " << i + 1 << ". Aktivitas " << i + 1 << " :" << endl;
        outputFile << "    - Durasi                : " << durasi[i] << endl;
        outputFile << "    - Kebutuhan sumber daya : ";
        for (const auto& k : kebutuhansDaya[i]) {
            //cout << k << " ";
            outputFile << k << " ";
        }
        //cout << endl;
        outputFile << endl;
        //cout << "    - Aktivitas pendahulu   : ";
        outputFile << "    - Aktivitas pendahulu   : ";
        for (const auto& p : pendahulu[i]) {
            //cout << p << " ";
            outputFile << p << " ";
        }
        //cout << endl << endl;
        outputFile << endl << endl;
    }
    //cout << " Dengan persediaan sumber daya : ";
    outputFile << " Dengan persediaan sumber daya : ";
    for (const auto& p : persediaansDaya) {
        //cout << p << " ";
        outputFile << p << " ";
    }
    //cout << endl << endl;
    outputFile << endl << endl;
}

void inisialisasiParameter(int pilih_data, int &jumlahKoloni, int &maxIterasi, int &limitAbandonment) {
    cout << "\n Parameter yang akan digunakan adalah sebagai berikut,\n" << endl;
    outputFile << "\n Parameter yang akan digunakan adalah sebagai berikut,\n" << endl;
    int jumlahAktivitas = (pilih_data == 1) ? 10 : (pilih_data == 2) ? 25 : 60;
    cout << "  1. Jumlah aktivitas                 : " << jumlahAktivitas << endl;
    outputFile << "  1. Jumlah aktivitas                 : " << jumlahAktivitas << endl;
    string jumlahKoloniStr;
    do {
        cout << "  2. Jumlah koloni lebah (genap, >=2) : ";
        cin >> jumlahKoloniStr;
        if (!isValidInput(jumlahKoloniStr) || (stoi(jumlahKoloniStr) < 2 || stoi(jumlahKoloniStr) % 2 != 0)) {
            cout << "     >>> Input tidak sesuai ketentuan. Mohon isi ulang." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        } 
        else {
            jumlahKoloni = stoi(jumlahKoloniStr);
            break;
        }
    } while (true);
    int employedBee = jumlahKoloni / 2;
    int onlookerBee = jumlahKoloni / 2;
    int scoutBee = employedBee;
    int trialLimitAwal = 0;
    cout << "  3. Jumlah employed bee              : " << employedBee << endl;
    cout << "  4. Jumlah onlooker bee              : " << onlookerBee << endl;
    cout << "  5. Jumlah scout bee                 : " << scoutBee << endl;
    outputFile << "  3. Jumlah employed bee              : " << employedBee << endl;
    outputFile << "  4. Jumlah onlooker bee              : " << onlookerBee << endl;
    outputFile << "  5. Jumlah scout bee                 : " << scoutBee << endl;
    string maxIterasiStr;
    do {
        cout << "  6. Maksimum iterasi (>= 1 & <=1000) : ";
        cin >> maxIterasiStr;
        if (!isValidInput(maxIterasiStr) || (stoi(maxIterasiStr) < 1 || stoi(maxIterasiStr) > 1000)) {
            cout << "     >>> Input tidak sesuai ketentuan. Mohon isi ulang." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        } 
        else {
            maxIterasi = stoi(maxIterasiStr); 
            break;
        }
    } while (true); 
    string limitAbandonmentStr;
    int maxLimitAbandonment = jumlahKoloni / 2 * jumlahAktivitas;
    do {
        cout << "     (batas Limit of Abandonment adalah 1 <= L <= " << maxLimitAbandonment << ")" << endl;
        //outputFile << "     (batas Limit of Abandonment adalah 1 <= x <= " << maxLimitAbandonment << ")" << endl;
        cout << "  7. Limit of abandonment             : ";
        cin >> limitAbandonmentStr;
        if (!isValidInput(limitAbandonmentStr) || (stoi(limitAbandonmentStr) < 1 || stoi(limitAbandonmentStr) > maxLimitAbandonment)) {
            cout << "     >>> Input tidak sesuai ketentuan. Mohon isi ulang." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        } 
        else {
            limitAbandonment = stoi(limitAbandonmentStr);
            break;
        }
    } while (true);
    cout << "  8. Trial Limit awal                 : " << trialLimitAwal << endl << endl;
    outputFile << "  6. Maksimum iterasi (>= 1 & <=1000) : " << maxIterasi << endl;
    outputFile << "  7. Limit of abandonment             : " <<     limitAbandonment << endl;
    outputFile << "  8. Trial Limit awal                 : " << trialLimitAwal << endl << endl;
}

// PSEUDO 4.2.2
void generateNilaiPrioritasAwal(vector<float> &priorities, int n) {
    for (int i = 0; i < n; ++i) {
        priorities[i] = static_cast<float>(rand()) / RAND_MAX;
    }
}

// PSEUDO 4.2.3
void menghitungNilaiFungsiTujuanAwal() {
    int jumlahAktivitas = (pilih_data == 1) ? 10 : (pilih_data == 2) ? 25 : 60;
    tnow = 0;
    sum = 0;
    banyak = 0;
    tercepat = 0;
    penuhi = 0;
    jumlahjadwalkan = 0;
    vector<float> priorities(jumlahAktivitas);
    generateNilaiPrioritasAwal(priorities, jumlahAktivitas);
    for (int j = 0; j < jumlahAktivitas; j++) {
        kegiatan[j] = j + 1;
        finished[j] = 0;
        st[j] = 0;
        ft[j] = 0;
    }
    for (int o = 0; o < ntipe; o++) {
        sumberdaya[o] = sda[o];
    }
    while (true) {
        bool anyScheduled = false;
        int highestPriorityIndex = -1;
        float highestPriority = -1.0;
        for (int k = 0; k < jumlahAktivitas; k++) {
            if (kegiatan[k] != 0) {
                if (pendahulu[k][0] == 0 || all_of(pendahulu[k].begin(), pendahulu[k].begin() + npendahulu, [](int p){ return p == 0; })) {
                    for (int f = 0; f < ntipe; f++) {
                        if (sumberdaya[f] >= kebutuhansDaya[k][f]) {
                            penuhi++;
                        }
                    }
                    if (penuhi == ntipe && priorities[k] > highestPriority) {
                        highestPriority = priorities[k];
                        highestPriorityIndex = k;
                    }
                    penuhi = 0;
                } 
                else {
                    for (int g = 0; g < npendahulu; g++) {
                        if (pendahulu[k][g] != 0) {
                            banyak++;
                            if (find(begin(finished), end(finished), pendahulu[k][g]) != end(finished)) {
                                sum++;
                            }
                        }
                    }
                    if (banyak == sum) {
                        for (int f = 0; f < ntipe; f++) {
                            if (sumberdaya[f] >= kebutuhansDaya[k][f]) {
                                penuhi++;
                            }
                        }
                        if (penuhi == ntipe && priorities[k] > highestPriority) {
                            highestPriority = priorities[k];
                            highestPriorityIndex = k;
                        }
                        penuhi = 0;
                        banyak = 0;
                    }
                }
            }
            banyak = 0;
            sum = 0;
        }
        if (highestPriorityIndex != -1) {
            jadwaljadi[0][jumlahjadwalkan] = highestPriorityIndex + 1;
            jumlahjadwalkan++;
            st[highestPriorityIndex] = tnow;
            ft[highestPriorityIndex] = tnow + durasi[highestPriorityIndex];
            kegiatan[highestPriorityIndex] = 0;
            for (int v = 0; v < ntipe; v++) {
                sumberdaya[v] -= kebutuhansDaya[highestPriorityIndex][v];
            }
            anyScheduled = true;
        } 
        else {
            tercepat = 1000;
            for (int s = 0; s < jumlahAktivitas; s++) {
                if (ft[s] > tnow && ft[s] < tercepat) {
                    tercepat = ft[s];
                }
            }
            for (int s = 0; s < jumlahAktivitas; s++) {
                if (ft[s] == tercepat) {
                    finished[s] = s + 1;
                    for (int y = 0; y < ntipe; y++) {
                        sumberdaya[y] += kebutuhansDaya[s][y];
                    }
                }
            }
        }
        tnow = tercepat;
        prejadwal = 0;
        for (int ab = 0; ab < jumlahAktivitas; ab++) {
            prejadwal += kegiatan[ab];
        }
        if (!anyScheduled && prejadwal == 0) break;
    }
    int dur = 0;
    for (int t = 0; t < jumlahAktivitas; t++) {
        if (ft[t] > dur) {
            dur = ft[t];
        }
    }
    for (int r = 0; r < jumlahAktivitas; r++) {
        if (ft[r] == 0) {
            dur = 100;
        }
    }
    x[0] = dur;
}

// PSEUDO 4.2.4
void mencariSolusiBaru(vector<vector<float>>& new_eb, const vector<vector<float>>& eb, int jml_eb, int jml_aktivitas) {
    for (int i = 0; i < jml_eb; ++i) {
        for (int j = 0; j < jml_aktivitas; ++j) {
            float rand_val = static_cast<float>(rand()) / RAND_MAX;
            int k = rand() % jml_eb;
            new_eb[i][j] = eb[i][j] + rand_val * (eb[i][j] - eb[k][j]);
        }
    }
}

// PSEUDO 4.2.5
void membandingkanNilaiFungsiTujuan(int jml_eb, int jml_aktivitas, vector<float>& nilaiFTujuan, const vector<float>& nilaiFTujuan_FoodSourceBaru, vector<vector<float>>& eb, const vector<vector<float>>& new_eb, vector<int>& TrialLimit) {
    for (int i = 0; i < jml_eb; ++i) {
        if (nilaiFTujuan[i] > nilaiFTujuan_FoodSourceBaru[i]) {
            for (int j = 0; j < jml_aktivitas; ++j) {
                eb[i][j] = new_eb[i][j];
            }
            nilaiFTujuan[i] = nilaiFTujuan_FoodSourceBaru[i];
            TrialLimit[i] = 0; 
        } 
        else {
            TrialLimit[i]++; 
        }
    }
}

// PSEUDO 4.2.6
void menghitungProbabilitas(int jml_eb, const vector<float>& nilaiFTujuan, vector<float>& prob_kum) {
    float total_nilaiFitness = 0;
    float kum_prob = 0;
    float FitnessMax = *max_element(nilaiFTujuan.begin(), nilaiFTujuan.end());
    float FitnessMin = *min_element(nilaiFTujuan.begin(), nilaiFTujuan.end());
    vector<float> fitness_seleksi(jml_eb);
    for (int i = 0; i < jml_eb; ++i) {
        fitness_seleksi[i] = FitnessMax + FitnessMin - nilaiFTujuan[i];
    }
    for (int i = 0; i < jml_eb; ++i) {
        total_nilaiFitness += fitness_seleksi[i];
    }
    for (int i = 0; i < jml_eb; ++i) {
        float prob = fitness_seleksi[i] / total_nilaiFitness;
        kum_prob += prob;
        prob_kum[i] = kum_prob;
    }
}

// PSEUDO 4.2.11
void simpanDurasiDanPrioritas(vector<pair<float, vector<float>>>& hasilIterasi, const vector<float>& durasi, const vector<vector<float>>& prioritas) {
    for (size_t i = 0; i < durasi.size(); ++i) {
        hasilIterasi.push_back({durasi[i], prioritas[i]});
    }
}

void simpanDurasiTotalMinimal(const vector<pair<float, vector<float>>>& hasilIterasi, float& durasiMinimal, vector<int>& urutanAktivitas) {
    if (hasilIterasi.empty()) {
        durasiMinimal = -1; 
        return;
    }
    durasiMinimal = numeric_limits<float>::max();
    size_t indeksMinimal = 0;
    bool allDurationsEqual = true;
    for (size_t i = 0; i < hasilIterasi.size(); ++i) {
        if (hasilIterasi[i].first < durasiMinimal) {
            durasiMinimal = hasilIterasi[i].first;
            indeksMinimal = i;
        }
        if (i > 0 && hasilIterasi[i].first != hasilIterasi[i - 1].first) {
            allDurationsEqual = false;
        }
    }
    if (allDurationsEqual) {
        indeksMinimal = 0;
    }
    urutanAktivitas.clear();
    for (int i = 0; i < jumlahjadwalkan; ++i) {
        urutanAktivitas.push_back(jadwaljadi[0][i]);
    }
}

void simpanDurasiTotalNonMinimal(const vector<pair<float, vector<float>>>& hasilIterasi, vector<pair<float, vector<float>>>& nonTabuSolutions, float& durasiMinimal) {
    if (hasilIterasi.empty()) {
        durasiMinimal = -1; 
        return;
    }
    set<float> durasiUnik;
    durasiMinimal = numeric_limits<float>::max();
    for (const auto& iterasi : hasilIterasi) {
        if (iterasi.first < durasiMinimal) {
            durasiMinimal = iterasi.first;
        }
    }
    for (const auto& iterasi : hasilIterasi) {
        if (iterasi.first != durasiMinimal && durasiUnik.find(iterasi.first) == durasiUnik.end()) {
            durasiUnik.insert(iterasi.first);
            nonTabuSolutions.push_back(iterasi);
        }
    }
}

// PSEUDO 4.2.12
void swapMutation(const vector<pair<float, vector<float>>>& nonTabuSolutions, vector<vector<float>>& mutatedPriorities, vector<float>& newObjectiveValues, vector<pair<int, int>>& swappedPositions) {
    if (nonTabuSolutions.empty()) {
        return;
    }
    for (size_t solusiIndex = 0; solusiIndex < nonTabuSolutions.size(); ++solusiIndex) {
        vector<float> prioritas = nonTabuSolutions[solusiIndex].second;
        int pos1 = rand() % prioritas.size();
        int pos2 = rand() % prioritas.size();
        while (pos1 == pos2) {
            pos2 = rand() % prioritas.size();
        }
        swap(prioritas[pos1], prioritas[pos2]);
        mutatedPriorities.push_back(prioritas);
        swappedPositions.push_back({pos1, pos2});
        for (int i = 0; i < prioritas.size(); ++i) {
            x[i] = prioritas[i];
        }
        menghitungNilaiFungsiTujuanAwal();
        newObjectiveValues.push_back(x[0]);
    }
}

// PSEUDO 4.2.13
void bandingkanDenganTabuList(vector<float>& newObjectiveValues, float durasiMinimal, vector<vector<float>>& mutatedPriorities, vector<float>& tabuListObjectiveValues, vector<vector<float>>& tabuListPriorities) {
    for (size_t i = 0; i < newObjectiveValues.size(); ++i) {
        if (newObjectiveValues[i] < durasiMinimal) {
            swap(newObjectiveValues[i], durasiMinimal);
            swap(mutatedPriorities[i], tabuListPriorities[0]);
        }
    }
}

int main() {
    int jumlahKoloni, jumlahAktivitas, maxIterasi, limitAbandonment;
    vector<pair<float, vector<float>>> hasilIterasi;
    greeting();
    inputData(pilih_data);
    showdata(pilih_data);
    inisialisasiParameter(pilih_data, jumlahKoloni, maxIterasi, limitAbandonment);
    srand(static_cast<unsigned int>(time(0)));
    cout << "\n Berikut adalah hasil nilai fungsi tujuan untuk setiap iterasi pada Algoritma Artificial Bee Colony \n" << endl;
    outputFile << "\n Berikut adalah penyelesaiannya, \n" << endl;
    outputFile << "\n (Detail dari Iterasi ke-1) " << endl;
    outputFile << " Mencari nilai fungsi tujuan dari solusi awal dengan" << endl;
    outputFile << " membangkitkan bilangan acak [0,1] sebagai nilai prioritas dan " << endl;
    outputFile << " mencari nilai fungsi tujuannya menggunakan priority scheduling method. " << endl;
    jumlahAktivitas = durasi.size();
    ntipe = persediaansDaya.size();
    npendahulu = pendahulu[0].size();
    for (int i = 0; i < ntipe; i++) {
        sda[i] = persediaansDaya[i];
    }
    vector<vector<float>> allPriorities(jumlahKoloni / 2, vector<float>(jumlahAktivitas));
    vector<float> allDurations(jumlahKoloni / 2);
    vector<vector<int>> allStartTimes(jumlahKoloni / 2, vector<int>(jumlahAktivitas));
    vector<vector<int>> allFinishTimes(jumlahKoloni / 2, vector<int>(jumlahAktivitas));
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        generateNilaiPrioritasAwal(allPriorities[bee], jumlahAktivitas);
        menghitungNilaiFungsiTujuanAwal();
        allDurations[bee] = x[0];
        for (int i = 0; i < jumlahAktivitas; ++i) {
            allStartTimes[bee][i] = st[i];
            allFinishTimes[bee][i] = ft[i];
        }
    }
    outputFile << "\n Berikut adalah cara mencari nilai fungsi tujuan untuk solusi awal" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "\n Solusi Awal ke-" << bee + 1 << endl;
        outputFile << "\n Solusi Awal ke-" << bee + 1 << endl;
        for (int i = 0; i < jumlahAktivitas; i++) {
            //cout << "  " << (i + 1) << ". Aktivitas " << (i + 1) << ", dengan nilai prioritasnya = " << allPriorities[bee][i] << ", waktu mulai = " <<allStartTimes[bee][i] << " dan waktu selesai = " << allFinishTimes[bee][i] << endl;
            outputFile << "  " << (i + 1) << ". Aktivitas " << (i + 1) << ", dengan nilai prioritasnya = " << allPriorities[bee][i] << ", waktu mulai = " <<allStartTimes[bee][i] << " dan waktu selesai = " << allFinishTimes[bee][i] << endl;
        }
    }
    //cout << "\n Didapatkan nilai fungsi tujuan dari " << jumlahKoloni / 2 << " solusi awal adalah sebagai berikut: \n" << endl;
    outputFile << "\n Didapatkan nilai fungsi tujuan dari " << jumlahKoloni / 2 << " solusi awal adalah sebagai berikut: \n" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  Solusi Awal ke-" << bee + 1 << " menghasilkan durasi total : " << allDurations[bee] << " satuan waktu." << endl;
        outputFile << "  Solusi Awal ke-" << bee + 1 << " menghasilkan durasi total : " << allDurations[bee] << " satuan waktu." << endl;
    }
    //cout << "\n\n Mencari nilai neighbourhood dari solusi awal dan" << endl;
    //cout << " mencari nilai fungsi tujuannya menggunakan priority scheduling method. " << endl;
    outputFile << "\n\n Mencari nilai neighbourhood dari solusi awal dan" << endl;
    outputFile << " mencari nilai fungsi tujuannya menggunakan priority scheduling method. " << endl;
    vector<float> newDurations(jumlahKoloni / 2);
    vector<vector<int>> newStartTimes(jumlahKoloni / 2, vector<int>(jumlahAktivitas));
    vector<vector<int>> newFinishTimes(jumlahKoloni / 2, vector<int>(jumlahAktivitas));
    vector<vector<float>> newPriorities(jumlahKoloni / 2, vector<float>(jumlahAktivitas));
    mencariSolusiBaru(newPriorities, allPriorities, jumlahKoloni / 2, jumlahAktivitas);
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        for (int i = 0; i < jumlahAktivitas; ++i) {
            allPriorities[bee][i] = newPriorities[bee][i];
        }
        menghitungNilaiFungsiTujuanAwal();
        newDurations[bee] = x[0];
        for (int i = 0; i < jumlahAktivitas; ++i) {
            newStartTimes[bee][i] = st[i];
            newFinishTimes[bee][i] = ft[i];
        }
    }
    outputFile << "\n Berikut adalah cara mencari nilai fungsi tujuan untuk employed bee" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "\n Employed Bee ke-" << bee + 1 << endl;
        outputFile << "\n Employed Bee ke-" << bee + 1 << endl;
        for (int i = 0; i < jumlahAktivitas; i++) {
            //cout << "  " << (i + 1) << ". Aktivitas " << (i + 1) << ", dengan nilai prioritasnya = " << newPriorities[bee][i] << ", waktu mulai = " << newStartTimes[bee][i] << " dan waktu selesai = " << newFinishTimes[bee][i] << endl;
            outputFile << "  " << (i + 1) << ". Aktivitas " << (i + 1) << ", dengan nilai prioritasnya = " << newPriorities[bee][i] << ", waktu mulai = " << newStartTimes[bee][i] << " dan waktu selesai = " << newFinishTimes[bee][i] << endl;
        }
    }
    //cout << "\n Didapatkan nilai fungsi tujuan dari " << jumlahKoloni / 2 << " employed bee adalah sebagai berikut: \n" << endl;
    outputFile << "\n Didapatkan nilai fungsi tujuan dari " << jumlahKoloni / 2 << " employed bee adalah sebagai berikut: \n" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  Employed Bee ke-" << bee + 1 << " menghasilkan durasi total : " << newDurations[bee] << " satuan waktu." << endl;
        outputFile << "  Employed Bee ke-" << bee + 1 << " menghasilkan durasi total : " << newDurations[bee] << " satuan waktu." << endl;
    }
    //cout << "\n\n Membandingkan solusi awal dengan solusi employed bee " << endl;
    outputFile << "\n\n Membandingkan solusi awal dengan solusi employed bee " << endl;
    //cout << "\n Solusi Awal " << endl;
    outputFile << "\n Solusi Awal " << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  Solusi ke-" << bee + 1 << " : " << allDurations[bee] << " satuan waktu." << endl;
        outputFile << "  Solusi ke-" << bee + 1 << " : " << allDurations[bee] << " satuan waktu." << endl;
    }
    //cout << "\n Solusi Employed Bee" << endl;
    outputFile << "\n Solusi Employed Bee" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  Solusi ke-" << bee + 1 << " : " << newDurations[bee] << " satuan waktu." << endl;
        outputFile << "  Solusi ke-" << bee + 1 << " : " << newDurations[bee] << " satuan waktu." << endl;
    }
    vector<int> trialLimit(jumlahKoloni / 2, 0);
    membandingkanNilaiFungsiTujuan(jumlahKoloni / 2, jumlahAktivitas, allDurations, newDurations, allPriorities, newPriorities, trialLimit);
    //cout << "\n Solusi yang dipilih dan trial limitnya adalah sebagai berikut, \n" << endl;
    outputFile << "\n Solusi yang dipilih dan trial limitnya adalah sebagai berikut, \n" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  Solusi ke-" << bee + 1 << " terpilih dengan durasi total : " << allDurations[bee] << " satuan waktu, dan trial limit: " << trialLimit[bee] << endl;
        outputFile << "  Solusi ke-" << bee + 1 << " terpilih dengan durasi total : " << allDurations[bee] << " satuan waktu, dan trial limit: " << trialLimit[bee] << endl;
    }
    vector<float> prob_kum(jumlahKoloni / 2);
    menghitungProbabilitas(jumlahKoloni / 2, newDurations, prob_kum);
    vector<float> fitness_seleksi(jumlahKoloni / 2);
    vector<float> fitness_relative(jumlahKoloni / 2);
    vector<float> fitness_kumulatif(jumlahKoloni / 2);
    const vector<float>& nilaiFTujuan = allDurations;
    float total_fitness = 0;
    float FitnessMax = *max_element(nilaiFTujuan.begin(), nilaiFTujuan.end());
    float FitnessMin = *min_element(nilaiFTujuan.begin(), nilaiFTujuan.end());
    //cout << "\n\n Selanjutnya, akan dihitung probabilitasnya " << endl;
    outputFile << "\n\n Selanjutnya, akan dihitung probabilitasnya " << endl;
    //cout << "\n Nilai Fungsi Tujuan Max  : " << FitnessMax << endl;
    //cout << " Nilai Fungsi Tujuan Min  : " << FitnessMin << endl;
    outputFile << "\n Nilai Fungsi Tujuan Max  : " << FitnessMax << endl;
    outputFile << " Nilai Fungsi Tujuan Min  : " << FitnessMin << endl;
    //cout << "\n Fitness Seleksi, Relative, dan Kumulatif dari solusi baru adalah sebagai berikut: \n" << endl;
    outputFile << "\n Fitness Seleksi, Relative, dan Kumulatif dari solusi baru adalah sebagai berikut: \n" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        fitness_seleksi[bee] = static_cast<int>(FitnessMax + FitnessMin - allDurations[bee]); 
        total_fitness += fitness_seleksi[bee];
    }
    float kumulatif = 0;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        fitness_relative[bee] = fitness_seleksi[bee] / total_fitness;
        kumulatif += fitness_relative[bee];
        fitness_kumulatif[bee] = kumulatif;
    }
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << " Solusi ke-" << bee + 1 << " memiliki," << endl;
        //cout << "  - Fitness seleksi   : " << fitness_seleksi[bee] << endl;
        //cout << "  - Fitness relative  : " << fitness_relative[bee] << endl;
        //cout << "  - Fitness kumulatif : " << fitness_kumulatif[bee] << endl << endl;
        outputFile << " Solusi ke-" << bee + 1 << " memiliki," << endl;
        outputFile << "  - Fitness seleksi   : " << fitness_seleksi[bee] << endl;
        outputFile << "  - Fitness relative  : " << fitness_relative[bee] << endl;
        outputFile << "  - Fitness kumulatif : " << fitness_kumulatif[bee] << endl << endl;
    }
    vector<vector<float>> onlooker_bee(jumlahKoloni / 2, vector<float>(jumlahAktivitas));
    float total_duration = 0;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        total_duration += newDurations[bee];
    }
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        prob_kum[bee] = newDurations[bee] / total_duration;
        if (bee > 0) {
            prob_kum[bee] += prob_kum[bee - 1];
        }
    }
    vector<int> selectedIndices(jumlahKoloni / 2); 
    vector<float> randomValues(jumlahKoloni / 2); 
    for (int i = 0; i < jumlahKoloni / 2; ++i) {
        float rand_val = static_cast<float>(rand()) / RAND_MAX;
        randomValues[i] = rand_val; 
        for (int k = 0; k < prob_kum.size(); ++k) {
            if (rand_val <= prob_kum[k]) {
                selectedIndices[i] = k;
                break;
            }
        }
    }
    for (int i = 0; i < jumlahKoloni / 2; ++i) {
        for (int j = 0; j < jumlahAktivitas; ++j) {
            onlooker_bee[i][j] = newPriorities[selectedIndices[i]][j];
        }
    }
    //cout << " Hasil Seleksi Roulette Wheel: \n" << endl;
    outputFile << " Hasil Seleksi Roulette Wheel: \n" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  " << bee + 1 << ". Onlooker Bee ke-" << bee + 1 <<  " dengan nilai random: " << randomValues[bee] << " memilih solusi ke-" << selectedIndices[bee] + 1 << endl;
        outputFile << "  " << bee + 1 << ". Onlooker Bee ke-" << bee + 1 <<  " dengan nilai random: " << randomValues[bee] << " memilih solusi ke-" << selectedIndices[bee] + 1 << endl;
    }
    //cout << "\n\n Mencari nilai neighbourhood dari solusi yang dipilih dan" << endl;
    //cout << " mencari nilai fungsi tujuannya menggunakan priority scheduling method. " << endl;
    outputFile << "\n\n Mencari nilai neighbourhood dari solusi yang dipilih dan" << endl;
    outputFile << " mencari nilai fungsi tujuannya menggunakan priority scheduling method. " << endl;
    vector<vector<float>> newOnlookerPriorities(jumlahKoloni / 2, vector<float>(jumlahAktivitas));
    for (int i = 0; i < jumlahKoloni / 2; ++i) {
        for (int j = 0; j < jumlahAktivitas; ++j) {
            float rand_val = static_cast<float>(rand()) / RAND_MAX;
            int k = rand() % (jumlahKoloni / 2);
            newOnlookerPriorities[i][j] = onlooker_bee[i][j] + rand_val * (onlooker_bee[i][j] - onlooker_bee[k][j]);
        }
    }
    vector<float> newOnlookerDurations(jumlahKoloni / 2);
    vector<vector<int>> newOnlookerStartTimes(jumlahKoloni / 2, vector<int>(jumlahAktivitas));
    vector<vector<int>> newOnlookerFinishTimes(jumlahKoloni / 2, vector<int>(jumlahAktivitas));
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        for (int i = 0; i < jumlahAktivitas; ++i) {
            allPriorities[bee][i] = newOnlookerPriorities[bee][i];
        }
        menghitungNilaiFungsiTujuanAwal();
        newOnlookerDurations[bee] = x[0];
        for (int i = 0; i < jumlahAktivitas; ++i) {
            newOnlookerStartTimes[bee][i] = st[i];
            newOnlookerFinishTimes[bee][i] = ft[i];
        }
    }
    outputFile << "\n Berikut adalah cara mencari nilai fungsi tujuan untuk onlooker bee" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "\n Onlooker Bee ke-" << bee + 1 << endl;
        outputFile << "\n Onlooker Bee ke-" << bee + 1 << endl;
        for (int i = 0; i < jumlahAktivitas; i++) {
            //cout << "  " << (i + 1) << ". Aktivitas " << (i + 1) << ", dengan nilai prioritasnya = " << newOnlookerPriorities[bee][i] << ", waktu mulai = " << newOnlookerStartTimes[bee][i] << " dan waktu selesai = " << newOnlookerFinishTimes[bee][i] << endl;
            outputFile << "  " << (i + 1) << ". Aktivitas " << (i + 1) << ", dengan nilai prioritasnya = " << newOnlookerPriorities[bee][i] << ", waktu mulai = " << newOnlookerStartTimes[bee][i] << " dan waktu selesai = " << newOnlookerFinishTimes[bee][i] << endl;
        }
    }
    //cout << "\n Didapatkan nilai fungsi tujuan dari " << jumlahKoloni / 2 << " onlooker bee adalah sebagai berikut: \n" << endl;
    outputFile << "\n Didapatkan nilai fungsi tujuan dari " << jumlahKoloni / 2 << " onlooker bee adalah sebagai berikut: \n" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  Onlooker Bee ke-" << bee + 1 << " menghasilkan durasi total : " << newOnlookerDurations[bee] << " satuan waktu." << endl;
        outputFile << "  Onlooker Bee ke-" << bee + 1 << " menghasilkan durasi total : " << newOnlookerDurations[bee] << " satuan waktu." << endl;
    }
    //cout << "\n\n Membandingkan solusi employed bee dengan solusi onlooker bee " << endl;
    outputFile << "\n\n Membandingkan solusi employed bee dengan solusi onlooker bee " << endl;
    //cout << "\n Solusi Employed Bee" << endl;
    outputFile << "\n Solusi Employed Bee" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  Solusi ke-" << bee + 1 << " : " << newDurations[bee] << " satuan waktu." << endl;
        outputFile << "  Solusi ke-" << bee + 1 << " : " << newDurations[bee] << " satuan waktu." << endl;
    }
    //cout << "\n Solusi Onlooker Bee" << endl;
    outputFile << "\n Solusi Onlooker Bee" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        //cout << "  Solusi ke-" << bee + 1 << " : " << newOnlookerDurations[bee] << " satuan waktu." << endl;
        outputFile << "  Solusi ke-" << bee + 1 << " : " << newOnlookerDurations[bee] << " satuan waktu." << endl;
    }
    vector<int> trialLimitOnlooker(jumlahKoloni / 2, 0);
    membandingkanNilaiFungsiTujuan(jumlahKoloni / 2, jumlahAktivitas, newDurations, newOnlookerDurations, newPriorities, newOnlookerPriorities, trialLimitOnlooker);
    //cout << "\n Solusi yang dipilih dan trial limitnya adalah sebagai berikut, \n" << endl;
    outputFile << "\n Solusi yang dipilih dan trial limitnya adalah sebagai berikut, \n" << endl;
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        outputFile << "  Solusi ke-" << bee + 1 << " terpilih dengan durasi total : " << newDurations[bee] << " satuan waktu, dan trial limit: " << trialLimitOnlooker[bee] << endl;
    }
    for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
        if (trialLimitOnlooker[bee] >= limitAbandonment) {
            outputFile << "\n Employed Bee ke-" << bee + 1 << " melebihi limit abandonment. Menghasilkan solusi baru." << endl;
            generateNilaiPrioritasAwal(allPriorities[bee], jumlahAktivitas);
            menghitungNilaiFungsiTujuanAwal();
            newDurations[bee] = x[0];
            for (int i = 0; i < jumlahAktivitas; ++i) {
                newStartTimes[bee][i] = st[i];
                newFinishTimes[bee][i] = ft[i];
            }
            trialLimitOnlooker[bee] = 0; // RESET TRIAL LIMIT UNTUK FASE SCOUT BEE
            outputFile << "  Solusi baru ke-" << bee + 1 << " dengan durasi total : " << newDurations[bee] << " satuan waktu." << endl;
        }
    }
    int indeksOptimal = -1;
    vector<vector<int>> optimalStartTimes;
    vector<vector<int>> optimalFinishTimes;
    vector<float> durasiIterasiSemuaIterasi;
    int iterasi = 0;
    while (iterasi < maxIterasi) {
        mencariSolusiBaru(newPriorities, allPriorities, jumlahKoloni / 2, jumlahAktivitas);
        for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
            for (int i = 0; i < jumlahAktivitas; ++i) {
                allPriorities[bee][i] = newPriorities[bee][i];
            }
            menghitungNilaiFungsiTujuanAwal();
            newDurations[bee] = x[0];
            for (int i = 0; i < jumlahAktivitas; ++i) {
                newStartTimes[bee][i] = st[i];
                newFinishTimes[bee][i] = ft[i];
            }
        }
        membandingkanNilaiFungsiTujuan(jumlahKoloni / 2, jumlahAktivitas, allDurations, newDurations, allPriorities, newPriorities, trialLimit);
        menghitungProbabilitas(jumlahKoloni / 2, newDurations, prob_kum);
        for (int i = 0; i < jumlahKoloni / 2; ++i) {
            float rand_val = static_cast<float>(rand()) / RAND_MAX;
            for (int k = 0; k < prob_kum.size(); ++k) {
                if (rand_val <= prob_kum[k]) {
                    selectedIndices[i] = k;
                    break;
                }
            }
        }
        for (int i = 0; i < jumlahKoloni / 2; ++i) {
            for (int j = 0; j < jumlahAktivitas; ++j) {
                onlooker_bee[i][j] = newPriorities[selectedIndices[i]][j];
            }
        }
        mencariSolusiBaru(newOnlookerPriorities, onlooker_bee, jumlahKoloni / 2, jumlahAktivitas);
        for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
            for (int i = 0; i < jumlahAktivitas; ++i) {
                allPriorities[bee][i] = newOnlookerPriorities[bee][i];
            }
            menghitungNilaiFungsiTujuanAwal();
            newOnlookerDurations[bee] = x[0];
            for (int i = 0; i < jumlahAktivitas; ++i) {
                newOnlookerStartTimes[bee][i] = st[i];
                newOnlookerFinishTimes[bee][i] = ft[i];
            }
        }
        membandingkanNilaiFungsiTujuan(jumlahKoloni / 2, jumlahAktivitas, newDurations, newOnlookerDurations, newPriorities, newOnlookerPriorities, trialLimitOnlooker);
        for (int i = 0; i < jumlahKoloni / 2; ++i) {
            if (trialLimit[i] >= limitAbandonment) {
                for (int j = 0; j < jumlahAktivitas; ++j) {
                    allPriorities[i][j] = rand() % jumlahAktivitas;
                }
                trialLimit[i] = 0;
            }
            if (trialLimitOnlooker[i] >= limitAbandonment) {
                for (int j = 0; j < jumlahAktivitas; ++j) {
                    onlooker_bee[i][j] = rand() % jumlahAktivitas;
                }
                trialLimitOnlooker[i] = 0;
            }
        }
        for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
            int limitAbandonment = jumlahKoloni / 2 * jumlahAktivitas;
            if (trialLimitOnlooker[bee] >= limitAbandonment) {
                generateNilaiPrioritasAwal(allPriorities[bee], jumlahAktivitas);
                menghitungNilaiFungsiTujuanAwal();
                newDurations[bee] = x[0];
                for (int i = 0; i < jumlahAktivitas; ++i) {
                    newStartTimes[bee][i] = st[i];
                    newFinishTimes[bee][i] = ft[i];
                }
                trialLimitOnlooker[bee] = 0;
            }
        }
        for (int bee = 0; bee < jumlahKoloni / 2; ++bee) {
            allDurations[bee] = newDurations[bee];
            for (int i = 0; i < jumlahAktivitas; ++i) {
                allPriorities[bee][i] = newPriorities[bee][i];
            }
        }
        durasiIterasiSemuaIterasi.push_back(newDurations[0]); 
        hasilIterasi.push_back({newDurations[0], newPriorities[0]}); 
        iterasi++;
    }
    //cout << "\n Karena trial limit tidak melebihi limit of abandonment maka iterasi 1 selesai." << endl;
    //outputFile << "\n Karena trial limit tidak melebihi limit of abandonment maka iterasi 1 selesai." << endl;
    float durasiOptimal = durasiIterasiSemuaIterasi[0]; 
    //cout << " Didapatkan durasi total yang optimal dari iterasi 1 adalah " << durasiOptimal << " satuan waktu." << endl;
    outputFile << "\n Didapatkan durasi total yang optimal dari iterasi 1 adalah " << durasiOptimal << " satuan waktu." << endl;    
    //cout << " Dengan urutan aktivitas terjadwalnya adalah sebagai berikut: ";
    outputFile << " Dengan urutan aktivitas terjadwalnya adalah sebagai berikut: ";
    for (int i = 0; i < jumlahjadwalkan; ++i) {
        //cout << jadwaljadi[0][i] << " ";
        outputFile << jadwaljadi[0][i] << " ";
    }
    //cout << endl << endl;
    outputFile << endl << endl;
    //cout << "\n\n Selanjutnya, hasil dari setiap iterasinya adalah sebagai berikut:\n" << endl;
    outputFile << "\n\n Selanjutnya, hasil dari setiap iterasinya adalah sebagai berikut:\n" << endl;
    for (size_t i = 0; i < min(durasiIterasiSemuaIterasi.size(), static_cast<size_t>(maxIterasi)); ++i) {
        cout << "  Iterasi ke-" << i + 1 << ": Durasi total = " << durasiIterasiSemuaIterasi[i] << " satuan waktu.\n";
        outputFile << "  Iterasi ke-" << i + 1 << ": Durasi total = " << durasiIterasiSemuaIterasi[i] << " satuan waktu.\n";
    }
    // Tabu Search
    float durasiMinimal;
    vector<int> urutanAktivitas;
    simpanDurasiTotalMinimal(hasilIterasi, durasiMinimal, urutanAktivitas);
    if (durasiMinimal == -1) {
        //cout << " Tidak ada data iterasi yang tersedia." << endl;
        outputFile << " Tidak ada data iterasi yang tersedia." << endl;
    } else {
        cout << "\n\n Solusi yang masuk ke dalam tabu list = " << durasiMinimal << " satuan waktu." << endl;
        outputFile << "\n\n Durasi total paling minimal dengan durasi total = " << durasiMinimal << " satuan waktu." << endl;
        //cout << " Dengan urutan aktivitas terjadwal = ";
        outputFile << " Dengan urutan aktivitas terjadwal = ";
        for (const auto& aktivitas : urutanAktivitas) {
            //cout << aktivitas << " ";
            outputFile << aktivitas << " ";
        }
        //cout << endl;
        outputFile << endl;
    }
    //cout << " Sehingga solusi tersebut akan masuk ke dalam tabu list." << endl;
    outputFile << " Sehingga solusi tersebut akan masuk ke dalam tabu list." << endl;
    vector<pair<float, vector<float>>> nonTabuSolutions;
    simpanDurasiTotalNonMinimal(hasilIterasi, nonTabuSolutions, durasiMinimal);
    if (durasiMinimal == -1) {
        //cout << " Tidak ada data iterasi yang tersedia." << endl;
        outputFile << " Tidak ada data iterasi yang tersedia." << endl;
    } else {
        cout << "\n Sedangkan, untuk solusi yang masuk ke dalam non tabu adalah sebagai berikut:" << endl;
        outputFile << "\n Sedangkan, untuk solusi non tabu adalah sebagai berikut:" << endl;
        for (const auto& iterasi : nonTabuSolutions) {
            cout << "  Durasi total = " << iterasi.first << " satuan waktu." << endl;
            outputFile << "  Durasi total = " << iterasi.first << " satuan waktu." <<endl;
        }
    }
    //cout << "\n Selanjutnya adalah melakukan swap mutation untuk 2 posisi secara acak" << endl;
    //cout << " Dan mencari nilai fungsi tujuannya" << endl;
    outputFile << "\n Selanjutnya adalah melakukan swap mutation untuk 2 posisi secara acak" << endl; 
    outputFile << " Dan mencari nilai fungsi tujuannya" << endl;
    vector<vector<float>> mutatedPriorities;
    vector<float> newObjectiveValues;
    vector<pair<int, int>> swappedPositions; // Tambahkan ini
    swapMutation(nonTabuSolutions, mutatedPriorities, newObjectiveValues, swappedPositions);
    for (size_t solusiIndex = 0; solusiIndex < nonTabuSolutions.size(); ++solusiIndex) {
        const vector<float>& prioritas = nonTabuSolutions[solusiIndex].second;
        int pos1 = swappedPositions[solusiIndex].first;
        int pos2 = swappedPositions[solusiIndex].second;
        //cout << "\n Solusi non tabu ke-" << solusiIndex + 1 << endl;
        outputFile << "\n Solusi non tabu ke-" << solusiIndex + 1 << endl;
        //cout << "  Nilai prioritas lama : ";
        outputFile << "  Nilai prioritas lama : ";
        for (const auto& p : prioritas) {
            //cout << p << " ";
            outputFile << p << " ";
        }
        //cout << endl;
        outputFile << endl;
        //cout << "  Nilai prioritas baru : ";
        outputFile << "  Nilai prioritas baru : ";
        for (const auto& p : mutatedPriorities[solusiIndex]) {
            //cout << p << " ";
            outputFile << p << " ";
        }
        //cout << endl;
        outputFile << endl;
        //cout << "  Posisi yang ditukar adalah posisi ke-" << pos1 + 1 << " dan posisi ke-" << pos2 + 1 << endl;
        outputFile << "  Posisi yang ditukar adalah posisi ke-" << pos1 + 1 << " dan posisi ke-" << pos2 + 1 << endl;
        //cout << "  Nilai fungsi tujuan baru : " << newObjectiveValues[solusiIndex] << endl;
        outputFile << "  Nilai fungsi tujuan baru : " << newObjectiveValues[solusiIndex] << endl;
    }
    //cout << endl;
    outputFile << endl;
    vector<float> urutanAktivitasFloat(urutanAktivitas.begin(), urutanAktivitas.end());
    vector<float> tabuListObjectiveValues = {durasiMinimal};
    vector<vector<float>> tabuListPriorities = {urutanAktivitasFloat};
    bandingkanDenganTabuList(newObjectiveValues, durasiMinimal, mutatedPriorities, tabuListObjectiveValues, tabuListPriorities);
    //cout << " Membandingkan nilai fungsi tujuan hasil swap mutation dengan nilai fungsi tujuan solusi tabu list\n" << endl;
    outputFile << " Membandingkan nilai fungsi tujuan hasil swap mutation dengan nilai fungsi tujuan solusi tabu list\n" << endl;
    for (size_t i = 0; i < newObjectiveValues.size(); ++i) {
        if (newObjectiveValues[i] < durasiMinimal) {
            //cout << "  Nilai fungsi tujuan baru : " << newObjectiveValues[i] << " (lebih baik dari solusi tabu list)" << endl;
            outputFile << "  Nilai fungsi tujuan baru : " << newObjectiveValues[i] << " (lebih baik dari solusi tabu list)" << endl;
        } else {
            //cout << "  Nilai fungsi tujuan baru : " << newObjectiveValues[i] << " (tidak lebih baik dari solusi tabu list)" << endl;
            outputFile << "  Nilai fungsi tujuan baru : " << newObjectiveValues[i] << " (tidak lebih baik dari solusi tabu list)" << endl;
        }
    }
    cout << "\n\n Berikut adalah hasil nilai fungsi tujuan untuk setiap iterasi pada Algoritma Tabu Seacrh\n" << endl;
    outputFile << "\n\n Selanjutnya, hasil dari setiap iterasinya adalah sebagai berikut:\n" << endl;
    for (int iterasi = 0; iterasi < maxIterasi; ++iterasi) {
        float durasiMinimal;
        vector<int> urutanAktivitas;
        simpanDurasiTotalMinimal(hasilIterasi, durasiMinimal, urutanAktivitas);
        vector<pair<float, vector<float>>> nonTabuSolutions;
        simpanDurasiTotalNonMinimal(hasilIterasi, nonTabuSolutions, durasiMinimal);
        vector<vector<float>> mutatedPriorities;
        vector<float> newObjectiveValues;
        swapMutation(nonTabuSolutions, mutatedPriorities, newObjectiveValues, swappedPositions);
        vector<float> urutanAktivitasFloat(urutanAktivitas.begin(), urutanAktivitas.end());
        vector<float> tabuListObjectiveValues = {durasiMinimal};
        vector<vector<float>> tabuListPriorities = {urutanAktivitasFloat};
        bandingkanDenganTabuList(newObjectiveValues, durasiMinimal, mutatedPriorities, tabuListObjectiveValues, tabuListPriorities);
        cout << "  Iterasi ke-" << iterasi + 1 << ": Durasi total = " << durasiMinimal << " satuan waktu." << endl;
        outputFile << "  Iterasi ke-" << iterasi + 1 << ": Durasi total = " << durasiMinimal << " satuan waktu." << endl;
    }
    cout << "\n\n Jadi, didapatkan hasil yang optimal dari hybrid Algoritma ABC dan TS adalah sebagai berikut:" << endl;
    outputFile << "\n\n Jadi, didapatkan hasil yang optimal dari hybrid Algoritma ABC dan TS adalah sebagai berikut:" << endl;
    cout << "\n Nilai fungsi tujuan akhir: " << durasiMinimal << " satuan waktu." << endl;
    cout << " Urutan aktivitas: ";
    outputFile << "\n Nilai fungsi tujuan akhir: " << durasiMinimal << " satuan waktu." << endl;
    outputFile << " Urutan aktivitas: ";
    for (const auto& aktivitas : urutanAktivitas) {
        cout << aktivitas << " ";
        outputFile << aktivitas << " ";
    }
    cout << endl << endl;
    outputFile << endl << endl;
    cout << "\n (*Untuk hasil selengkapnya terdapat pada file 'output.txt')\n" << endl;
    outputFile.close();
    return 0;
}